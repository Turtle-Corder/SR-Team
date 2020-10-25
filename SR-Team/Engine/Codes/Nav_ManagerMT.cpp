#include "GameObject.h"
#include "NavAgent.h"
#include "..\Headers\Nav_ManagerMT.h"

USING(Engine)

IMPLEMENT_SINGLETON(CNavigation_ManagerMT)

CNavigation_ManagerMT::CNavigation_ManagerMT()
{
}

HRESULT CNavigation_ManagerMT::Setup_NavigationManager()
{
	m_hNavEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if (NULL == m_hNavEvent)
	{
		PRINT_LOG(L"Failed To Create NavEvent", LOG::ENGINE);
		return E_FAIL;
	}

	m_hNavThread = (HANDLE)_beginthreadex(NULL, 0, Call_NavUpdateThread, this, 0, NULL);
	if (NULL == m_hNavThread)
	{
		PRINT_LOG(L"Failed To NavThread Begin", LOG::ENGINE);
		return E_FAIL;
	}

	return S_OK;
}

_int CNavigation_ManagerMT::Update_NavgationManager()
{
	_int iRet = 0;
	m_iCntPerCycle = 0;

	while (true)
	{
		//--------------------------------------------------
		// ó���� ����� ���� ���
		//--------------------------------------------------
		if (m_QueueDone.empty())
			break;

		//--------------------------------------------------
		// ���к��� ��� ����
		//--------------------------------------------------
		if (m_iCntPerCycle >= m_iMaxPerCycle)
			break;

		CNavAgent* pNavAgent = m_QueueDone.front();
		m_QueueDone.pop();
		
		if (!pNavAgent)
		{
			iRet = -1;
			break;
		}

		CGameObject* pGameObject = pNavAgent->Get_Owner();
		if (!pGameObject)
		{
			iRet = -1;
			break;
		}


		//--------------------------------------------------
		// �Ϸ� ���·� ���� �� RefCnt �����ش�.
		// ���⼭ dead ó���� �� ��� layer list���� ���Ÿ� ���� �� ���� �ȴ�.
		// object update���� refcnt�� Ȯ���ؼ� dead ó���� ���� ���־�� �Ѵ�.
		//--------------------------------------------------
		pNavAgent->Set_NavDone();
		Safe_Release(pGameObject);
		++m_iCntPerCycle;
	}

	return iRet;
}

HRESULT CNavigation_ManagerMT::NavUpdateThread()
{
	DWORD dwResult = 0;

	while (!m_bThreadDown)
	{
		//--------------------------------------------------
		// ��ȣ ���������� ������ ���
		//--------------------------------------------------
		dwResult = WaitForSingleObject(m_hNavEvent, INFINITE);

		if (WAIT_OBJECT_0 != dwResult)
		{
			PRINT_LOG(L"Wait NavEvent Error!", LOG::ENGINE);
			return E_FAIL;
		}

		// TODO : �� ã��..


	}

	return S_OK;
}

_uint CNavigation_ManagerMT::Call_NavUpdateThread(LPVOID _lpParam)
{
	CNavigation_ManagerMT* pNavMgr = (CNavigation_ManagerMT*)_lpParam;
	if (nullptr == pNavMgr)
		return -1;

	if (FAILED(pNavMgr->NavUpdateThread()))
		return -1;

	return 0;
}

HRESULT CNavigation_ManagerMT::Add_NavAgent(CNavAgent* _pNavAgent)
{
	if (nullptr == _pNavAgent)
	{
		PRINT_LOG(L"NavAgent is nullptr in Add_NavAgent", LOG::ENGINE);
		return E_FAIL;
	}

	CGameObject* pGameObject = _pNavAgent->Get_Owner();
	if (nullptr == pGameObject)
	{
		PRINT_LOG(L"GameObject Not Found!", LOG::ENGINE);
		return E_FAIL;
	}

	//--------------------------------------------------
	// RefCnt���� �ø� �� Enqueue
	//--------------------------------------------------
	pGameObject->AddRef();
	m_QueueWait.push(_pNavAgent);

	//--------------------------------------------------
	// �����尡 ������� ��ȣ�� ������
	//--------------------------------------------------
	SetEvent(m_hNavEvent);
	
	return S_OK;
}

void CNavigation_ManagerMT::Free()
{
	//--------------------------------------------------
	// ��������� �����Ų �� ť�� ����� ��
	//--------------------------------------------------
	m_bThreadDown = true;
	SetEvent(m_hNavEvent);

	WaitForSingleObject(m_hNavThread, INFINITE);

	while (!m_QueueWait.empty())
	{
		CNavAgent* pNavAgent = m_QueueWait.front();
		m_QueueWait.pop();
		CGameObject* pGameObject = pNavAgent->Get_Owner();
		Safe_Release(pGameObject);
	}

	while (!m_QueueDone.empty())
	{
		CNavAgent* pNavAgent = m_QueueDone.front();
		m_QueueDone.pop();
		CGameObject* pGameObject = pNavAgent->Get_Owner();
		Safe_Release(pGameObject);
	}

	//--------------------------------------------------
	// �̺�Ʈ, ������ �ڵ� ����
	//--------------------------------------------------
	CloseHandle(m_hNavEvent);
	CloseHandle(m_hNavThread);
}
