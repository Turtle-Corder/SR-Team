#include "stdafx.h"
#include "Scene_Logo.h"
#include "KeyManager.h"
#include "..\Headers\MainApp.h"


#pragma region Scene_Headers
#include "Scene_Logo.h"
#pragma endregion


#pragma region GameObject_Headers
#include "Player.h"
#include "MainCamera.h"
#include "UICamera.h"
#include "MainUI.h"
#include "Shop.h"
#include "Item.h"
#include "Inventory.h"
#include "ItemManager.h"
#include "DamageInfo.h"
#pragma endregion

#pragma region Component_Headers
#include "Status.h"
#include "VIBuffer_TreeHead.h"
#include "VIBuffer_Flower.h"
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

	// device
	m_pDevice = m_pManagement->Get_Device();
	if (nullptr == m_pDevice)
	{
		PRINT_LOG(L"Device is nullptr", LOG::CLIENT);
		return E_FAIL;
	}

	Safe_AddRef(m_pDevice);

	// sprite
	m_pSprite = m_pManagement->Get_Sprite();
	if (nullptr == m_pSprite)
	{
		PRINT_LOG(L"Sprite is nullptr", LOG::CLIENT);
		return E_FAIL;
	}

	Safe_AddRef(m_pSprite);

	// font
	m_pFont = m_pManagement->Get_Font();
	if (nullptr == m_pFont)
	{
		PRINT_LOG(L"Font is nullptr", LOG::CLIENT);
		return E_FAIL;
	}

	Safe_AddRef(m_pFont);



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

#pragma region GameObject_UICamera
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_UICamera", CUICamera::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_MainUI
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_MainUI", CMainUI::Create(m_pDevice, m_pSprite, m_pFont))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Shop
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Shop", CShop::Create(m_pDevice, m_pSprite, m_pFont))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Item
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Item", CItem::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Inven
	if (FAILED(m_pManagement->Add_GameObject_Prototype(SCENE_STATIC, L"GameObject_Inven", CInventory::Create(m_pDevice, m_pSprite, m_pFont))))
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

#pragma region Component_VIBuffer_TreeHead
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_VIBuffer_TreeHead", CVIBuffer_TreeHead::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_VIBuffer_Flower
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_VIBuffer_Flower", CVIBuffer_Flower::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Transform
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Transform", CTransform::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Raycast
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Raycast", CRaycast::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Collider
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Collider", CCollider::Create(m_pDevice))))
		return E_FAIL;
#pragma  endregion
	
#pragma region Component_Status
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Status", CStatus::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_ItemManager
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_ItemManager", CItemManager::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_ItemManager
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_DamageInfo", CDamageInfo::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

	//----------------------------------------------------------------------------------------------------
	// Textures
	//----------------------------------------------------------------------------------------------------
#pragma region Component_Texture_Player
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Player", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, 
		L"../Resources/한겨미%d.png"))))
		return E_FAIL;
#pragma endregion


#pragma region Component_Textures_MainUI

	// frame
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_MainUI_Main", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/MainUI/MainUI임%d.png"))))
		return E_FAIL;

	// hp
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_MainUI_Hp", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/MainUI/HPbar%d.png"))))
		return E_FAIL;

	// mp
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_MainUI_Mp", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/MainUI/MpBar%d.png"))))
		return E_FAIL;

	// slot_left
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_MainUI_QuickSlot_Left", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/MainUI/QuickSlot_Right%d.png"))))
		return E_FAIL;

	// slot_left
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_MainUI_QuickSlot_Right", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/MainUI/QuickSlot_Right%d.png"))))
		return E_FAIL;

#pragma endregion


#pragma region Component_Textures_Shop
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Shop_ShopWnd", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/Shop/ShopWnd%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Shop_ShopScrollBar", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
		L"../Resources/2DResource/Shop/ScrollBar%d.png"))))
		return E_FAIL;
#pragma endregion


#pragma region Component_Textures_Inventory
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Inven_InvenWnd", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/인벤이야%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Inven_SellButton",CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/Inven/SellButton%d.png"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Item_Empty",CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/Item/empty%d.png"))))
		return E_FAIL;
	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Select_SellItem",CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/Item/select_sellitem%d.png"))))
		return E_FAIL;
#pragma endregion


#pragma region Component_Textures_Item

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Item_GoldenSword", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/new_Item/golden_sword%d.png"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_ItemName_GoldenSword", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/ItemName/gold_sword%d.png"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_ItemPrice_GoldenSword", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/ItemPrice/gold_sword%d.png"))))
		return E_FAIL;


	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Item_IronSword", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/new_Item/iron_sword%d.png"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_ItemName_IronSword", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/ItemName/iron_sword%d.png"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_ItemPrice_IronSword",CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/ItemPrice/iron_sword%d.png"))))
		return E_FAIL;


	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Item_DiaSword", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/new_Item/diamond_sword%d.png"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_ItemName_DiaSword", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/ItemName/diamond_sword%d.png"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_ItemPrice_DiaSword", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/ItemPrice/diamond_sword%d.png"))))
		return E_FAIL;



	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Item_BlackDress", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/new_Item/black_dress%d.png"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_ItemName_BlackDress", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/ItemName/black_dress%d.png"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_ItemPrice_BlackDress", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/ItemPrice/black_dress%d.png"))))
		return E_FAIL;



	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Item_PupleDress", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/new_Item/puple_dress%d.png"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_ItemName_PupleDress", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/ItemName/puple_dress%d.png"))))
		return E_FAIL;

	if (FAILED(m_pManagement->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_ItemPrice_PupleDress", CTexture::Create(m_pDevice, CTexture::TEXTURE_SPRITE,
			L"../Resources/2DResource/ItemPrice/puple_dress%d.png"))))
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
	CKeyManager::Destroy_Instance();

	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);
	Safe_Release(m_pDevice);
	Safe_Release(m_pManagement);

	if (CManagement::Release_Engine())
		PRINT_LOG(L"Failed To Release Engine", LOG::CLIENT);
}