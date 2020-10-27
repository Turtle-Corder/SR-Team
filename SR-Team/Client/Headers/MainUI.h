#pragma once
#ifndef __MAINUI_H__
#define __MAINUI_H__

#include "UIObject.h"

USING(Engine)
BEGIN(Client)

class CMainUI : public CUIObject
{
private:
	explicit CMainUI(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CMainUI(const CMainUI& _rOther);
	virtual ~CMainUI() = default;


public:
	// ------------------------------------------------------------------
	// ���� ���������� �̵��ϰ� �ִ� �������� ������ �޾ƿ��� �Լ�
	// INVEN_ITEM* pItem : ���� ���������� �̵��ϰ� �ִ� ������
	// ------------------------------------------------------------------
	HRESULT Get_QuickSlotItem(INVEN_ITEM* pItem);
	// ------------------------------------------------------------------
	// �κ����� �Ǹŵ� ������ �޾ƿͼ� 
	// ���� �����Կ� �ִ��� Ȯ���ϰ�, ������ ������ 0���� ��������ִ� �Լ�
	// INVEN_ITEM* pItem : �κ����� �Ǹŵ� ����
	// ------------------------------------------------------------------
	HRESULT Delete_Potion(INVEN_ITEM* pItem);


private:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * pArg) override;
	virtual int Update_GameObject(float DeltaTime) override;
	virtual int LateUpdate_GameObject(float DeltaTime) override;
	virtual HRESULT Render_UI() override;

	
// ������ ���� �Լ�--------------------------------------------------
private:
	// �������� ���������� �̵���Ű�� �Լ�
	HRESULT Move_To_QuickSlot();
	// ����/������ �����԰� �������� �浹�ߴ��� �˻��ϴ� �Լ�
	// -> �浹������ ���� ���Կ� �������� �ִ��� Ȯ���ϰ� ������ ����
	HRESULT Check_LeftQuickSlot_Item();
	HRESULT Check_RightQuickSlot_Item();
	// ���� ���Կ� �������� �ִ��� �˻�
	// -> ���� : false / ���� : true ��ȯ
	_bool Check_Item_In_Slot();
	// �κ����� �������� ������ �޾ƿ��� �Լ�
	// -> ������ ������ 0�̸� ���X
	HRESULT Set_SlotItem_Count();


private:
	// ���������� �̵����� �������� �׸��� �Լ�
	HRESULT Render_Item_GoingToQuickSlot();
	// �����Կ� �ִ� �������� �׸��� �Լ�
	HRESULT Render_QuickSlot_Item();

private:
	HRESULT Add_Component();

public:
	static CMainUI* Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	// Component-------------------------------------------------------------
	CVIBuffer*		m_pVIBufferCom[MAINUI_END];
	CTransform*		m_pTransformCom[MAINUI_END];
	CTexture*		m_pTextureCom[MAINUI_END];

	// ��ġ, ũ��-------------------------------------------------------------
	_vec3			m_vPos[MAINUI_END];
	_vec3			m_vScale[MAINUI_END];

	// �浹 ��Ʈ
	RECT			m_tCollRt[MAINUI_END];


// ������==================================================================
private:
	// ���� �̵���Ű�� �ִ� ������ ����
	INVEN_ITEM*		m_pMovingItem = nullptr;

	// ����, ������ ������ �浹 ��Ʈ
	RECT			m_tLeftSlotCollRt[8];
	RECT			m_tRightSlotCollRt[8];

	// ����, ������ ������ ���� ��ġ
	CTransform*		m_pTransformLeftSlot[8];
	CTransform*		m_pTransformRightSlot[8];

	// ����, ������ ū ������ ��ġ
	_vec3			m_vLeftSlotPos = { 170.f, 500.f, 0.f };
	_vec3			m_vRightSlotPos = { 660.f, 500.f, 0.f };

	CTexture*		m_pEmptyTexture = nullptr;

	// ���������� ���� ������ ��ġ, �浹 ��Ʈ
	_vec3			m_vGoingItem_Pos = { 0.f, 0.f, 0.f };
	RECT			m_tGoingItem_CollRt = { 0, 0, 0, 0 };

	// ���������� ���� ������ �׸��� ����
	_bool			m_bRender_GoingItem = false;
	// ���������� ���� ������ �׸� �ؽ�ó
	CTexture*		m_pTexture_GoingItem = nullptr;

	// �����Կ� �׸� �̹���
	CTexture*		m_pTextureLeftQuickSlot[8];
	CTexture*		m_pTextureRightQuickSlot[8];

	// �������� ������ �ִ� ������ ����
	INVEN_ITEM*		m_pLeftSlotItem[8];
	INVEN_ITEM*		m_pRightSlotItem[8];
};
END

#endif // !__MAINUI_H__
