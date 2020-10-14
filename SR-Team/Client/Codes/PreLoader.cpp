#include "stdafx.h"
#include "..\Headers\PreLoader.h"

USING(Client)



CPreLoader::CPreLoader(LPDIRECT3DDEVICE9 _pDevice)
	: m_pDevice(_pDevice)
{
	Safe_AddRef(_pDevice);
}

_bool CPreLoader::IsFinished()
{
	return m_bFinished;
}

HRESULT CPreLoader::Setup_PreLoader(eSCENE_ID _eNextSceneID)
{
	m_eNextSceneID = _eNextSceneID;

	InitializeCriticalSection(&m_CriticalSection);

	m_hLoadThread = (HANDLE)_beginthreadex(0, 0, ThreadMain, this, 0, 0);
	if (nullptr == m_hLoadThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CPreLoader::Load_Resources_Stage0()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	//----------------------------------------------------------------------------------------------------
	// GameObject
	//----------------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------------
	// Component
	//----------------------------------------------------------------------------------------------------



	return S_OK;
}

HRESULT CPreLoader::Load_Resources_Stage1()
{
	return E_NOTIMPL;
}

HRESULT CPreLoader::Load_Resources_Stage2()
{
	return E_NOTIMPL;
}

HRESULT CPreLoader::Load_Resources_Stage3()
{
	return E_NOTIMPL;
}

HRESULT CPreLoader::Load_Resources_Stage4()
{
	return E_NOTIMPL;
}

HRESULT CPreLoader::Load_Resources_Stage5()
{
	return E_NOTIMPL;
}

_uint CPreLoader::ThreadMain(void * _pParam)
{
	CPreLoader* pInstance = (CPreLoader*)_pParam;
	if (nullptr == pInstance)
		return 1;

	HRESULT hr = 0;
	
	EnterCriticalSection(&pInstance->m_CriticalSection);
	switch (pInstance->m_eNextSceneID)
	{
	case eSCENE_ID::SCENE_STATIC:
		break;

	case eSCENE_ID::SCENE_LOGO:
		break;

	case eSCENE_ID::SCENE_STAGE0:
		hr = pInstance->Load_Resources_Stage0();
		break;

	case eSCENE_ID::SCENE_STAGE1:
		hr = pInstance->Load_Resources_Stage1();
		break;

	case eSCENE_ID::SCENE_STAGE2:
		hr = pInstance->Load_Resources_Stage2();
		break;

	case eSCENE_ID::SCENE_STAGE3:
		hr = pInstance->Load_Resources_Stage3();
		break;

	case eSCENE_ID::SCENE_STAGE4:
		hr = pInstance->Load_Resources_Stage4();
		break;

	case eSCENE_ID::SCENE_STAGE5:
		hr = pInstance->Load_Resources_Stage5();
		break;

	default:
		break;
	}

	LeaveCriticalSection(&pInstance->m_CriticalSection);
	if (FAILED(hr))
		return 1;

	pInstance->m_bFinished = true;
	return 0;
}

CPreLoader * CPreLoader::Create(LPDIRECT3DDEVICE9 _pDevice, eSCENE_ID _eNextSceneID)
{
	if (nullptr == _pDevice)
		return nullptr;

	CPreLoader* pInstance = new CPreLoader(_pDevice);
	if (FAILED(pInstance->Setup_PreLoader(_eNextSceneID)))
	{
		PRINT_LOG(L"Failed To Create CPreLoader", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPreLoader::Free()
{
	DeleteCriticalSection(&m_CriticalSection);
	CloseHandle(m_hLoadThread);

	Safe_Release(m_pDevice);
}
