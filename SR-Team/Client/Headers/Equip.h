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
	// ------------------------------------------------------------------
	// �������� �����ϴ� �Լ�
	// eITEM_SORT eSort : ������ ������ ����
	// const wstring& strItemTag : ������ ������Tag
	// ------------------------------------------------------------------
	void Equip_Item(eITEM_SORT eSort, const wstring& strItemTag);
	// ------------------------------------------------------------------
	// �������� �����ϰ� �ִ��� �˻��ϴ� �Լ�
	// const wstring& strItemTag : �˻��� ������Tag
	// return true : ���� �����ϰ� �ִ�
	// return false : �����ϰ� ���� �ʴ�
	// ------------------------------------------------------------------
	_bool Check_IsItemEquip(const wstring& strItemTag);

public:
	CStatus::STAT Get_PlayerStat() { return m_pStatCom->Get_Status(); }

public:
	void Set_PlayerAtt(_int iAtt);
	void Set_PlayerHp(_int iHP);
	void Set_PlayerMP(_int iMP);

public:
	_bool Get_Render() { return m_bRender; }

public:
	// CUIObject��(��) ���� ��ӵ�
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
	// ������ ����ϴ� �Լ�
	// ------------------------------------------------------------------
	HRESULT Count_Stat();
	// ------------------------------------------------------------------
	// ������ ������ �����ϴ� �Լ�
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

	// ���� �÷��̾��� ����
	CStatus*				m_pStatCom = nullptr;
	// �÷��̾� ���� ��Ʈ ��ġ
	//CTransform*				m_pTransformFont = nullptr;

	// ���â ������
	_vec3					m_vPos = { 500.f, 400.f, 0.f };
	CTransform*				m_pTransformCom[EQUIP_END];
	CTexture*				m_pTextureCom[EQUIP_END];
	RECT					m_tEquipWndCollRt[EQUIP_END];

	// �����ϰ� �ִ� ��� ����
	CStatus*				m_pStatItem[ITEMSORT_END];
	CTexture*				m_pTextureItem[ITEMSORT_END];
	vector<CTransform*>		m_pTransformItem;
	RECT					m_tEquipItemCollRt[ITEMSORT_END] = {};
	_bool					m_bEquip[ITEMSORT_END] = { false, };
	// ���� �����ϰ� �ִ� ������Tag
	TCHAR					szEquipItemTag[ITEMSORT_END][MAX_PATH] = { L"", };
};


END
#endif
