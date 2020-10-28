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
	//void Equip_Item(INVEN_ITEM& _tItem);
	void Equip_Item(eITEM_SORT eSort, const wstring& strItemTag);

public:
	void Set_PlayerHp(_int iHP);
	void Set_PlayerMP(_int iMP);

public:
	_bool Get_Render() { return m_bRender; }

public:
	// CUIObject을(를) 통해 상속됨
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_UI() override;

private:
	HRESULT Render_EquipItem();
	HRESULT Render_Stat();

private:
	// ------------------------------------------------------------------
	// 스탯을 계산하는 함수
	// ------------------------------------------------------------------
	HRESULT Count_Stat();
	// ------------------------------------------------------------------
	// 아이템 장착을 해제하는 함수
	// ------------------------------------------------------------------
	HRESULT UnEquip_Item();

private:
	HRESULT Add_Component();

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual void Free() override;

private:
	_bool					m_bRender = false;

	// 현재 플레이어의 스탯
	CStatus*				m_pStatCom = nullptr;
	// 플레이어 스탯 폰트 위치
	//CTransform*				m_pTransformFont = nullptr;

	// 장비창 윈도우
	_vec3					m_vPos = _vec3(350.f, 300.f, 0.f);
	CTransform*				m_pTransformCom[EQUIP_END];
	CTexture*				m_pTextureCom[EQUIP_END];
	RECT					m_tEquipWndCollRt[EQUIP_END];

	// 착용하고 있는 장비 정보
	//vector<INVEN_ITEM*>		m_vEquipItem;
	CStatus*				m_pStatItem[ITEMSORT_END];
	CTexture*				m_pTextureItem[ITEMSORT_END];
	vector<CTransform*>		m_pTransformItem;
	RECT					m_tEquipItemCollRt[ITEMSORT_END] = {};
	_bool					m_bEquip[ITEMSORT_END] = { false, };
};


END
#endif
