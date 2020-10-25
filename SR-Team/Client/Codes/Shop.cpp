#include "stdafx.h"
#include "Item.h"
#include "UICamera.h"
#include "Inventory.h"
#include "KeyManager.h"
#include "..\Headers\Shop.h"

USING(Client)

CShop::CShop(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
	for (_uint i = 0; i < SHOP_END; i++)
	{
		m_pTransformCom[i] = nullptr;
		m_pTextureCom[i] = nullptr;
	}

	for (_uint i = 0; i < 15; ++i)
	{
		m_pItemTransformCom[i] = nullptr;
		m_pItemTextureCom[i] = nullptr;

		m_pItemNameTransformCom[i] = nullptr;
		m_pItemNameTextureCom[i] = nullptr;

		m_pItemPriceTransformCom[i] = nullptr;
		m_pItemPriceTextureCom[i] = nullptr;
	}
}

CShop::CShop(const CShop & other)
	: CUIObject(other)
{
}

void CShop::Set_ShopItemTexturePos(_vec3 * pPos)
{
	memcpy(pPos, m_vItemTexturePos, sizeof(m_vItemTexturePos));
}

void CShop::Set_ShopItemNamePos(_vec3 * pPos)
{
	memcpy(pPos, m_vItemNamePos, sizeof(m_vItemNamePos));
}

void CShop::Set_ShopItemPricePos(_vec3 * pPos)
{
	memcpy(pPos, m_vItemPricePos, sizeof(m_vItemPricePos));
}

HRESULT CShop::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CShop::Setup_GameObject(void * pArg)
{
	m_vShopItem.reserve(15);

	if (FAILED(Add_Component()))
		return E_FAIL;
	if (FAILED(Add_Component_ShopItem()))
		return E_FAIL;
	if (FAILED(Add_Component_ShopItemName()))
		return E_FAIL;
	if (FAILED(Add_Component_ShopItemPrice()))
		return E_FAIL;

	m_pTransformCom[SHOP_WND]->Set_Position(_vec3(400.f, 300.f, 0.f));
	m_pTransformCom[SHOP_SCROLLBAR]->Set_Position(_vec3(735.f, 160.f, 0.f));

	// 아이템 텍스처, 이름, 가격 위치 설정
	_uint iIndex = 0;
	for (_uint j = 0; j < 3; ++j)
	{
		for (_uint i = 0; i < 4; ++i)
		{
			iIndex = j * 4 + i;
			m_vItemTexturePos[j][i].x = (i * 135.f) + 250.f;
			m_vItemTexturePos[j][i].y = (j * 172.f) + 105.f;
			m_vItemTexturePos[j][i].z = 0.f;

			m_vItemNamePos[j][i].x = (i * 115.f) + 260.f;
			m_vItemNamePos[j][i].y = (j * 167.f) + 150.f;
			m_vItemNamePos[j][i].z = 0.f;

			m_vItemPricePos[j][i].x = (i * 115.f) + 280.f;
			m_vItemPricePos[j][i].y = (j * 167.f) + 180.f;
			m_vItemPricePos[j][i].z = 0.f;

			if (iIndex < 5)
			{
				m_pItemTransformCom[iIndex]->Set_Position(m_vItemTexturePos[j][i]);
				m_pItemNameTransformCom[iIndex]->Set_Position(m_vItemNamePos[j][i]);
				m_pItemPriceTransformCom[iIndex]->Set_Position(m_vItemPricePos[j][i]);
			}
			int k = 0;
		}
	}

	// 아이템 이미지 충돌 렉트
	for (_uint j = 0; j < 3; ++j)
	{
		for (_uint i = 0; i < 4; ++i)
		{
			iIndex = j * 4 + i;
			if (iIndex > 4)
				return S_OK;
			m_tItemTextureRt[j][i].left = (LONG)(m_vItemTexturePos[j][i].x - 20.f);
			m_tItemTextureRt[j][i].right = (LONG)(m_vItemTexturePos[j][i].x + 20.f);
			m_tItemTextureRt[j][i].top = (LONG)(m_vItemTexturePos[j][i].y - 20.f);
			m_tItemTextureRt[j][i].bottom = (LONG)(m_vItemTexturePos[j][i].y + 20.f);
		}
	}

	return S_OK;
}

int CShop::Update_GameObject(float DeltaTime)
{
	if ((CKeyManager::Get_Instance()->Key_Down('O')))
		m_bRender = !m_bRender;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (m_bRender)
	{
		if (FAILED(Check_BuyItem()))
			return GAMEOBJECT::WARN;
		if (FAILED(Move_ScrollBar()))
			return GAMEOBJECT::WARN;

		for (_uint i = 0; i < SHOP_END; ++i)
		{
			m_pTransformCom[i]->Update_Transform();
		}
		for (_uint i = 0; i < 5; ++i)
		{
			m_pItemTransformCom[i]->Update_Transform();
			m_pItemNameTransformCom[i]->Update_Transform();
			m_pItemPriceTransformCom[i]->Update_Transform();
		}
	}

	return GAMEOBJECT::NOEVENT;
}

int CShop::LateUpdate_GameObject(float DeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_UI, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CShop::Render_UI()
{
	if (m_bRender)
	{
		m_DC = GetDC(g_hWnd);

		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return E_FAIL;

		// 상점 윈도우 그리기
		for (_uint i = 0; i < SHOP_END; ++i)
		{
			const D3DXIMAGE_INFO* pTexInfo = m_pTextureCom[i]->Get_TexInfo(0);
			_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };
			_vec3 vPos = m_pTransformCom[i]->Get_Desc().vPosition;

			m_tShopWndCollRt[i].left = (LONG)(vPos.x - vCenter.x);
			m_tShopWndCollRt[i].right = (LONG)(vPos.x + vCenter.x);
			m_tShopWndCollRt[i].top = (LONG)(vPos.y - vCenter.y);
			m_tShopWndCollRt[i].bottom = (LONG)(vPos.y + vCenter.y);

			m_pSprite->SetTransform(&m_pTransformCom[i]->Get_Desc().matWorld);
			m_pSprite->Draw(
				(LPDIRECT3DTEXTURE9)m_pTextureCom[i]->GetTexture(0),
				nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		if (FAILED(Render_ShopItem()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CShop::Check_BuyItem()
{
	for (_uint j = m_iStartIndex; j < 3; ++j)
	{
		for (_uint i = 0; i < 4; ++i)
		{
			POINT ptMouse = {};
			GetCursorPos(&ptMouse);
			ScreenToClient(g_hWnd, &ptMouse);

			if (CKeyManager::Get_Instance()->Key_Pressing(VK_LBUTTON))
			{
				if (PtInRect(&m_tItemTextureRt[j][i], ptMouse))
					Buy_Item(j, i);
			}
		}
	}

	return S_OK;
}

HRESULT CShop::Buy_Item(_uint iIndexJ, _uint iIndexI)
{
	int iIndex = iIndexJ * 4 + iIndexI;
	wstring strName = m_vShopItem[iIndex]->szItemTag;

	CManagement* pManagement = CManagement::Get_Instance();
	CInventory* pInven = (CInventory*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Inventory");
	pInven->Get_ShopItem(strName);

	return S_OK;
}

HRESULT CShop::Move_ScrollBar()
{
	if (CKeyManager::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		POINT ptMouse = {};
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		_vec3 vScrollBarPos = m_pTransformCom[SHOP_SCROLLBAR]->Get_Desc().vPosition;

		if (PtInRect(&m_tShopWndCollRt[SHOP_SCROLLBAR], ptMouse))
		{
			vScrollBarPos.y = (_float)ptMouse.y;

			if (vScrollBarPos.y >= 470.f)
			{
				vScrollBarPos.y = 460.f;
				m_iStartIndex = 2;
			}
			else if (vScrollBarPos.y >= 360.f)
				m_iStartIndex = 2;
			else if (vScrollBarPos.y >= 260.f)
				m_iStartIndex = 1;
			else if (vScrollBarPos.y > 160.f)
				m_iStartIndex = 0;
			else if (vScrollBarPos.y <= 160.f)
			{
				vScrollBarPos.y = 165.f;
				m_iStartIndex = 0;
			}
		}

		m_pTransformCom[SHOP_SCROLLBAR]->Set_Position(vScrollBarPos);
	}
	return S_OK;
}

HRESULT CShop::Add_Component()
{
	for (_uint i = 0; i < SHOP_END; ++i)
	{
		// 2. Transform
		TCHAR szTransform[MAX_STR] = L"";
		wsprintf(szTransform, L"Com_Transform%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_Transform"
			, szTransform, (CComponent**)&m_pTransformCom[i])))
			return E_FAIL;

		// 3. Texture
		TCHAR szTexture[MAX_STR] = L"";
		TCHAR szTextureName[MAX_STR] = L"";
		if (i == SHOP_WND)
			wsprintf(szTextureName, L"Component_Texture_Shop_ShopWnd");
		else if (i == SHOP_SCROLLBAR)
			wsprintf(szTextureName, L"Component_Texture_Shop_ShopScrollBar");
		wsprintf(szTexture, L"Com_Texture%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, szTextureName,
			szTexture, (CComponent**)&m_pTextureCom[i])))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CShop::Add_Component_ShopItem()
{
	int iIndex = 0;

	for (_uint i = 0; i < 15; ++i)
	{
		if (i == 5)
			return S_OK;
		// Transform-----------------------------------------------------------------
		TCHAR szItemTransform[MAX_STR] = L"";
		wsprintf(szItemTransform, L"Com_ShopItemTransform%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_Transform",
			szItemTransform, (CComponent**)&m_pItemTransformCom[i])))
			return E_FAIL;

		// Texture-------------------------------------------------------------------
		TCHAR szItemTexture[MAX_STR] = L"";
		TCHAR szItemTextureName[MAX_STR] = L"";
		INVEN_ITEM* pItem = new INVEN_ITEM;
		if (i == 0)
		{
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"GoldenSword");
			wsprintf(szItemTextureName, L"Component_Texture_Item_GoldenSword");
			pItem->eSort = eITEM_SORT::STAFF1;
		}
		else if (i == 1)
		{
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"IronSword");
			wsprintf(szItemTextureName, L"Component_Texture_Item_IronSword");
			pItem->eSort = eITEM_SORT::STAFF1;
		}
		else if (i == 2)
		{
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"DiaSword");
			wsprintf(szItemTextureName, L"Component_Texture_Item_DiaSword");
			pItem->eSort = eITEM_SORT::STAFF1;
		}
		else if (i == 3)
		{
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"BlackDress");
			wsprintf(szItemTextureName, L"Component_Texture_Item_BlackDress");
			pItem->eSort = eITEM_SORT::STAFF1;
		}
		else if (i == 4)
		{
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"PupleDress");
			wsprintf(szItemTextureName, L"Component_Texture_Item_PupleDress");
			pItem->eSort = eITEM_SORT::STAFF1;
		}
		m_vShopItem.emplace_back(pItem);
		wsprintf(szItemTexture, L"Com_ShopItemTexture%d", i);

		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, szItemTextureName,
			szItemTexture, (CComponent**)&m_pItemTextureCom[i])))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CShop::Add_Component_ShopItemName()
{
	int iIndex = 0;

	for (_uint i = 0; i < 15; ++i)
	{
		if (i == 5)
			return S_OK;
		// Transform-----------------------------------------------------------------
		TCHAR szItemTransform[MAX_STR] = L"";
		wsprintf(szItemTransform, L"Com_ShopItemNameTransform%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_Transform",
			szItemTransform, (CComponent**)&m_pItemNameTransformCom[i])))
			return E_FAIL;

		// Texture-------------------------------------------------------------------
		TCHAR szItemTexture[MAX_STR] = L"";
		TCHAR szItemTextureName[MAX_STR] = L"";
		if (i == 0)
			wsprintf(szItemTextureName, L"Component_Texture_ItemName_GoldenSword");
		else if (i == 1)
			wsprintf(szItemTextureName, L"Component_Texture_ItemName_IronSword");
		else if (i == 2)
			wsprintf(szItemTextureName, L"Component_Texture_ItemName_DiaSword");
		else if (i == 3)
			wsprintf(szItemTextureName, L"Component_Texture_ItemName_BlackDress");
		else if (i == 4)
			wsprintf(szItemTextureName, L"Component_Texture_ItemName_PupleDress");
		wsprintf(szItemTexture, L"Com_ShopItemNameTexture%d", i);

		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, szItemTextureName,
			szItemTexture, (CComponent**)&m_pItemNameTextureCom[i])))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CShop::Add_Component_ShopItemPrice()
{
	int iIndex = 0;

	for (_uint i = 0; i < 15; ++i)
	{
		if (i == 5)
			return S_OK;
		// Transform-----------------------------------------------------------------
		TCHAR szItemTransform[MAX_PATH] = L"";
		StringCchPrintf(szItemTransform, sizeof(TCHAR) * MAX_PATH,
			L"Com_ShopItemPriceTransform%d", i);

		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_Transform",
			szItemTransform, (CComponent**)&m_pItemPriceTransformCom[i])))
			return E_FAIL;

		// Texture-------------------------------------------------------------------
		TCHAR szItemTexture[MAX_PATH] = L"";
		TCHAR szItemTextureName[MAX_PATH] = L"";
		if (i == 0)
			StringCchPrintf(szItemTextureName, sizeof(TCHAR) * MAX_PATH,
				L"Component_Texture_ItemPrice_GoldenSword");
		else if (i == 1)
			StringCchPrintf(szItemTextureName, sizeof(TCHAR) * MAX_PATH,
				L"Component_Texture_ItemPrice_IronSword");
		else if (i == 2)
			StringCchPrintf(szItemTextureName, sizeof(TCHAR) * MAX_PATH,
				L"Component_Texture_ItemPrice_DiaSword");
		else if (i == 3)
			StringCchPrintf(szItemTextureName, sizeof(TCHAR) * MAX_PATH,
				L"Component_Texture_ItemPrice_BlackDress");
		else if (i == 4)
			StringCchPrintf(szItemTextureName, sizeof(TCHAR) * MAX_PATH,
				L"Component_Texture_ItemPrice_PupleDress");

		StringCchPrintf(szItemTexture, sizeof(TCHAR) * MAX_PATH,
			L"Com_ShopItemPriceTexture%d", i);

		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, szItemTextureName,
			szItemTexture, (CComponent**)&m_pItemPriceTextureCom[i])))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CShop::Render_ShopItem()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	//m_vItemTexturePos[j][i].x = (i * 115.f) + 270.f;
	int iIndex = 0;
	for (_uint j = m_iStartIndex, k = 0; j < 3; ++j, ++k)
	{
		for (_uint i = 0; i < 4; ++i)
		{
			iIndex = j * 4 + i;
			if (iIndex >= 5)
				return S_OK;

			// 아이템 이미지--------------------------------------------------------------------------------------
			const D3DXIMAGE_INFO* pTexInfo = m_pItemTextureCom[iIndex]->Get_TexInfo(0);
			_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };
			//_vec3 vPos = m_pItemTransformCom[iIndex]->Get_Desc().vPosition;
			_vec3 vPos = m_vItemTexturePos[k][i];
			m_pItemTransformCom[iIndex]->Set_Position(vPos);

			m_tItemTextureRt[j][i].left = (LONG)(vPos.x - vCenter.x);
			m_tItemTextureRt[j][i].right = (LONG)(vPos.x + vCenter.x);
			m_tItemTextureRt[j][i].top = (LONG)(vPos.y - vCenter.y);
			m_tItemTextureRt[j][i].bottom = (LONG)(vPos.y + vCenter.y);

			m_pSprite->SetTransform(&m_pItemTransformCom[iIndex]->Get_Desc().matWorld);
			m_pSprite->Draw(
				(LPDIRECT3DTEXTURE9)m_pItemTextureCom[iIndex]->GetTexture(0),
				nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			// 아이템 이름
			pTexInfo = m_pItemNameTextureCom[iIndex]->Get_TexInfo(0);
			vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };
			//vPos = m_pItemNameTransformCom[iIndex]->Get_Desc().vPosition;
			vPos = m_vItemNamePos[k][i];
			m_pItemNameTransformCom[iIndex]->Set_Position(vPos);

			m_pSprite->SetTransform(&m_pItemNameTransformCom[iIndex]->Get_Desc().matWorld);
			m_pSprite->Draw(
				(LPDIRECT3DTEXTURE9)m_pItemNameTextureCom[iIndex]->GetTexture(0),
				nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			// 아이템 가격
			pTexInfo = m_pItemPriceTextureCom[iIndex]->Get_TexInfo(0);
			vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };
			//vPos = m_pItemPriceTransformCom[iIndex]->Get_Desc().vPosition;
			vPos = m_vItemPricePos[k][i];
			m_pItemPriceTransformCom[iIndex]->Set_Position(vPos);

			m_pSprite->SetTransform(&m_pItemPriceTransformCom[iIndex]->Get_Desc().matWorld);
			m_pSprite->Draw(
				(LPDIRECT3DTEXTURE9)m_pItemPriceTextureCom[iIndex]->GetTexture(0),
				nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
	int l = 0;
	return S_OK;
}

CShop * CShop::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == _pDevice)
		return nullptr;

	CShop* pInstance = new CShop(_pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CShop", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CShop::Clone_GameObject(void * pArg)
{
	CShop* pInstance = new CShop(*this);
	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CShop", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShop::Free()
{
	for (_uint i = 0; i < SHOP_END; ++i)
	{
		Safe_Release(m_pTransformCom[i]);
		Safe_Release(m_pTextureCom[i]);
	}
	

	for (_uint i = 0; i < 15; ++i)
	{
		if (i >= 5)
			break;
		Safe_Release(m_pItemTransformCom[i]);
		Safe_Release(m_pItemTextureCom[i]);

		Safe_Release(m_pItemNameTransformCom[i]);
		Safe_Release(m_pItemNameTextureCom[i]);

		Safe_Release(m_pItemPriceTransformCom[i]);
		Safe_Release(m_pItemPriceTextureCom[i]);
	}

	for (auto& pItem : m_vShopItem)
		Safe_Delete(pItem);
	m_vShopItem.clear();

	CUIObject::Free();
}
