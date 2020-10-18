#include "stdafx.h"
#include "Scene_Logo.h"
#include "..\Headers\MainApp.h"


#pragma region Scene_Headers
#include "Scene_Logo.h"
#pragma endregion


#pragma region GameObject_Headers
#include "Player.h"
#include "MainCamera.h"
#pragma endregion


USING(Client)

CMainApp::CMainApp()
	: m_pManagement(CManagement::Get_Instance())
{
	Safe_AddRef(m_pManagement);
}

HRESULT CMainApp::Setup_MainApp()
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	if (FAILED(m_pManagement->Setup_Engine(g_hWnd, WINCX, WINCY, CDevice_Manager::DISPLAY_WINDOW, SCENE_END, MAINAPP_TIMER)))
	{
		PRINT_LOG(L"Failed To Setup Engine", LOG::CLIENT);
		return E_FAIL;
	}

	m_pDevice = m_pManagement->Get_Device();
	if (nullptr == m_pDevice)
	{
		PRINT_LOG(L"Device is nullptr", LOG::CLIENT);
		return E_FAIL;
	}

	Safe_AddRef(m_pDevice);

	if (FAILED(m_pManagement->Change_CurrentScene(SCENE_LOGO, CScene_Logo::Create(m_pDevice))))
	{
		PRINT_LOG(L"Failed To Change_CurrentScene SCENE_LOGO", LOG::CLIENT);
		return E_FAIL;
	}

	if (FAILED(Setup_DefaultSetting()))
	{
		PRINT_LOG(L"Failed To Setup_DefaultSetting", LOG::CLIENT);
		return E_FAIL;
	}

	if (FAILED(Setup_StaticResources()))
	{
		PRINT_LOG(L"Failed To Setup_StaticResources", LOG::CLIENT);
		return E_FAIL;
	}

	return S_OK;
}

_int CMainApp::Update_MainApp()
{
	if (nullptr == m_pManagement)
		return 0;

	_int iBehaviour = m_pManagement->Update_Engine();

	return iBehaviour;
}

HRESULT CMainApp::Render_MainApp()
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	if (FAILED(m_pManagement->Render_Engine()))
	{
		PRINT_LOG(L"Failed To Render_Engine", LOG::CLIENT);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMainApp::Setup_DefaultSetting()
{
	srand(unsigned(time(NULL)));

	if (FAILED(m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE)))
	{
		PRINT_LOG(L"Failed To Light Off", LOG::CLIENT);
		return E_FAIL;
	}

	if (FAILED(m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE)))
	{
		PRINT_LOG(L"Failed To Backface Cull Off", LOG::CLIENT);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMainApp::Setup_StaticResources()
{
	//----------------------------------------------------------------------------------------------------
	// GameObject
	//----------------------------------------------------------------------------------------------------
#pragma region GameObject_Player
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Player", CPlayer::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_MainCamera
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_MainCamera", CMainCamera::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion


	//----------------------------------------------------------------------------------------------------
	// Component
	//----------------------------------------------------------------------------------------------------
#pragma region Component_VIBuffer_RectTextrue
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_VIBuffer_RectTexture", CVIBuffer_RectTexture::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_VIBuffer_CubeTextrue
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", CVIBuffer_CubeTexture::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Transform
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Transform", CTransform::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Player
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Player", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/ÇÑ°Ü¹Ì%d.png"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Raycast
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Raycast", CRaycast::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion


	//----------------------------------------------------------------------------------------------------
	// Sound
	//----------------------------------------------------------------------------------------------------
#pragma region Sound_
#pragma endregion

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp;
	if (FAILED(pInstance->Setup_MainApp()))
	{
		PRINT_LOG(L"Failed To Create CMainApp", LOG::CLIENT);
		Safe_Release(pInstance);
	}
	
	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pManagement);

	if (CManagement::Release_Engine())
		PRINT_LOG(L"Failed To Release Engine", LOG::CLIENT);
}

