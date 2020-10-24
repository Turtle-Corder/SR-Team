#include "GameObject.h"
#include "..\Headers\Nav_Manager.h"

USING(Engine)

IMPLEMENT_SINGLETON(CNavigation_Manager)

CNavigation_Manager::CNavigation_Manager()
{
}

CNavigation_Manager::~CNavigation_Manager()
{
}

HRESULT CNavigation_Manager::Setup_Navigation_Manager()
{
	return E_NOTIMPL;
}

HRESULT CNavigation_Manager::Add_NavAgent(const wstring& _strNavAgentTag, CGameObject * _pGameObject)
{
	if (nullptr == _pGameObject)
	{
		PRINT_LOG(L"GameObject is nullptr in Add_NavAgent", LOG::ENGINE);
		return E_FAIL;
	}

	CComponent* pAgentComp = _pGameObject->Get_Component(_strNavAgentTag);
	if (nullptr == pAgentComp)
	{
		PRINT_LOG(L"AgentComponent Cant Found in Add_NavAgent", LOG::ENGINE);
		return E_FAIL;
	}

	_pGameObject->AddRef();
	m_QueueDone.push(_pGameObject);
	
	return S_OK;
}

void CNavigation_Manager::Free()
{
	while (!m_QueueWait.empty())
		m_QueueWait.pop();

	while (!m_QueueDone.empty())
		m_QueueDone.pop();
}
