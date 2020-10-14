#include "stdafx.h"
#include "PreLoader.h"
#include "..\Headers\Scene_Stage0.h"

USING(Client)

CScene_Stage0::CScene_Stage0(LPDIRECT3DDEVICE9 _pDevice)
	: CScene(_pDevice)
{
}

HRESULT CScene_Stage0::Setup_Scene()
{
	SetWindowText(g_hWnd, L"CScene_Stage0");

	//if (FAILED(Setup_Layer_Skybox()))
	//	return E_FAIL;

	//if (FAILED(Setup_Layer_Terrain()))
	//	return E_FAIL;

	//if (FAILED(Setup_Layer_Camera()))
	//	return E_FAIL;

	//if (FAILED(Setup_Layer_Player()))
	//	return E_FAIL;

	//if (FAILED(Setup_Layer_Monster()))
	//	return E_FAIL;

	//if (FAILED(Setup_Layer_Environment()))
	//	return E_FAIL;

	m_pPreLoader = CPreLoader::Create(m_pDevice, SCENE_STAGE1);
	if (nullptr == m_pPreLoader)
	{
		PRINT_LOG(L"Failed To PreLoader Create in CScene_Stage0", LOG::CLIENT);
		return E_FAIL;
	}

	return S_OK;
}

_int CScene_Stage0::Update_Scene(_float _fDeltaTime)
{
	return 0;
}

_int CScene_Stage0::LateUpdate_Scene(_float _fDeltaTime)
{
	return 0;
}

CScene_Stage0 * CScene_Stage0::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CScene_Stage0* pInstance = new CScene_Stage0(_pDevice);
	if (FAILED(pInstance->Setup_Scene()))
	{
		PRINT_LOG(L"Failed To Create CScene_Stage0", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScene_Stage0::Free()
{
	Safe_Release(m_pPreLoader);

	CScene::Free();
}

HRESULT CScene_Stage0::Setup_Layer_Skybox(const wstring & LayerTag)
{
	return E_NOTIMPL;
}

HRESULT CScene_Stage0::Setup_Layer_Terrain(const wstring & LayerTag)
{
	return E_NOTIMPL;
}

HRESULT CScene_Stage0::Setup_Layer_Camera(const wstring & LayerTag)
{
	return E_NOTIMPL;
}

HRESULT CScene_Stage0::Setup_Layer_Player(const wstring & LayerTag)
{
	return E_NOTIMPL;
}

HRESULT CScene_Stage0::Setup_Layer_Monster(const wstring & LayerTag)
{
	return E_NOTIMPL;
}

HRESULT CScene_Stage0::Setup_Layer_Environment(const wstring & LayerTag)
{
	return E_NOTIMPL;
}
