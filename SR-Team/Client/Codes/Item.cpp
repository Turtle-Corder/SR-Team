#include "stdafx.h"
#include "Status.h"
#include "UICamera.h"
#include "Inventory.h"
#include "ItemManager.h"
#include "..\Headers\Item.h"

USING(Client)

CItem::CItem(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
	for (_uint i = 0; i < 5; ++i)
	{
		m_pPriceTextureCom[i] = nullptr;
		m_pPriceTransformCom[i] = nullptr;
		m_pPriceVIBufferCom[i] = nullptr;

		m_pNameTextureCom[i] = nullptr;
		m_pNameTransformCom[i] = nullptr;
		m_pNameVIBufferCom[i] = nullptr;

		m_pStatCom[i] = nullptr;
		m_pTextureCom[i] = nullptr;
		m_pTransformCom[i] = nullptr;
		m_pVIBufferCom[i] = nullptr;
	}
}

CItem::CItem(const CItem & _rOther)
	: CGameObject(_rOther)
{
}

void CItem::Set_RenderShopItem(bool bRender)
{
	m_bRenderShopItem = bRender;
}

void CItem::Set_RenderInvenItem(bool bRender)
{
	m_bRenderInvenItem = bRender;
}

HRESULT CItem::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CItem::Setup_GameObject(void * pArg)
{
	if (Add_Component())
		return E_FAIL;

	return S_OK;
}

int CItem::Update_GameObject(float DeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

int CItem::LateUpdate_GameObject(float DeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return GAMEOBJECT::WARN;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_UI, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CItem::Render_UI()
{
	if (m_bRenderShopItem)
		if (FAILED(Render_ShopItem()))
			return E_FAIL;
	if (m_bRenderInvenItem)
		if (FAILED(Render_InvenItem()))
			return E_FAIL;

	return S_OK;
}

HRESULT CItem::Add_Component()
{
	if (FAILED(Add_Component_Item()))
		return E_FAIL;
	if (FAILED(Add_Component_ItemName()))
		return E_FAIL;
	if (FAILED(Add_Component_ItemPrice()))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_ItemManagement", L"Com_ItemMgr", (CComponent**)&m_pItmeMgrCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CItem::Add_Component_Item()
{
	for (_uint i = 0; i < 5; ++i)
	{
		// 1. VIBuffer-------------------------------------------------------------
		TCHAR szVIBuffer[MAX_STR] = L"";
		wsprintf(szVIBuffer, L"Com_VIBuffer%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_VIBuffer_RectTexture"
			, szVIBuffer, (CComponent**)&m_pVIBufferCom[i])))
			return E_FAIL;
		//-------------------------------------------------------------------------

		// 2. Transform------------------------------------------------------------
		TCHAR szTransform[MAX_STR] = L"";
		wsprintf(szTransform, L"Com_Transform%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_Transform"
			, szTransform, (CComponent**)&m_pTransformCom[i])))
			return E_FAIL;
		//-------------------------------------------------------------------------

		// 3. Texture--------------------------------------------------------------
		TCHAR szTexture[MAX_STR] = L"";
		TCHAR szTextureName[MAX_STR] = L"";
		if (i == 0)
			wsprintf(szTextureName, L"Component_Texture_Item_GoldenSword");
		else if (i == 1)
			wsprintf(szTextureName, L"Component_Texture_Item_IronSword");
		else if (i == 2)
			wsprintf(szTextureName, L"Component_Texture_Item_DiaSword");
		else if (i == 3)
			wsprintf(szTextureName, L"Component_Texture_Item_BlackDress");
		else if (i == 4)
			wsprintf(szTextureName, L"Component_Texture_Item_PupleDress");
		wsprintf(szTexture, L"Com_Texture%d", i);

		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, szTextureName,
			szTexture, (CComponent**)&m_pTextureCom[i])))
			return E_FAIL;
		//-------------------------------------------------------------------------

		// 4. Stat-----------------------------------------------------------------
		CStatus::STAT		tStat;
		if (i == 0)
		{
			tStat.bShop = true;
			tStat.iHp = 100;
			tStat.iSort = 1;

			// 상점
			tStat.vShopPos = { 270.f, 105.f, 0.f };
			tStat.vShopNamePos = { 270.f, 150.f, 0.f };
			tStat.vShopPricePos = { 280.f, 180.f, 0.f };

			// 인벤
			tStat.vInvenPos = { 380.f, 117.f, 0.f };
		}
		else if (i == 1)
		{
			tStat.bShop = true;
			tStat.iMp = 100;
			tStat.iSort = 2;

			// 상점
			tStat.vShopPos = { 385.f, 105.f, 0.f };
			tStat.vShopNamePos = { 385.f, 150.f, 0.f };
			tStat.vShopPricePos = { 395.f, 180.f, 0.f };

			// 인벤
			tStat.vInvenPos = { 425.f, 117.f, 0.f };

		}
		else if (i == 2)
		{
			tStat.bShop = true;
			tStat.iMp = 100;
			tStat.iSort = 3;

			// 상점
			tStat.vShopPos = { 500.f, 105.f, 0.f };
			tStat.vShopNamePos = { 500.f, 150.f, 0.f };
			tStat.vShopPricePos = { 500.f, 180.f, 0.f };
		}
		else if (i == 3)
		{
			tStat.bShop = true;
			tStat.iMp = 100;
			tStat.iSort = 4;

			// 상점
			tStat.vShopPos = { 615.f, 105.f, 0.f };
			tStat.vShopNamePos = { 615.f, 150.f, 0.f };
			tStat.vShopPricePos = { 625.f, 180.f, 0.f };

		}
		else if (i == 4)
		{
			tStat.bShop = true;
			tStat.iMp = 100;
			tStat.iSort = 5;

			// 상점
			tStat.vShopPos = { 270.f, 277.f, 0.f };
			tStat.vShopNamePos = { 270.f, 317.f, 0.f };
			tStat.vShopPricePos = { 280.f, 347.f, 0.f };

			// 인벤
			tStat.vInvenPos = { 425.f, 117.f, 0.f };

		}

		tStat.vShopScale = { 40.f, 40.f, 0.f };
		tStat.vShopNameScale = { 70.f, 70.f, 0.f };
		tStat.vShopPriceScale = { 70.f, 70.f, 0.f };

		tStat.vInvenScale = { 25.f, 25.f, 0.f };

		TCHAR szStat[MAX_STR] = L"";
		wsprintf(szStat, L"Com_Stat%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_Stat",
			szStat, (CComponent**)&m_pStatCom[i], &tStat)))
			return E_FAIL;
		m_pStatCom[i]->Set_Status(tStat);
		//-------------------------------------------------------------------------
	}

	return S_OK;
}

HRESULT CItem::Add_Component_ItemName()
{
	for (_uint i = 0; i < 5; ++i)
	{
		// 1. VIBuffer
		TCHAR szVIBuffer[MAX_STR] = L"";
		wsprintf(szVIBuffer, L"Com_NameVIBuffer%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_VIBuffer_RectTexture"
			, szVIBuffer, (CComponent**)&m_pNameVIBufferCom[i])))
			return E_FAIL;

		// 2. Transform
		TCHAR szTransform[MAX_STR] = L"";
		wsprintf(szTransform, L"Com_NameTransform%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_Transform"
			, szTransform, (CComponent**)&m_pNameTransformCom[i])))
			return E_FAIL;

		// 3. Texture
		TCHAR szTexture[MAX_STR] = L"";
		TCHAR szTextureName[MAX_STR] = L"";
		if (i == 0)
			wsprintf(szTextureName, L"Component_Texture_ItemName_GoldenSword");
		else if (i == 1)
			wsprintf(szTextureName, L"Component_Texture_ItemName_IronSword");
		else if (i == 2)
			wsprintf(szTextureName, L"Component_Texture_ItemName_DiaSword");
		else if (i == 3)
			wsprintf(szTextureName, L"Component_Texture_ItemName_BlackDress");
		else if (i == 4)
			wsprintf(szTextureName, L"Component_Texture_ItemName_PupleDress");
		wsprintf(szTexture, L"Com_NameTexture%d", i);

		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, szTextureName,
			szTexture, (CComponent**)&m_pNameTextureCom[i])))
			return E_FAIL;

	}

	return S_OK;
}

HRESULT CItem::Add_Component_ItemPrice()
{
	for (_uint i = 0; i < 5; ++i)
	{
		// 1. VIBuffer
		TCHAR szVIBuffer[MAX_STR] = L"";
		wsprintf(szVIBuffer, L"Com_PriceVIBuffer%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_VIBuffer_RectTexture"
			, szVIBuffer, (CComponent**)&m_pPriceVIBufferCom[i])))
			return E_FAIL;

		// 2. Transform
		TCHAR szTransform[MAX_STR] = L"";
		wsprintf(szTransform, L"Com_PriceTransform%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_Transform"
			, szTransform, (CComponent**)&m_pPriceTransformCom[i])))
			return E_FAIL;

		// 3. Texture
		TCHAR szTexture[MAX_STR] = L"";
		TCHAR szTextureName[MAX_STR] = L"";
		if (i == 0)
			wsprintf(szTextureName, L"Component_Texture_ItemPrice_GoldenSword");
		else if (i == 1)
			wsprintf(szTextureName, L"Component_Texture_ItemPrice_IronSword");
		else if (i == 2)
			wsprintf(szTextureName, L"Component_Texture_ItemPrice_DiaSword");
		else if (i == 3)
			wsprintf(szTextureName, L"Component_Texture_ItemPrice_BlackDress");
		else if (i == 4)
			wsprintf(szTextureName, L"Component_Texture_ItemPrice_PupleDress");
		wsprintf(szTexture, L"Com_PriceTexture%d", i);

		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, szTextureName,
			szTexture, (CComponent**)&m_pPriceTextureCom[i])))
			return E_FAIL;


	}

	return S_OK;
}

HRESULT CItem::Render_ShopItem()
{
	// Z 버퍼 끄기
	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CUICamera* pCamera = (CUICamera*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_UICamera");
	if (nullptr == pCamera)
		return E_FAIL;


	for (_uint i = 0; i < 5; ++i)
	{
		// 아이템 이미지
		if (FAILED(dynamic_cast<CVIBuffer_RectTexture*>(m_pVIBufferCom[i])->Set_UITransform(
			&m_pTransformCom[i]->Get_Desc().matWorld, pCamera,
			m_pStatCom[i]->Get_Status().vShopPos, m_pStatCom[i]->Get_Status().vShopScale)))
			return E_FAIL;

		if (FAILED(m_pTextureCom[i]->SetTexture(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom[i]->Render_VIBuffer()))
			return E_FAIL;

		// 아이템 이름
		if (FAILED(dynamic_cast<CVIBuffer_RectTexture*>(m_pNameVIBufferCom[i])->Set_UITransform(
			&m_pNameTransformCom[i]->Get_Desc().matWorld, pCamera,
			m_pStatCom[i]->Get_Status().vShopNamePos, m_pStatCom[i]->Get_Status().vShopNameScale)))
			return E_FAIL;

		if (FAILED(m_pNameTextureCom[i]->SetTexture(0)))
			return E_FAIL;

		if (FAILED(m_pNameVIBufferCom[i]->Render_VIBuffer()))
			return E_FAIL;

		// 아이템 가격
		if (FAILED(dynamic_cast<CVIBuffer_RectTexture*>(m_pPriceVIBufferCom[i])->Set_UITransform(
			&m_pPriceTransformCom[i]->Get_Desc().matWorld, pCamera,
			m_pStatCom[i]->Get_Status().vShopPricePos, m_pStatCom[i]->Get_Status().vShopPriceScale)))
			return E_FAIL;

		if (FAILED(m_pPriceTextureCom[i]->SetTexture(0)))
			return E_FAIL;

		if (FAILED(m_pPriceVIBufferCom[i]->Render_VIBuffer()))
			return E_FAIL;
	}

	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	return S_OK;
}

HRESULT CItem::Render_InvenItem()
{
	// Z 버퍼 끄기
	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CUICamera* pCamera = (CUICamera*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_UICamera");
	if (nullptr == pCamera)
		return E_FAIL;

	CInventory* pInven = (CInventory*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Inven");
	if (nullptr == pInven)
		return E_FAIL;

	const list<INVEN_ITEM*> pItemList = dynamic_cast<CInventory*>(pInven)->Get_InvenList();

	for (_uint i = 0; i < 5; ++i)
	{
		for (auto& pItem : pItemList)
		{
			if (m_pStatCom[i]->Get_Status().iSort == pItem->iSort)
			{
				if (FAILED(dynamic_cast<CVIBuffer_RectTexture*>(m_pVIBufferCom[i])->Set_UITransform(
					&m_pTransformCom[i]->Get_Desc().matWorld, pCamera,
					m_pStatCom[i]->Get_Status().vInvenPos, m_pStatCom[i]->Get_Status().vInvenScale)))
					return E_FAIL;

				if (FAILED(m_pTextureCom[i]->SetTexture(0)))
					return E_FAIL;

				if (FAILED(m_pVIBufferCom[i]->Render_VIBuffer()))
					return E_FAIL;
			}
		}
	}

	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	return S_OK;
}

CItem * CItem::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CItem* pInstance = new CItem(pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CItem", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItem::Clone_GameObject(void * pArg)
{
	CItem* pInstance = new CItem(*this);
	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CItem", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem::Free()
{
	Safe_Release(m_pItmeMgrCom);

	for (_uint i = 0; i < 5; ++i)
	{
		Safe_Release(m_pPriceTextureCom[i]);
		Safe_Release(m_pPriceTransformCom[i]);
		Safe_Release(m_pPriceVIBufferCom[i]);

		Safe_Release(m_pNameTextureCom[i]);
		Safe_Release(m_pNameTransformCom[i]);
		Safe_Release(m_pNameVIBufferCom[i]);

		Safe_Release(m_pStatCom[i]);
		Safe_Release(m_pTextureCom[i]);
		Safe_Release(m_pTransformCom[i]);
		Safe_Release(m_pVIBufferCom[i]);
	}

	CGameObject::Free();
}
