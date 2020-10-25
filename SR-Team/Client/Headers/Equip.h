#pragma once

#ifndef __EQUIP_H__
#define __EQUIP_H__

#include "UIObject.h"
USING(Engine)
BEGIN(Client)

class CEquip final : public CUIObject
{
public:
	explicit CEquip(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CEquip(const CEquip& other);
	virtual ~CEquip() = default;


public:
	void Equip_Item(INVEN_ITEM& _tItem);


public:
	// CUIObject을(를) 통해 상속됨
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_UI() override;

private:
	HRESULT Render_EquipItem();

private:
	HRESULT Add_Component();

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual void Free() override;

private:
	_bool			m_bRender = false;
	
	// 장비창 윈도우
	CTransform*				m_pTransformCom[EQUIP_END];
	CTexture*				m_pTextureCom[EQUIP_END];
	RECT					m_tEquipWndCollRt[EQUIP_END];

	// 착용하고 있는 장비 정보
	vector<INVEN_ITEM*>		m_vEquipItem;
	CStatus*				m_pStatItem[ITEMSORT_END];
	CTexture*				m_pTextureItem[ITEMSORT_END];
	CTransform*				m_pTransformItem[ITEMSORT_END];
	bool					m_bEquip[ITEMSORT_END] = { false, };
};


END
#endif
