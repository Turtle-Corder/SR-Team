#pragma once
#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include "UIObject.h"

USING(Engine)
BEGIN(Client)

class CItemManager;
class CInventory : public CUIObject
{
private:
	explicit CInventory(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CInventory(const CInventory& _rOther);
	virtual ~CInventory() = default;

public:
	void Set_Render(bool bRender);
	void Set_InvenItemList(list<INVEN_ITEM*>* pItemList);

public:
	bool Get_Render() { return m_bRender; }
	const list<INVEN_ITEM*>& Get_InvenList() { return m_pInvenList; }

public:
	// ------------------------------------------------------------------
	// 상점에서 선택한 아이템 구매 / 인벤에 추가하는 함수
	// const wstring& strItemName : 구매한 아이템 이름
	// ------------------------------------------------------------------
	HRESULT Get_ShopItem(const wstring& strItemName);

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * pArg) override;
	virtual _int Update_GameObject(float DeltaTime) override;
	virtual _int LateUpdate_GameObject(float DeltaTime) override;
	virtual HRESULT Render_UI() override;

// 판매 관련 함수
private:
	// ------------------------------------------------------------------
	// 판매 버튼을 눌렀는지 확인하는 함수
	// -> 눌렀으면 m_bSelect_SellItem = true, 아니면 false
	// ------------------------------------------------------------------
	HRESULT Check_SellButton();

	// ------------------------------------------------------------------
	// 판매 상태일 때, 판매할 아이템을 선택하는 함수
	// ------------------------------------------------------------------
	HRESULT Select_SellItem();

	HRESULT Sell_Item();


// 정렬 관련 함수
private:
	// ------------------------------------------------------------------
	// 자동 정렬 버튼을 눌렀는지 확인하는 함수
	// ------------------------------------------------------------------
	HRESULT Check_AutoSortButton();


// Render() 관련 함수
private:
	// ------------------------------------------------------------------
	// 아이템을 그리는 함수
	// ------------------------------------------------------------------
	HRESULT Render_Item();
	
private:
	HRESULT Add_Component();
	HRESULT Add_Component_Item();

public:
	static CInventory* Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	// true : 인벤을 그린다 / false : 인벤을 그리지 않는다
	bool				m_bRender = false;


	// 아이템들을 리스트로 관리
	list<INVEN_ITEM*>	m_pInvenList;
	// 인벤에 있는 아이템들
	vector<CTexture*>	m_pTextureItem;
	vector<CTransform*>	m_pTransformItem;
	// 인벤에 있는 아이템들 위치
	_vec3				m_vItemPos[6][6];
	// 인벤에 있는 아이템들 충돌 RECT
	RECT				m_tItemCollRt[6][6];
	// 판매할 아이템
	CTexture*			m_pTextureSell = nullptr;

	// 인벤창
	CTransform*			m_pTransformCom[INVEN_END];
	CTexture*			m_pTextureCom[INVEN_END];
	// 인벤창 위치
	_vec3				m_vInvenWndPos[INVEN_END];
	RECT				m_tInvenWndCollRt[INVEN_END];


	// m_bRenderEmptySell가 true 일 때,
	// 빈 텍스처를 그릴 버퍼, 텍스처
	CTexture*			m_pTextureEmpty = nullptr;


	// 아이템이 인벤에 삽입되는 순서
	// == 인벤에 있는 아이템들 개수 - 1
	_int				m_iInsertOrder = 0;
	// 판매 버튼 충돌 RECT
	RECT				m_tSellButtonRt = {};


	// 판매 아이템을 선택하는중 = true, 그 외 = false
	_bool				m_bSelect_SellItem = false;

	// 현재 칸에 아이템이 삽입되어 있는지
	_bool				m_bIsItemHere[36] = { false };

	// 현재 아이템이 판매 아이템으로 선택되어 있음 = true
	_bool				m_bSelectedSell[36] = { false };

	// 판매버튼을 누르고 정렬을 누르지 않았을 때
	// -> true일 때만 빈 텍스처로 바꿔주고
	// -> 이후 정렬 버튼을 누르면 이 때, 리스트에서도 삭제해준다
	_bool				m_bRenderEmptySell = false;

	// 정렬했는지
	_bool				m_bAutoSort = false;

};

END

#endif // !__INVENTORY_H__
