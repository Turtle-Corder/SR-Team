#include "..\Headers\Management.h"

USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
	: m_pDevice_Manager(CDevice_Manager::Get_Instance())
	, m_pScene_Manager(CScene_Manager::Get_Instance())
{
	Safe_AddRef(m_pDevice_Manager);
	Safe_AddRef(m_pScene_Manager);
}

void CManagement::Free()
{
	Safe_Release(m_pScene_Manager);
	Safe_Release(m_pDevice_Manager);
}

HRESULT CManagement::Setup_Engine(HWND _hWnd, _uint _iWinCX, _uint _iWinCY, CDevice_Manager::DISPLAY_MODE _eDisplayMode, _uint _iSceneCnt, const wstring & _strAppTimerTag)
{
	if (nullptr == m_pDevice_Manager ||
		nullptr == m_pScene_Manager)
		return E_FAIL;

	if (FAILED(m_pDevice_Manager->Setup_GraphicDevice(_hWnd, _iWinCX, _iWinCY, _eDisplayMode)))
		return E_FAIL;

	m_strAppTimerTag = _strAppTimerTag;

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
	// 씬 변경으로인한 Frame Skip
	// UNDONE : enum?
	//--------------------------------------------------
	if (1 == m_iUpdate_Result)
	{
		m_iUpdate_Result = 0;
		return S_OK;
	}



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

	// device


	return iRefCnt;
}

LPDIRECT3DDEVICE9 CManagement::Get_Device(void) const
{
	if (nullptr == m_pDevice_Manager)
		return nullptr;

	return m_pDevice_Manager->Get_Device();
}

HRESULT CManagement::Change_CurrentScene(_uint _iSceneID, CScene * pCurrentScene)
{
	if (nullptr == m_pScene_Manager)
		return E_FAIL;

	return m_pScene_Manager->Change_CurrentScene(_iSceneID, pCurrentScene);
}