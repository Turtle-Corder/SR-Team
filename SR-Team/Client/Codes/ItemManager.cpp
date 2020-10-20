#include "stdafx.h"
#include "..\Headers\ItemManager.h"

USING(Client)

CItemManager::CItemManager(LPDIRECT3DDEVICE9 pDevice)
	: CComponent(pDevice)
{
}

CItemManager::CItemManager(const CItemManager & other)
	: CComponent(other)
{
}

list<INVEN_ITEM*> CItemManager::Get_InvenList()
{ 
	return m_pInvenList; 
}

list<INVEN_ITEM*> CItemManager::Get_ShopList()
{ 
	return m_pShopList; 
}

list<INVEN_ITEM*>& CItemManager::Get_DropItem(wstring wstrItemName, int iSort, int iPrice)
{
	auto& iter_begin = m_pInvenList.begin();

	for (auto& pItem : m_pInvenList)
	{
		// 만약 이미 존재하는 아이템이라면 개수만 증가시킨다
		if (pItem->szItemTag == wstrItemName)
		{
			++pItem->iCnt;
			return m_pInvenList;
		}
	}

	// 아니면 새로 만들어서 리스트에 넣어준다
	INVEN_ITEM* pItem = new INVEN_ITEM;
	StringCchPrintf(pItem->szItemTag, _countof(pItem->szItemTag), wstrItemName.c_str());
	pItem->iSort = iSort;
	pItem->iPrice = iPrice;
	pItem->iInvneInsertOrder = m_iInvenInsertOrder;

	m_pInvenList.emplace_back(pItem);
	++m_iInvenInsertOrder;

	return m_pInvenList;
}

HRESULT CItemManager::Setup_Component_Prototype()
{
	return S_OK;
}

HRESULT CItemManager::Setup_Component(void * pArg)
{
	return S_OK;
}

CItemManager * CItemManager::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (pDevice == nullptr)
		return nullptr;

	CItemManager* pInstance = new CItemManager(pDevice);
	if (FAILED(pInstance->Setup_Component_Prototype()))
	{
		PRINT_LOG(L"Failed To Creating CItemManagement", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CItemManager::Clone_Component(void * pArg)
{
	CItemManager* pClone = new CItemManager(*this);
	if (FAILED(pClone->Setup_Component(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CItemManagement", LOG::CLIENT);
		Safe_Release(pClone);
	}

	return pClone;
}

void CItemManager::Free()
{
	m_pInvenList.clear();

	CComponent::Free();
}
