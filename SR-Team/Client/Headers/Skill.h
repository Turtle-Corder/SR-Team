#pragma once

#ifndef __SKILL_H__
#define __SKILL_H__

#include "UIObject.h"
USING(Engine)
BEGIN(Client)

class CSkill final : public CUIObject
{
public:
	enum SKILL_SORT
	{
		ACTIVE, PASSIVE, SKILL_SORT_END
	};

public:
	explicit CSkill(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CSkill(const CSkill& other);
	virtual ~CSkill() = default;

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
	// ------------------------------------------------------------------
	// ��Ƽ��/�нú� ��ų �� �ϳ��� �����ϴ� �Լ�
	// ------------------------------------------------------------------
	HRESULT Set_SkillSort();
	// ------------------------------------------------------------------
	// ��Ƽ�� ��ų�� ������ ���� �����ִ� �Լ�
	// ------------------------------------------------------------------
	HRESULT Show_ActiveSkill_Info();
	// ------------------------------------------------------------------
	// ��ų �������� ���������� �ű�� �Լ�
	// ------------------------------------------------------------------
	HRESULT Move_To_QuickSlot();

private:
	HRESULT Add_Component();

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual void Free() override;
	
private:
	_bool					m_bRender = false;

// ��ų ������
private:
	// ��ų ������ �߽� ��ġ
	_vec3					m_vCenterPos = { 500.f, 300.f, 0.f };
	CTransform*				m_pTransformWnd[SKILL_END];
	CTexture*				m_pTextureWnd[SKILL_END];
	RECT					m_tWndCollRt[SKILL_END];

// �нú�/��Ƽ�� ��ų ����
private:
	SKILL_SORT				m_eSkillSort = ACTIVE;	// ��ų ����
	RECT					m_tSkillSortCollRt[2];	// ��Ƽ��/�нú� ��ư �浹 ��Ʈ

private:
	RECT					m_tActiveCollRt[6];

private:
	CTexture*				m_pTextureSkillInfo[6] = { nullptr, };
	_int					m_iSkillInfoIndex = -1;
	_bool					m_bRenderSkillInfo = false;
	_vec3					m_vSkillInfoPos = { 0.f, 0.f, 0.f };
};

END
#endif