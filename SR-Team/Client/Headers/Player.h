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

	// 지형 타기
	HRESULT IsOnTerrain();
	// 이동
	HRESULT Movement(_float _fDeltaTime);
	void Move_Vertical(_float _fDeltaTime);
	void Move_Horizontal(_float _fDeltaTime);
	void Turn(_float _fDeltaTime);

	// 마우스 피킹
	HRESULT RaycastOnTerrain();
	void Move_Target(_float _fDeltaTime);

	// 처음 시작 상태로 돌아가기
	virtual _int Initial_Update_GameObject();
	// 부위별 업데이트
	_int Update_Parts();

	// 점프
	void Jump(_float fDeltaTime);

	// 이동 모션
	void MoveMotion(_float fDeltaTime);

	HRESULT Universal_Key();

private:
	// 퀵슬롯에 있는 스킬 사용
	void Check_Skill(_float fDeltaTime);
	// 스킬에 따른 모션
	void Move_SkillMotion(_float fDeltaTime, eActiveSkill_ID eSkillID);

	// 퀵슬롯에 있는 아이템 사용
	void Check_QuickSlotItem();

private:
	// 일반 공격
	void Normal_Attack(_float fDeltaTime);
	// 스킬1 - 레이저, 에너지 볼트
	void Skill_Laser(_float fDeltaTime);
	// 스킬2 - 투사체 낙하
	void Skill_ProjectileFall(_float fDeltaTime);
	// 버프1 - 마나 드리프트(일정 시간 동안 마나 감소 X)
	void Buff_ManaDrift(_float fDeltaTime);
	// 버프2 - 에너지 익스플로전(일정 시간 동안 공격력 증가)
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
	// 플레이어 초기 회전값 보관용
	_bool				m_bInitial = true;
	D3DXVECTOR3			m_vConstRot[PART_END];

	// 팔, 다리는 일정 거리만큼 떨어짐
	_float				m_fDisY = 2.5f;
	_float				m_fHandDis = 0.8f;
	_float				m_fFootDis = 0.2f;
	_float				m_fRand[2] = {};
	//---------------------------------------------
	// 이동 모션
	//---------------------------------------------
	_bool				m_bMove = false;
	CHANGE_MOVE			m_eMovingDir = CHANGE_LEFT;
	MOVEING_DIR			m_ePlayerDir = MOVING_UP;
	_float				m_fMovingTime = 0.f;

	//---------------------------------------------
	// HP바 지연 감소
	//---------------------------------------------
	_int				m_iHP = 100;

	//---------------------------------------------
	// 점프
	//---------------------------------------------
	_bool				m_bJump = false;
	_float				m_fFallSpeed = 0.f;
	_float				m_fJumpPower = 5.f;
	_float				m_fJumpTime = 0.f;

	// 플레이어가 공격, 스킬 사용할 때 참고할 초기 회전값 보관
	_vec3				m_vInitialRot;
	// 플레이어가 현재 스킬을 사용하고 있는 중인지
	// 사용 -> true, 사용X -> false
	_bool				m_bUsingSkill = false;
	// 현재 플레이어가 사용하고 있는 스킬 아이디
	ePLAYER_SKILL_ID	m_ePlayerSkillID = PLAYER_SKILL_END;

	//---------------------------------------------
	// 일반 공격
	//---------------------------------------------
	_bool				m_bIsNormalAtt = false;
	_float				m_fAttTime = 0.f;
	_bool				m_bRightAtt = false;
	_bool				m_bLeftAtt = false;

	//---------------------------------------------
	// 스킬1 - 레이저
	//---------------------------------------------
	_bool				m_bStartLaser = true;
	_bool				m_bUsingLaser = false;
	_float				m_fLaserTime = 0.f;

	//---------------------------------------------
	// 스킬2 - 투사체 낙하
	//---------------------------------------------
	_bool				m_bStartFall = true;
	_bool				m_bIsFall = false;
	_bool				m_bDownHand = false;
	_float				m_fFallTime = 0.f;

	//---------------------------------------------
	// 버프 스킬들 사용 시간 체크
	//---------------------------------------------
	_float				m_fManaDriftTime = 0.f;
	_float				m_fEnergyExploitationSkill = 0.f;

	//---------------------------------------------
	// 상하좌우 이동할 때 마다 큐브들 자전
	//---------------------------------------------
	_bool				m_bLeftTurn = false;
	_bool				m_bRightTurn = false;
	_bool				m_bUpTurn = false;
	_bool				m_bDownTurn = false;

	// 상점, 인벤, 장비창이 출력되어 있으면 true
	_bool				m_bRenderShop = false;
	_bool				m_bRenderInven = false;
	_bool				m_bRenderEquip = false;
	_bool				m_bRenderSkill = false;

	// 현재 사용중인 액티브 스킬ID
	eActiveSkill_ID eSkillID = ACTIVE_SKILL_END;

	// 현재 플레이어가 사용하고 있는 버프들
	// 사용중 -> true
	_bool			m_bActiveBuff[BUFF_END] = { false, };

	_int			m_iAttBuff = 100;
	CWand*			m_pWand = nullptr;

	_float			m_fAttCount = 0.f;
	_float			m_fAttEndCount = 3.f;
	//--------------------------------------------
};

END

#endif // !__PLAYER_H__
