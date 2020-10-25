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

// Setter===================================================================================
public:
	// ------------------------------------------------------------------
	// �κ��� �������� �׸������� �ȱ׸������� �ܺο��� �������ִ� �Լ�
	// bool bRender : true�� �׸���, false�� �׸��� �ʴ´�
	// ------------------------------------------------------------------
	void Set_Render(bool bRender);


// Getter===================================================================================
public:
	// ------------------------------------------------------------------
	// �κ��� �������� �׸������� �ȱ׸������� �ܺο��� �˷��ִ� �Լ�
	// m_bRender : true�� �׸���, false�� �׸��� �ʴ´�
	// ------------------------------------------------------------------
	bool Get_Render() { return m_bRender; }


public:
	// ------------------------------------------------------------------
	// �������� ������ ������ ���� / �κ��� �߰��ϴ� �Լ�
	// const wstring& strItemName : ������ ������ �̸�
	// ------------------------------------------------------------------
	HRESULT Get_ShopItem(const wstring& strItemName);

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * pArg) override;
	virtual _int Update_GameObject(float DeltaTime) override;
	virtual _int LateUpdate_GameObject(float DeltaTime) override;
	virtual HRESULT Render_UI() override;


// �Ǹ� ���� �Լ�
private:
	// ------------------------------------------------------------------
	// �Ǹ� ��ư�� �������� Ȯ���ϴ� �Լ�
	// -> �������� m_bSelect_SellItem = true, �ƴϸ� false
	// ------------------------------------------------------------------
	HRESULT Check_SellButton();

	// ------------------------------------------------------------------
	// �Ǹ� ������ ��, �Ǹ��� �������� �����ϴ� �Լ�
	// ------------------------------------------------------------------
	HRESULT Select_SellItem();


// ���� ���� �Լ�
private:
	// ------------------------------------------------------------------
	// �ڵ� ���� ��ư�� �������� Ȯ���ϴ� �Լ�
	// ------------------------------------------------------------------
	HRESULT Check_AutoSortButton();


// ������ ���� ���� �Լ�
private:
	// ------------------------------------------------------------------
	// �������� ������ ������ Ȯ���ϴ� �Լ�
	// ------------------------------------------------------------------
	HRESULT Check_EquipItem();


// Render() ���� �Լ�
private:
	// ------------------------------------------------------------------
	// �������� �׸��� �Լ�
	// ------------------------------------------------------------------
	HRESULT Render_Item();

private:
	HRESULT Move_InventoryWnd();
	HRESULT Change_AllPos();
	
private:
	HRESULT Add_Component();
	HRESULT Add_Component_Item();

public:
	static CInventory* Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	_int				m_iNewInsertOrder = 0;
	_int				m_iSellGold = 0;
	// ���� ������ �ִ� ���
	_int				m_iGold = 10000;
	// true : �κ��� �׸��� / false : �κ��� �׸��� �ʴ´�
	bool				m_bRender = false;
	// �κ� ������ ��ġ
	_vec3 vWndPos =		{ 500.f, 200.f, 0.f };

// ������--------------------------------------------------------------------------------
private:
	// �����۵��� ����Ʈ�� ����
	//list<INVEN_ITEM*>	m_pInvenList;
	vector<INVEN_ITEM*>	m_pInvenList;
	// �κ��� �ִ� �����۵�
	vector<CTexture*>	m_pTextureItem;
	vector<CTransform*>	m_pTransformItem;

	// �κ��� �ִ� �����۵� ��ġ
	_vec3				m_vItemPos[6][6];
	// �κ��� �ִ� �����۵� �浹 RECT
	RECT				m_tItemCollRt[6][6];
	// �Ǹ��� ������
	CTexture*			m_pTextureSell = nullptr;

// �κ�â--------------------------------------------------------------------------------
private:
	CTransform*			m_pTransformCom[INVEN_END];
	CTexture*			m_pTextureCom[INVEN_END - 1];
	// �κ�â ��ġ, �浹 RECT
	_vec3				m_vInvenWndPos[INVEN_END];
	RECT				m_tInvenWndCollRt[INVEN_END];

private:
	// m_bRenderEmptySell�� true �� ��,
	// �� �ؽ�ó�� �׸� ����, �ؽ�ó
	CTexture*			m_pTextureEmpty = nullptr;


private:
	// �������� �κ��� ���ԵǴ� ����
	// == �κ��� �ִ� �����۵� ���� - 1
	_int				m_iInsertOrder = 0;

	// �Ǹ� �������� �����ϴ��� = true, �� �� = false
	_bool				m_bSelect_SellItem = false;

	// ���� ĭ�� �������� ���ԵǾ� �ִ���
	_bool				m_bIsItemHere[36] = { false };

	// ���� �������� �Ǹ� ���������� ���õǾ� ���� = true
	_bool				m_bSelectedSell[36] = { false };

	// �ǸŹ�ư�� ������ ������ ������ �ʾ��� ��
	// -> true�� ���� �� �ؽ�ó�� �ٲ��ְ�
	// -> ���� ���� ��ư�� ������ �� ��, ����Ʈ������ �������ش�
	_bool				m_bRenderEmptySell = false;

	// �����ߴ���
	_bool				m_bAutoSort = false;

};

END

#endif // !__INVENTORY_H__
