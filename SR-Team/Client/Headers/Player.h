#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "GameObject.h"

BEGIN(Client)
USING(Engine)

class CStatus;
class CItemManager;
class CPlayer final : public CGameObject
{
public:
	enum CHANGE_MOVE { CHANGE_LEFT, CHANGE_RIGHT, MOVE_END };
	enum MOVEING_DIR { MOVING_UP, MOVING_DOWN, MOVING_LEFT, MOVING_RIGHT };

private:
	explicit CPlayer(LPDIRECT3DDEVICE9 _pDevice);
	explicit CPlayer(const CPlayer& _rOther);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void* _pArg) override;

	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;

	virtual HRESULT Take_Damage() override;

private:
	HRESULT Add_Component();

	HRESULT IsOnTerrain();
	HRESULT Movement(_float _fDeltaTime);
	void Move_Vertical(_float _fDeltaTime);
	void Move_Horizontal(_float _fDeltaTime);
	void Turn(_float _fDeltaTime);

	HRESULT RaycastOnTerrain();
	void Move_Target(_float _fDeltaTime);

	virtual _int Initial_Update_GameObject();
	_int Update_Parts();

	void Jump(float fDeltaTime);

	// �̵� ���
	void MoveMotion(float fDeltaTime);

	HRESULT Universal_Key();
	HRESULT Wnd_OpenClose();

private:
	// �Ϲ� ����
	void Normal_Attack(float fDeltaTime);
	// ��ų1 - ������
	void Skill_Laser(float fDeltaTime);
	// ��ų2 - ����ü ����
	void Skill_ProjectileFall(float fDeltaTime);

private:
	// hp �� ���� ����
	HRESULT Draw_HpBar();


public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject* Clone_GameObject(void* _pArg) override;
	virtual void Free() override;



private:
	CVIBuffer*			m_pVIBufferCom[PART_END];
	CTransform*			m_pTransformCom[PART_END];
	CTexture*			m_pTextureCom[PART_END];
	CRaycast*			m_pRaycastCom	= nullptr;
	CCollider*			m_pColliderCom	= nullptr;
	CStatus*			m_pStatusCom	= nullptr;
	CItemManager*		m_pItemMgrCom	= nullptr;



	bool		m_bIsTagetMove = false;
	_vec3		m_vTargetPos = {};
	

	// �÷��̾� �ʱ� ȸ���� ������
	bool			m_bInitial = true;
	D3DXVECTOR3		m_vConstRot[PART_END];

	// ��, �ٸ��� ���� �Ÿ���ŭ ������
	float			m_fDisY = 2.5f;
	float			m_fHandDis = 0.8f;
	float			m_fFootDis = 0.2f;

	//---------------------------------------------
	// �̵� ���
	//---------------------------------------------
	bool			m_bMove = false;
	CHANGE_MOVE		m_eMovingDir = CHANGE_LEFT;
	MOVEING_DIR		m_ePlayerDir = MOVING_UP;
	float			m_fMovingTime = 0.f;

	//---------------------------------------------
	// HP�� ���� ����
	//---------------------------------------------
	int				m_iHP = 100;

	//---------------------------------------------
	// ����
	//---------------------------------------------
	bool				m_bJump = false;
	float				m_fFallSpeed = 0.f;
	float				m_fJumpPower = 5.f;
	float				m_fJumpTime = 0.f;

	// �÷��̾ ����, ��ų ����� �� ������ �ʱ� ȸ���� ����
	D3DXVECTOR3			m_vInitialRot;
	// �÷��̾ ���� ��ų�� ����ϰ� �ִ� ������
	// ��� -> true, ���X -> false
	bool				m_bUsingSkill = false;
	// ���� �÷��̾ ����ϰ� �ִ� ��ų ���̵�
	ePLAYER_SKILL_ID	m_ePlayerSkillID = PLAYER_SKILL_END;

	//---------------------------------------------
	// �Ϲ� ����
	//---------------------------------------------
	bool				m_bIsNormalAtt = false;
	float				m_fAttTime = 0.f;
	bool				m_bRightAtt = false;
	bool				m_bLeftAtt = false;

	//---------------------------------------------
	// ��ų1 - ������
	//---------------------------------------------
	bool				m_bStartLaser = false;
	bool				m_bUsingLaser = false;
	float				m_fLaserTime = 0.f;

	//---------------------------------------------
	// ��ų2 - ����ü ����
	//---------------------------------------------
	bool				m_bStartFall = false;
	bool				m_bIsFall = false;
	bool				m_bDownHand = false;
	float				m_fFallTime = 0.f;

	//---------------------------------------------
	// �����¿� �̵��� �� ���� ť��� ����
	//---------------------------------------------
	bool				m_bLeftTurn = false;
	bool				m_bRightTurn = false;
	bool				m_bUpTurn = false;
	bool				m_bDownTurn = false;

	// ���� �׸��� ����
	bool				m_bRenderShop = false;
	bool				m_bRenderInven = false;
	//--------------------------------------------
};

END

#endif // !__PLAYER_H__
