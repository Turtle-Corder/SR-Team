#pragma once
#ifndef __ITEM_H__
#define __ITEM_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CStatus;
class CItemManager;
class CItem : public CGameObject
{
private:
	explicit CItem(LPDIRECT3DDEVICE9 _pDevice);
	explicit CItem(const CItem& _rOther);
	virtual ~CItem() = default;


public:
	void Set_RenderShopItem(bool bRender); 
	void Set_RenderInvenItem(bool bRender); 

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * pArg) override;
	virtual int Update_GameObject(float DeltaTime) override;
	virtual int LateUpdate_GameObject(float DeltaTime) override;
	virtual HRESULT Render_UI() override;


private:
	HRESULT Add_Component();
	HRESULT Add_Component_Item();
	HRESULT Add_Component_ItemName();
	HRESULT Add_Component_ItemPrice();

private:
	HRESULT Render_ShopItem();
	HRESULT Render_InvenItem();

public:
	static CItem* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;



private:
	bool			m_bRenderShopItem = false;
	bool			m_bRenderInvenItem = false;

	// Component-----------------------------------
	CItemManager*	m_pItmeMgrCom = nullptr;

	// 아이템 이미지(상점), 지금은 인벤도 같이 있는데 나중에 분리하기
	CVIBuffer*		m_pVIBufferCom[5];
	CTransform*		m_pTransformCom[5];
	CTexture*		m_pTextureCom[5];
	CStatus*		m_pStatCom[5];

	// 아이템 이름(상점)
	CVIBuffer*		m_pNameVIBufferCom[5];
	CTransform*		m_pNameTransformCom[5];
	CTexture*		m_pNameTextureCom[5];

	// 아이템 가격(상점)
	CVIBuffer*		m_pPriceVIBufferCom[5];
	CTransform*		m_pPriceTransformCom[5];
	CTexture*		m_pPriceTextureCom[5];

	CVIBuffer*		m_pVIBufferShop[4][3];
	CTransform*		m_pTransformShop[4][3];
	CTexture*		m_pTextureShop[4][3];
	CStatus*		m_pStatShop[4][3];
};

END

#endif // !__ITEM_H__
