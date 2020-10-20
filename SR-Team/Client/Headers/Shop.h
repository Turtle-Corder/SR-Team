#pragma once
#ifndef __SHOP_H__
#define __SHOP_H__

#include "GameObject.h"
USING(Engine)
BEGIN(Client)

class CShop final : public CGameObject
{
private:
	explicit CShop(LPDIRECT3DDEVICE9 pDevice);
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

private:
	HRESULT Add_Component();

public:
	static CShop* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;



private:
	HDC				m_DC;
	bool			m_bRender = false;
	RECT			m_tShopWnd;
	// Component-----------------------------------
	CVIBuffer*		m_pVIBufferCom;
	CTransform*		m_pTransformCom;
	CTexture*		m_pTextureCom;

	// Pos, Scale----------------------------------
	_vec3			m_vPos[SHOP_END];
	_vec3			m_vScale[SHOP_END];

	// 상점 윈도우 내 아이템 위치
	_vec3			m_vItemTexturePos[4][3];
	_vec3			m_vItemNamePos[4][3];
	_vec3			m_vItemPricePos[4][3];

	// 구입하기 버튼 위치, RECT
	_vec3			m_vBuyPos[4][3];
	RECT			m_tBuyRect[4][3];


};

END

#endif // !__SHOP_H__
