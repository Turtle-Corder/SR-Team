#include "..\Headers\Management.h"

USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
	: m_pDevice_Manager(CDevice_Manager::Get_Instance())
	, m_pTimer_Manager(CTimer_Manager::Get_Instance())
	, m_pScene_Manager(CScene_Manager::Get_Instance())
{
	Safe_AddRef(m_pDevice_Manager);
	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pScene_Manager);
}

void CManagement::Free()
{
	Safe_Release(m_pScene_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pDevice_Manager);
}

HRESULT CManagement::Setup_Engine(HWND _hWnd, _uint _iWinCX, _uint _iWinCY, CDevice_Manager::DISPLAY_MODE _eDisplayMode, _uint _iSceneCnt, const wstring & _strAppTimerTag)
{
	if (nullptr == m_pDevice_Manager ||
		nullptr == m_pScene_Manager ||
		nullptr == m_pTimer_Manager)
		return E_FAIL;

	if (FAILED(m_pDevice_Manager->Setup_GraphicDevice(_hWnd, _iWinCX, _iWinCY, _eDisplayMode)))
		return E_FAIL;

	if (FAILED(m_pTimer_Manager->Setup_TimerManager()))
		return E_FAIL;

	m_strAppTimerTag = _strAppTimerTag;
	if (FAILED(m_pTimer_Manager->Add_Timer(_strAppTimerTag, true)))
		return E_FAIL;

	m_pRenderer = CRenderer::Create(m_pDevice_Manager->Get_Device());
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return S_OK;
}

_int CManagement::Update_Engine(void)
{
	if (nullptr == m_pDevice_Manager ||
		nullptr == m_pScene_Manager)
		return -1;


	//--------------------------------------------------
	// timer
	//--------------------------------------------------
	_float fDeltaTime = 0.f;
	if (m_iUpdate_Result = m_pTimer_Manager->Update_TimeManager())
		return 0;

	if (FAILED(m_pTimer_Manager->Get_DeltaTime(m_strAppTimerTag, fDeltaTime)))
		return 0;


	//--------------------------------------------------
	// Update
	//--------------------------------------------------
	if (m_iUpdate_Result = m_pScene_Manager->Update_Scene_Manager(fDeltaTime))
		return 0;


	//--------------------------------------------------
	// LateUpdate
	//--------------------------------------------------
	if (m_iUpdate_Result = m_pScene_Manager->LateUpdate_Scene_Manager(fDeltaTime))
		return 0;


	return 0;
}

HRESULT CManagement::Render_Engine(void)
{
	//--------------------------------------------------
	// 씬 변경으로인한 Frame Skip / UNDONE : enum?
	//--------------------------------------------------
	if (1 == m_iUpdate_Result)
	{
		m_iUpdate_Result = 0;
		return S_OK;
	}

	if (nullptr == m_pRenderer)
		return E_FAIL;

	if (FAILED(m_pRenderer->Render_Renderer()))
		return E_FAIL;

	return S_OK;
}

_uint CManagement::Release_Engine()
{
	_uint iRefCnt = 0;
	if (iRefCnt = CManagement::Get_Instance()->Destroy_Instance())
	{
		PRINT_LOG(L"Failed To Destroy CManagement", LOG::ENGINE);
		return iRefCnt;
	}

	// obj

	// component

	// scene
	if (iRefCnt = CScene_Manager::Get_Instance()->Destroy_Instance())
	{
		PRINT_LOG(L"Failed To Destroy CScene_Manager", LOG::ENGINE);
		return iRefCnt;
	}

	// timer
	if (iRefCnt = CTimer_Manager::Get_Instance()->Destroy_Instance())
	{
		PRINT_LOG(L"Failed To Destroy CTimer_Manager", LOG::ENGINE);
		return iRefCnt;
	}

	// device
	if (iRefCnt = CDevice_Manager::Get_Instance()->Destroy_Instance())
	{
		PRINT_LOG(L"Failed To Destroy CDevice_Manager", LOG::ENGINE);
		return iRefCnt;
	}

	return iRefCnt;
}

HRESULT CManagement::Clear_ForScene(_uint _iSceneID)
{
	return S_OK;
}

LPDIRECT3DDEVICE9 CManagement::Get_Device(void) const
{
	if (nullptr == m_pDevice_Manager)
		return nullptr;

	return m_pDevice_Manager->Get_Device();
}

HRESULT CManagement::Add_Timer(const wstring & _strTimerTag, bool _bStart)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Add_Timer(_strTimerTag, _bStart);
}

HRESULT CManagement::Get_DeltaTime(const wstring & _strTimerTag, float & _fDeltaTime)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Get_DeltaTime(_strTimerTag, _fDeltaTime);
}

HRESULT CManagement::Pause(const wstring & _strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Pause(_strTimerTag);
}

HRESULT CManagement::Resume(const wstring & _strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Resume(_strTimerTag);
}

HRESULT CManagement::Add_RendererList(CRenderer::eRENDER_TYPE _eType, CGameObject * _pObject)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Add_RendererList(_eType, _pObject);
}

HRESULT CManagement::Change_CurrentScene(_uint _iSceneID, CScene * pCurrentScene)
{
	if (nullptr == m_pScene_Manager)
		return E_FAIL;

	return m_pScene_Manager->Change_CurrentScene(_iSceneID, pCurrentScene);
}