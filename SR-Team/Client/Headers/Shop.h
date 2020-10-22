#pragma once
#ifndef __SHOP_H__
#define __SHOP_H__

#include "UIObject.h"
USING(Engine)
BEGIN(Client)

class CShop final : public CUIObject
{
private:
	explicit CShop(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CShop(const CShop& other);
	virtual ~CShop() = default;

public:
	void Set_Render(bool bRender) { m_bRender = bRender; }
	void Set_ShopItemTexturePos(_vec3* pPos);
	void Set_ShopItemNamePos(_vec3* pPos);
	void Set_ShopItemPricePos(_vec3* pPos);

public:
	bool Get_Render() { return m_bRender; }

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * pArg) override;
	virtual int Update_GameObject(float DeltaTime) override;
	virtual int LateUpdate_GameObject(float DeltaTime) override;
	virtual HRESULT Render_UI() override;

// ������ ���� ���� �Լ���
private:
	HRESULT Check_BuyItem();
	HRESULT Buy_Item(_uint iIndexJ, _uint iIndexI);

private:
	// ���� ��ũ�� ���� �̵�
	HRESULT Move_ScrollBar();

private:
	HRESULT Add_Component();
	HRESULT Add_Component_ShopItem();
	HRESULT Add_Component_ShopItemName();
	HRESULT Add_Component_ShopItemPrice();

private:
	HRESULT Render_ShopItem();

public:
	static CShop* Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	HDC				m_DC;
	bool			m_bRender = false;
	RECT			m_tShopWnd;
	// Component-----------------------------------
	// ���� ������
	CTransform*		m_pTransformCom[SHOP_END];
	CTexture*		m_pTextureCom[SHOP_END];
	RECT			m_tShopWndCollRt[SHOP_END];

	// ���� ������
	vector<INVEN_ITEM*>	m_vShopItem;

	// ���� ������ �̹���
	CTransform*		m_pItemTransformCom[15];
	CTexture*		m_pItemTextureCom[15];

	// ���� ������ �̸�
	CTransform*		m_pItemNameTransformCom[15];
	CTexture*		m_pItemNameTextureCom[15];

	// ���� ������ ����
	CTransform*		m_pItemPriceTransformCom[15];
	CTexture*		m_pItemPriceTextureCom[15];

	// Pos, Scale----------------------------------
	_vec3			m_vPos[SHOP_END];
	_vec3			m_vScale[SHOP_END];

	// ���� ������ �� ������ ��ġ
	_vec3			m_vItemTexturePos[3][4];
	_vec3			m_vItemNamePos[3][4];
	_vec3			m_vItemPricePos[3][4];

	// ������ �ϴ� �̹����� �浹�ϴµ� 
	// ���߿� �����ϱ� ��ư rect�� �浹üũ�ϱ�
	RECT			m_tItemTextureRt[3][4];

	// �����ϱ� ��ư ��ġ, RECT
	_vec3			m_vBuyPos[4][3];
	RECT			m_tBuyRect[4][3];

	// ��ũ�ѹ� �浹 RECT
	RECT			m_tScrollBarRt = {};
	int				m_iStartIndex = 0;


};

END

#endif // !__SHOP_H__
