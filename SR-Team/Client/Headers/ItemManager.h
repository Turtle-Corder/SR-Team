#pragma once
#ifndef __ITEM_MANAGER_H__
#define __ITEM_MANAGER_H__

#include "Component.h"

USING(Engine)
BEGIN(Client)

class CItemManager final : public CComponent
{
private:
	explicit CItemManager(LPDIRECT3DDEVICE9 pDevice);
	explicit CItemManager(const CItemManager& other);
	virtual ~CItemManager() = default;

public:
	list<INVEN_ITEM*> Get_InvenList();
	list<INVEN_ITEM*> Get_ShopList();

public:
	list<INVEN_ITEM*>& Get_DropItem(wstring wstrItemName, int iSort, int iPrice);

public:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Setup_Component_Prototype() override;
	virtual HRESULT Setup_Component(void * pArg) override;

public:
	static CItemManager* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent * Clone_Component(void * pArg) override;
	virtual void Free() override;
	


private:
	list<INVEN_ITEM*>		m_pInvenList;
	// 인벤 삽입 순서
	int				m_iInvenInsertOrder = 0;

	list<INVEN_ITEM*>		m_pShopList;

};

END

#endif // !__ITEM_MANAGER_H__
