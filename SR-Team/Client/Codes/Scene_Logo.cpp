#include "stdafx.h"
#include "PreLoader.h"
#include "Scene_Stage0.h"
#include "..\Headers\Scene_Logo.h"

USING(Client)

CScene_Logo::CScene_Logo(LPDIRECT3DDEVICE9 _pDevice)
	: CScene(_pDevice)
{
}

HRESULT CScene_Logo::Setup_Scene()
{
	SetWindowText(g_hWnd, L"Logo Scene");

	m_pPreLoader = CPreLoader::Create(m_pDevice, SCENE_STAGE0);
	if (nullptr == m_pPreLoader)
	{
		PRINT_LOG(L"Failed To PreLoader Create in CScene_Logo", LOG::CLIENT);
		return E_FAIL;
	}

	return S_OK;
}

_int CScene_Logo::Update_Scene(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return -1;

	// UNDONE : KeyManager
	if (GetAsyncKeyState(VK_RETURN) & 0x8000 && m_pPreLoader->IsFinished())
	{
		if (FAILED(pManagement->Change_CurrentScene(SCENE_STAGE0, CScene_Stage0::Create(m_pDevice))))
		{
			PRINT_LOG(L"Failed To Setup CScene_Stage0", LOG::CLIENT);
			return -1;
		}

		if (FAILED(pManagement->Clear_ForScene(SCENE_LOGO)))
		{
			PRINT_LOG(L"Failed To Clear CScene_Logo", LOG::CLIENT);
			return -1;
		}

		return 1;
	}

	return 0;
}

_int CScene_Logo::LateUpdate_Scene(_float _fDeltaTime)
{
	return 0;
}

CScene_Logo * CScene_Logo::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CScene_Logo* pInstance = new CScene_Logo(_pDevice);
	if (FAILED(pInstance->Setup_Scene()))
	{
		PRINT_LOG(L"Failed To Create CScene_Logo", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScene_Logo::Free()
{
	Safe_Release(m_pPreLoader);

	CScene::Free();
}
