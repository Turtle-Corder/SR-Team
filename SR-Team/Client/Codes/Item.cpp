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
		m_pTextureCom[i] = nullptr;
	}
}

CItem::CItem(const CItem & _rOther)
	: CGameObject(_rOther)
{
}

CTexture * CItem::Get_ItemInfo_Texture(const wstring & strItemTag)
{
	_int iIndex = 0;
	for (auto& pItem : m_vItemList)
	{
		if (!wcscmp(pItem->szItemTag, strItemTag.c_str()))
			return m_pTextureCom[iIndex];
		++iIndex;
	}

	return nullptr;
}

const _int CItem::Get_ItemInfo_Price(const wstring & strItemTag)
{
	_int iIndex = 0;
	for (auto& pItem : m_vItemList)
	{
		if (!wcscmp(pItem->szItemTag, strItemTag.c_str()))
			return pItem->iPrice;
		++iIndex;
	}

	return -1;
}

HRESULT CItem::Get_ItemInfo(const wstring & strItemTag, INVEN_ITEM & tItem)
{
	_int iIndex = 0;
	for (auto& pItem : m_vItemList)
	{
		if (!wcscmp(pItem->szItemTag, strItemTag.c_str()))
		{
			memcpy_s(&tItem, sizeof(INVEN_ITEM), pItem, sizeof(INVEN_ITEM));
			return S_OK;
		}
		++iIndex;
	}

	return E_FAIL;
}



HRESULT CItem::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CItem::Setup_GameObject(void * pArg)
{
	if (Add_Component())
		return E_FAIL;
	if (Add_Component_Item())
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
	return S_OK;
}

HRESULT CItem::Add_Component()
{
	//if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_ItemManagement", L"Com_ItemMgr", (CComponent**)&m_pItmeMgrCom)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CItem::Add_Component_Item()
{
	for (_uint i = 0; i < 5; ++i)
	{
		// 3. Texture--------------------------------------------------------------
		TCHAR szTexture[MAX_PATH] = L"";
		TCHAR szTextureName[MAX_PATH] = L"";
		if (i == 0)
			StringCchPrintf(szTextureName, sizeof(TCHAR) * MAX_PATH,
				L"Component_Texture_Item_GoldenSword");
		else if (i == 1)
			StringCchPrintf(szTextureName, sizeof(TCHAR) * MAX_PATH,
				L"Component_Texture_Item_IronSword");
		else if (i == 2)
			StringCchPrintf(szTextureName, sizeof(TCHAR) * MAX_PATH,
				L"Component_Texture_Item_DiaSword");
		else if (i == 3)
			StringCchPrintf(szTextureName, sizeof(TCHAR) * MAX_PATH,
				L"Component_Texture_Item_BlackDress");
		else if (i == 4)
			StringCchPrintf(szTextureName, sizeof(TCHAR) * MAX_PATH,
				L"Component_Texture_Item_PupleDress");
		
		StringCchPrintf(szTexture, sizeof(TCHAR) * MAX_PATH,
			L"Com_Texture%d", i);

		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, szTextureName,
			szTexture, (CComponent**)&m_pTextureCom[i])))
			return E_FAIL;

		INVEN_ITEM* pItem = new INVEN_ITEM;
		if (i == 0)
		{
			pItem->eSort = eITEM_SORT::STAFF;
			pItem->iPrice = 200;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"GoldenSword");
		}
		if (i == 1)
		{
			pItem->eSort = eITEM_SORT::STAFF;
			pItem->iPrice = 100;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"IronSword");
		}
		if (i == 2)
		{
			pItem->iPrice = 300;
			pItem->eSort = eITEM_SORT::STAFF;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"DiaSword");
		}
		if (i == 3)
		{
			pItem->iPrice = 150;
			pItem->eSort = eITEM_SORT::SUIT;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"BlackDress");
		}
		if (i == 4)
		{
			pItem->iPrice = 200;
			pItem->eSort = eITEM_SORT::SUIT;
			swprintf(pItem->szItemTag, sizeof(pItem->szItemTag) / sizeof(TCHAR),
				L"%s", L"PupleDress");
		}
		m_vItemList.emplace_back(pItem);
	}

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
	for (auto& pItem : m_vItemList)
	{
		Safe_Delete(pItem);
	}
	m_vItemList.clear();

	for (_uint i = 0; i < 5; ++i)
	{
		Safe_Release(m_pTextureCom[i]);
	}

	CGameObject::Free();
}
