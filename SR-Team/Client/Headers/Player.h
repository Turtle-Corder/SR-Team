#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "GameObject.h"

BEGIN(Client)
USING(Engine)

class CStatus;
class CDamageInfo;
class CWand;

class CPlayer final : public CGameObject
{
public:
	enum STATE { IDLE, MOVE, SKILL, STATE_END };
	enum CHANGE_MOVE { CHANGE_LEFT, CHANGE_RIGHT, MOVE_END };
	enum MOVEING_DIR { MOVING_UP, MOVING_DOWN, MOVING_LEFT, MOVING_RIGHT };
	enum ACTIVE_BUFF { BUFF_MANA, BUFF_ATT, BUFF_SHIELD, BUFF_END };

private:
	explicit CPlayer(LPDIRECT3DDEVICE9 _pDevice);
	explicit CPlayer(const CPlayer& _rOther);
	virtual ~CPlayer() = default;

public:
	_bool Get_IsOnActiveBuff(_int iIndex) { return m_bActiveBuff[iIndex]; }

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void* _pArg) override;

	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;

	virtual HRESULT Take_Damage(const CComponent* _pDamageComp) override;

private:
	HRESULT Add_Component();

	// ���� Ÿ��
	HRESULT IsOnTerrain();
	// �̵�
	HRESULT Movement(_float _fDeltaTime);
	void Move_Vertical(_float _fDeltaTime);
	void Move_Horizontal(_float _fDeltaTime);
	void Turn(_float _fDeltaTime);

	// ���콺 ��ŷ
	HRESULT RaycastOnTerrain();
	void Move_Target(_float _fDeltaTime);

	// ó�� ���� ���·� ���ư���
	virtual _int Initial_Update_GameObject();
	// ������ ������Ʈ
	_int Update_Parts();

	// ����
	void Jump(_float fDeltaTime);

	// �̵� ���
	void MoveMotion(_float fDeltaTime);

	HRESULT Universal_Key();

private:
	// �����Կ� �ִ� ��ų ���
	void Check_Skill(_float fDeltaTime);
	// ��ų�� ���� ���
	void Move_SkillMotion(_float fDeltaTime, eActiveSkill_ID eSkillID);

	// �����Կ� �ִ� ������ ���
	void Check_QuickSlotItem();

private:
	// �Ϲ� ����
	void Normal_Attack(_float fDeltaTime);
	// ��ų1 - ������, ������ ��Ʈ
	void Skill_Laser(_float fDeltaTime);
	// ��ų2 - ����ü ����
	void Skill_ProjectileFall(_float fDeltaTime);
	// ����1 - ���� �帮��Ʈ(���� �ð� ���� ���� ���� X)
	void Buff_ManaDrift(_float fDeltaTime);
	// ����2 - ������ �ͽ��÷���(���� �ð� ���� ���ݷ� ����)
	void Buff_EnergyExploitation(_float fDeltaTime);


private:
	HRESULT Ready_Layer_Meteor(const wstring& _strLayerTag, _vec3 vGoalPos);

	HRESULT Setup_Layer_PlaneSkill(const wstring & LayerTag, _vec3 _vMouse);

	HRESULT Setup_Layer_EnergyBolt(const wstring & LayerTag);

	HRESULT Setup_Layer_Wand(const wstring & LayerTag);


public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject* Clone_GameObject(void* _pArg) override;
	virtual void Free() override;



private:
	CVIBuffer*			m_pVIBufferCom[PART_END];
	CTransform*			m_pTransformCom[PART_END];
	CTexture*			m_pTextureCom[PART_END];
	CRaycast*			m_pRaycastCom	= nullptr;
	CSphereCollider*	m_pColliderCom	= nullptr;
	CStatus*			m_pStatusCom	= nullptr;
	CDamageInfo*		m_pDmgInfoCom	= nullptr;


	_float				m_fMeteor_Create_Time = 0.f;
	_bool				m_bIsTagetMove = false;
	_vec3				m_vTargetPos = {};
	_bool				m_bCheck = false;
	_bool				m_bOnece = false;
	// �÷��̾� �ʱ� ȸ���� ������
	_bool				m_bInitial = true;
	D3DXVECTOR3			m_vConstRot[PART_END];

	// ��, �ٸ��� ���� �Ÿ���ŭ ������
	_float				m_fDisY = 2.5f;
	_float				m_fHandDis = 0.8f;
	_float				m_fFootDis = 0.2f;
	_float				m_fRand[2] = {};
	//---------------------------------------------
	// �̵� ���
	//---------------------------------------------
	_bool				m_bMove = false;
	CHANGE_MOVE			m_eMovingDir = CHANGE_LEFT;
	MOVEING_DIR			m_ePlayerDir = MOVING_UP;
	_float				m_fMovingTime = 0.f;

	//---------------------------------------------
	// HP�� ���� ����
	//---------------------------------------------
	_int				m_iHP = 100;

	//---------------------------------------------
	// ����
	//---------------------------------------------
	_bool				m_bJump = false;
	_float				m_fFallSpeed = 0.f;
	_float				m_fJumpPower = 5.f;
	_float				m_fJumpTime = 0.f;

	// �÷��̾ ����, ��ų ����� �� ������ �ʱ� ȸ���� ����
	_vec3				m_vInitialRot;
	// �÷��̾ ���� ��ų�� ����ϰ� �ִ� ������
	// ��� -> true, ���X -> false
	_bool				m_bUsingSkill = false;
	// ���� �÷��̾ ����ϰ� �ִ� ��ų ���̵�
	ePLAYER_SKILL_ID	m_ePlayerSkillID = PLAYER_SKILL_END;

	//---------------------------------------------
	// �Ϲ� ����
	//---------------------------------------------
	_bool				m_bIsNormalAtt = false;
	_float				m_fAttTime = 0.f;
	_bool				m_bRightAtt = false;
	_bool				m_bLeftAtt = false;

	//---------------------------------------------
	// ��ų1 - ������
	//---------------------------------------------
	_bool				m_bStartLaser = true;
	_bool				m_bUsingLaser = false;
	_float				m_fLaserTime = 0.f;

	//---------------------------------------------
	// ��ų2 - ����ü ����
	//---------------------------------------------
	_bool				m_bStartFall = true;
	_bool				m_bIsFall = false;
	_bool				m_bDownHand = false;
	_float				m_fFallTime = 0.f;

	//---------------------------------------------
	// ���� ��ų�� ��� �ð� üũ
	//---------------------------------------------
	_float				m_fManaDriftTime = 0.f;
	_float				m_fEnergyExploitationSkill = 0.f;

	//---------------------------------------------
	// �����¿� �̵��� �� ���� ť��� ����
	//---------------------------------------------
	_bool				m_bLeftTurn = false;
	_bool				m_bRightTurn = false;
	_bool				m_bUpTurn = false;
	_bool				m_bDownTurn = false;

	// ����, �κ�, ���â�� ��µǾ� ������ true
	_bool				m_bRenderShop = false;
	_bool				m_bRenderInven = false;
	_bool				m_bRenderEquip = false;
	_bool				m_bRenderSkill = false;

	// ���� ������� ��Ƽ�� ��ųID
	eActiveSkill_ID eSkillID = ACTIVE_SKILL_END;

	// ���� �÷��̾ ����ϰ� �ִ� ������
	// ����� -> true
	_bool			m_bActiveBuff[BUFF_END] = { false, };

	_int			m_iAttBuff = 100;
	CWand*			m_pWand = nullptr;

	_float			m_fAttCount = 0.f;
	_float			m_fAttEndCount = 3.f;
	//--------------------------------------------
};

END

#endif // !__PLAYER_H__
