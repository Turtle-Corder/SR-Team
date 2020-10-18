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

	//if (FAILED(Setup_Layer_Skybox(L"Layer_Skybox")))
	//	return E_FAIL;

	if (FAILED(Setup_Layer_Terrain(L"Layer_Terrain")))
		return E_FAIL;

	if (FAILED(Setup_Layer_Camera(L"Layer_Camera")))
		return E_FAIL;

	if (FAILED(Setup_Layer_Player(L"Layer_Player")))
		return E_FAIL;

	//if (FAILED(Setup_Layer_Monster()))
	//	return E_FAIL;

	//if (FAILED(Setup_Layer_Environment()))
	//	return E_FAIL;

	//m_pPreLoader = CPreLoader::Create(m_pDevice, SCENE_STAGE1);
	//if (nullptr == m_pPreLoader)
	//{
	//	PRINT_LOG(L"Failed To PreLoader Create in CScene_Stage0", LOG::CLIENT);
	//	return E_FAIL;
	//}

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
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE0, L"GameObject_Skybox", SCENE_STAGE0, LayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage0::Setup_Layer_Terrain(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE0, L"GameObject_Terrain", SCENE_STAGE0, LayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage0::Setup_Layer_Camera(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera::CAMERA_DESC tCameraDesc;
	ZeroMemory(&tCameraDesc, sizeof(CCamera::CAMERA_DESC));
	D3DXMatrixIdentity(&tCameraDesc.matView);
	tCameraDesc.vUp = _vec3(0.f, 1.f, 0.f);

	D3DXMatrixIdentity(&tCameraDesc.matProj);
	tCameraDesc.fFovY = D3DXToRadian(60.f);
	tCameraDesc.fAspect = (float)WINCX / WINCY;
	tCameraDesc.fNear = 1.f;
	tCameraDesc.fFar = 500.f;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_MainCamera", SCENE_STAGE0, LayerTag, &tCameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage0::Setup_Layer_Player(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_Player", SCENE_STAGE0, LayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage0::Setup_Layer_Monster(const wstring & LayerTag)
{
	return E_NOTIMPL;
}

HRESULT CScene_Stage0::Setup_Layer_Environment(const wstring & LayerTag)
{
	return E_NOTIMPL;
}
