#include "stdafx.h"
#include "Status.h"
#include "Inventory.h"
#include "DamageInfo.h"
#include "Shop.h"
#include "Equip.h"
#include "PlaneSkill.h"
#include "Skill.h"
#include "SkillInven.h"
#include "ItemInventory.h"
#include "Wand.h"
#include "Mouse.h"
#include "..\Headers\Player.h"

USING(Client)

CPlayer::CPlayer(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
	for (_uint iCnt = 0; iCnt < PART_END; ++iCnt)
	{
		m_pTextureCom[iCnt] = nullptr;
		m_pTransformCom[iCnt] = nullptr;
		m_pVIBufferCom[iCnt] = nullptr;
	}
}

CPlayer::CPlayer(const CPlayer& _rOther)
	:CGameObject(_rOther)
{
}

HRESULT CPlayer::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Setup_GameObject(void * _pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;


	if (FAILED(Setup_Layer_Wand(L"Layer_Wand")))
		return E_FAIL;

	//---------------------------------------------
	// HP바 지연 감소
	//---------------------------------------------
	m_iHP = 100;

	m_bUsingSkill = false;
	m_ePlayerSkillID = PLAYER_SKILL_END;

	//---------------------------------------------
	// 점프
	//---------------------------------------------
	m_bJump = false;
	m_fFallSpeed = 0.f;
	m_fJumpPower = 5.f;
	m_fJumpTime = 0.f;

	//---------------------------------------------
	// 일반 공격
	//---------------------------------------------
	m_bIsNormalAtt = false;
	m_fAttTime = 0.f;
	m_bRightAtt = false;
	m_bLeftAtt = false;

	//---------------------------------------------
	// 스킬1 - 레이저
	//---------------------------------------------
	m_bStartLaser = false;
	m_bUsingLaser = false;
	m_fLaserTime = 0.f;

	//---------------------------------------------
	// 스킬2 - 투사체 낙하
	//---------------------------------------------
	m_bStartFall = false;
	m_bIsFall = false;
	m_bDownHand = false;
	m_fFallTime = 0.f;

	// 현재 사용중인 액티브 스킬ID
	eSkillID = ACTIVE_SKILL_END;

	_vec3 vRight = { 1.f, 0.f, 0.f };

	// 오른쪽 손
	_vec3 vRightHand = (vRight * m_fHandDis);
	m_pTransformCom[PART_HAND_RIGHT]->Set_Position(vRightHand);
	// 왼쪽 손
	_vec3 vLeftHand = (-vRight * m_fHandDis);
	m_pTransformCom[PART_HAND_LEFT]->Set_Position(vLeftHand);
	// 오른쪽 발
	_vec3 vRightFoot = (vRight * 0.3f);
	vRightFoot.y = -0.7f;

	m_pTransformCom[PART_FOOT_RIGHT]->Set_Position(vRightFoot);
	// 왼쪽 발
	_vec3 vLeftFoot = (-vRight * 0.3f);
	vLeftFoot.y = -0.7f;
	m_pTransformCom[PART_FOOT_LEFT]->Set_Position(vLeftFoot);
	
	return S_OK;
}

_int CPlayer::Update_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;
	CInventory* pInven = (CInventory*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Inventory");
	if (pInven == nullptr)
		return 0;
	CShop* pShop = (CShop*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Shop");
	if (pShop == nullptr)
		return 0;
	CEquip* pEquip = (CEquip*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_MainUI", 1);
	if (pEquip == nullptr)
		return 0;
	CSkill* pSkill = (CSkill*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_MainUI", 2);
	if (pSkill == nullptr)
		return 0;

	// 에너지 익스플로전 사용중이면 공격력 증가
	if (m_bActiveBuff[BUFF_ATT])
		pEquip->Set_PlayerAtt(m_iAttBuff);

	if (GetAsyncKeyState(VK_F6) & 0x8000)
	{
		if (FAILED(Setup_Layer_EnergyBolt(L"Layer_EnergyBolt")))
			return E_FAIL;
	}

	if (m_bInitial)
		Initial_Update_GameObject();

	m_bRenderInven = pInven->Get_Render();
	m_bRenderShop = pShop->Get_Render();
	m_bRenderEquip = pEquip->Get_Render();
	m_bRenderSkill = pSkill->Get_Render();

	if (!m_bRenderInven && !m_bRenderShop && !m_bRenderEquip && !m_bRenderSkill)
	{
		if (FAILED(Movement(_fDeltaTime)))
			return GAMEOBJECT::WARN;

		if (Update_Parts())
			return GAMEOBJECT::WARN;

		if (FAILED(m_pColliderCom->Update_Collider(m_pTransformCom[PART_BODY]->Get_Desc().vPosition)))
			return GAMEOBJECT::WARN;
	}

	return GAMEOBJECT::NOEVENT;
}

_int CPlayer::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagemnet = CManagement::Get_Instance();
	if (nullptr == pManagemnet)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagemnet->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CPlayer::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	for (_uint iCnt = 0; iCnt < PART_END; ++iCnt)
	{
		if (FAILED(m_pVIBufferCom[iCnt]->Set_Transform(&m_pTransformCom[iCnt]->Get_Desc().matWorld, pCamera)))
			return E_FAIL;

		if (FAILED(m_pTextureCom[iCnt]->SetTexture(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom[iCnt]->Render_VIBuffer()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CPlayer::Take_Damage(const CComponent* _pDamageComp)
{
	if (!_pDamageComp)
		return S_OK;

	m_pStatusCom->Set_HP(((CDamageInfo*)_pDamageComp)->Get_Desc().iMinAtt);

	//if (0 >= m_pStatusCom->Get_Status().iHp)
	//	PRINT_LOG(L"아얏", LOG::CLIENT);

	return S_OK;
}

HRESULT CPlayer::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;

	for (_uint iCnt = 0; iCnt < PART_END; ++iCnt)
	{
		TCHAR szVIBuffer[MAX_PATH] = L"";
		StringCchPrintf(szVIBuffer, sizeof(TCHAR) * MAX_PATH,
			L"Com_VIBuffer%d", iCnt);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", szVIBuffer, (CComponent**)&m_pVIBufferCom[iCnt])))
			return E_FAIL;


		if (iCnt == PART_HEAD)
		{
			tTransformDesc.vPosition = { 5.f, 5.f, 5.f };
		}
		else if (iCnt == PART_BODY)
		{
			_vec3 vHeadPos = m_pTransformCom[PART_HEAD]->Get_Desc().vPosition;
			tTransformDesc.vPosition = { vHeadPos.x, vHeadPos.y - 1.5f, vHeadPos.z };
			tTransformDesc.vScale = { 0.9f, 0.9f, 0.9f };
		}
		else if (iCnt == PART_HAND_LEFT)
		{
			_vec3 vBodyPos = m_pTransformCom[PART_BODY]->Get_Desc().vPosition;
			tTransformDesc.vPosition
				= { vBodyPos.x - 0.8f, vBodyPos.y - 2.5f, vBodyPos.z };
			tTransformDesc.vScale = { 0.2f, 0.7f, 0.2f };
		}
		else if (iCnt == PART_HAND_RIGHT)
		{
			_vec3 vBodyPos = m_pTransformCom[PART_BODY]->Get_Desc().vPosition;
			tTransformDesc.vPosition
				= { vBodyPos.x + 0.8f, vBodyPos.y - 2.5f, vBodyPos.z };
			tTransformDesc.vScale = { 0.2f, 0.7f, 0.2f };
		}
		else if (iCnt == PART_FOOT_LEFT)
		{
			_vec3 vBodyPos = m_pTransformCom[PART_BODY]->Get_Desc().vPosition;
			tTransformDesc.vPosition = { vBodyPos.x - 0.2f, vBodyPos.y - 2.5f, vBodyPos.z };
			tTransformDesc.vScale = { 0.3f, 0.5f, 0.5f };
		}
		else if (iCnt == PART_FOOT_RIGHT)
		{
			_vec3 vBodyPos = m_pTransformCom[PART_BODY]->Get_Desc().vPosition;
			tTransformDesc.vPosition = { vBodyPos.x + 0.2f, vBodyPos.y - 2.5f, vBodyPos.z };
			tTransformDesc.vScale = { 0.3f, 0.5f, 0.5f };
		}
		tTransformDesc.fSpeedPerSecond = 10.f;
		tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);

		TCHAR szTransform[MAX_PATH] = L"";
		StringCchPrintf(szTransform, sizeof(TCHAR) * MAX_PATH,
			L"Com_Transform%d", iCnt);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", szTransform, (CComponent**)&m_pTransformCom[iCnt], &tTransformDesc)))
			return E_FAIL;

		// For.Com_Texture
		TCHAR szTexture[MAX_PATH] = L"", szTextureName[MAX_PATH] = L"";
		StringCchPrintf(szTexture, sizeof(TCHAR) * MAX_PATH,
			L"Com_Texture%d", iCnt);
		if (iCnt == PART_HEAD)			StringCchPrintf(szTextureName, sizeof(TCHAR) * MAX_PATH,L"Component_Texture_Monster");
		else if (iCnt == PART_BODY)		StringCchPrintf(szTextureName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_PlayerHead");
		else if (iCnt == PART_HAND_LEFT || iCnt == PART_HAND_RIGHT)	StringCchPrintf(szTextureName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_PlayerHand");
		else							StringCchPrintf(szTextureName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_PlayerFoot");

		if (FAILED(CGameObject::Add_Component(SCENE_STAGE0, szTextureName, szTexture, (CComponent**)&m_pTextureCom[iCnt])))
			return E_FAIL;
	}

	// For.Com_Stat
	CStatus::STAT tStat;
	tStat.iCriticalRate = 20;	tStat.iCriticalHit = 10;
	tStat.iDef = 50;
	tStat.iHp = 100;			tStat.iMp = 100;
	tStat.iMaxHp = 100;			tStat.iMaxMp = 100;
	tStat.iMinAtt = 10;			tStat.iMaxAtt = 50;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Status", L"Com_Stat", (CComponent**)&m_pStatusCom, &tStat)))
		return E_FAIL;

	CSphereCollider::COLLIDER_DESC tColDesc;
	tColDesc.vPosition = tTransformDesc.vPosition;
	tColDesc.fRadius = 0.7f;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &tColDesc)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Raycast", L"Com_Ray", (CComponent**)&m_pRaycastCom)))
		return E_FAIL;

	CDamageInfo::DAMAGE_DESC tDmgInfo;
	tDmgInfo.iMaxAtt = 99;
	tDmgInfo.pOwner = this;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_DamageInfo", L"Com_DmgInfo", (CComponent**)&m_pDmgInfoCom, &tDmgInfo)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Movement(_float _fDeltaTime)
{
	m_bMove = false;
	if (!m_bUsingSkill)
	{
		Move_Vertical(_fDeltaTime);
		Turn(_fDeltaTime);

		MoveMotion(_fDeltaTime);
	}

	Normal_Attack(_fDeltaTime);
	Check_Skill(_fDeltaTime);
	Check_QuickSlotItem();

	if (m_bActiveBuff[BUFF_MANA])
		Buff_ManaDrift(_fDeltaTime);
	if (m_bActiveBuff[BUFF_ATT])
		Buff_EnergyExploitation(_fDeltaTime);

	if (FAILED(IsOnTerrain()))
		return E_FAIL;

	if (FAILED(RaycastOnTerrain()))
		return E_FAIL;
	Jump(_fDeltaTime);

	return S_OK;
}

HRESULT CPlayer::IsOnTerrain()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CVIBuffer_TerrainTexture* pTerrainBuffer = (CVIBuffer_TerrainTexture*)pManagement->Get_Component(SCENE_STAGE0, L"Layer_Terrain", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	for (_uint iCnt = 4; iCnt < PART_END; ++iCnt)
	{
		_vec3 vPosition = m_pTransformCom[iCnt]->Get_Desc().vPosition;
		if (true == pTerrainBuffer->IsOnTerrain(&vPosition))
		{
			m_pTransformCom[iCnt]->Set_Position(vPosition);
		}
	}

	float fLeftFootPosY = m_pTransformCom[PART_FOOT_LEFT]->Get_Desc().vPosition.y;
	// 변화한 다리의 위치에 따라 몸의 위치도 변경해준다
	_vec3 vBodyPos = m_pTransformCom[PART_BODY]->Get_Desc().vPosition;
	vBodyPos.y = fLeftFootPosY + 1.f;
	m_pTransformCom[PART_BODY]->Set_Position(vBodyPos);

	_vec3 vHeadPos = m_pTransformCom[PART_HEAD]->Get_Desc().vPosition;
	vHeadPos.y = vBodyPos.y + 0.5f;
	m_pTransformCom[PART_HEAD]->Set_Position(vHeadPos);

	_vec3 vLeftHandPos = m_pTransformCom[PART_HAND_LEFT]->Get_Desc().vPosition;
	vLeftHandPos.y = fLeftFootPosY + 1.f;
	m_pTransformCom[PART_HAND_LEFT]->Set_Position(vLeftHandPos);

	_vec3 vRightHandPos = m_pTransformCom[PART_HAND_RIGHT]->Get_Desc().vPosition;
	vRightHandPos.y = fLeftFootPosY + 1.f;
	m_pTransformCom[PART_HAND_RIGHT]->Set_Position(vRightHandPos);

	return S_OK;
}

HRESULT CPlayer::RaycastOnTerrain()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CVIBuffer_TerrainTexture* pTerrainBuffer = (CVIBuffer_TerrainTexture*)pManagement->Get_Component(SCENE_STAGE0, L"Layer_Terrain", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	// 카메라의 월드행렬은 항등행렬!
	_matrix mat;
	D3DXMatrixIdentity(&mat);

	// 반환받을 마우스 위치값
	_vec3 vOutPos;

	if (GetAsyncKeyState(VK_F1) & 0x8000)
	{
		m_bCheck = true;
		m_bOnece = true;
	}

	if (m_bCheck)
	{
		if (m_bOnece)
		{
			if (Setup_Layer_PlaneSkill(L"Layer_PlaneSkill", vOutPos))
				return E_FAIL;

			m_bOnece = false;
		}

		if (m_pRaycastCom->IsSimulate<VTX_TEXTURE, INDEX16>(
			g_hWnd, WINCX, WINCY, pTerrainBuffer, &mat, pCamera, &vOutPos))
		{
			CTransform* pSkill = (CTransform*)pManagement->Get_Component(SCENE_STAGE0, L"Layer_PlaneSkill", L"Com_Transform");
			if (nullptr == pSkill)
				return E_FAIL;

			pSkill->Set_Position(vOutPos);

			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			{

			}
		}
	}
	return S_OK;
}

void CPlayer::Move_Vertical(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return;

	if (pManagement->Key_Pressing(VK_UP))
	{
		m_ePlayerDir = MOVING_UP;
		if (!m_bUpTurn)
		{
			for (_uint i = 0; i < 2; ++i)
			{
				_vec3 vRot = m_pTransformCom[i]->Get_Desc().vRotate;
				vRot.y = 0.f;
				//m_pTransformCom[i]->Set_Rotation(vRot);
			}
			m_bUpTurn = true;
		}

		// 위로 이동
		for (_uint i = 0; i < 2; ++i)
		{
			m_pTransformCom[i]->Move_Vertical(_fDeltaTime);
		}

		if (m_bLeftTurn)
			m_bLeftTurn = false;
		if (m_bRightTurn)
			m_bRightTurn = false;
		if (m_bDownTurn)
			m_bDownTurn = false;
		m_bMove = true;
	}

	else if (pManagement->Key_Pressing(VK_DOWN))
	{
		m_ePlayerDir = MOVING_DOWN;
		if (!m_bDownTurn)
		{
			m_bDownTurn = true;
		}

		for (_uint i = 0; i < 2; ++i)
			m_pTransformCom[i]->Move_Vertical(-_fDeltaTime);

		if (m_bLeftTurn)
			m_bLeftTurn = false;
		if (m_bRightTurn)
			m_bRightTurn = false;
		if (m_bUpTurn)
			m_bUpTurn = false;
		m_bMove = true;
	}
}

void CPlayer::Move_Horizontal(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return;

	if (pManagement->Key_Pressing(VK_LEFT))
	{
		m_ePlayerDir = MOVING_LEFT;
		if (!m_bLeftTurn)
		{
			for (_uint i = 0; i < 2; ++i)
			{
				_vec3 vRot = m_pTransformCom[i]->Get_Desc().vRotate;
				vRot.y = -1.5f;
				m_pTransformCom[i]->Set_Rotation(vRot);
			}

			m_bLeftTurn = true;
		}

		for (_uint i = 0; i < 2; ++i)
			m_pTransformCom[i]->Move_Vertical(_fDeltaTime);

		m_bMove = true;
		if (m_bRightTurn)
			m_bRightTurn = false;
		if (m_bDownTurn)
			m_bDownTurn = false;
		if (m_bUpTurn)
			m_bUpTurn = false;
	}

	else if (pManagement->Key_Pressing(VK_RIGHT))
	{
		m_ePlayerDir = MOVING_RIGHT;
		if (!m_bRightTurn)
		{
			for (_uint i = 0; i < 2; ++i)
			{
				_vec3 vRot = m_pTransformCom[i]->Get_Desc().vRotate;
				vRot.y = 1.57079637f;
				m_pTransformCom[i]->Set_Rotation(vRot);
			}
			m_bRightTurn = true;
		}

		for (_uint i = 0; i < 2; ++i)
			m_pTransformCom[i]->Move_Vertical(_fDeltaTime);

		m_bMove = true;
		if (m_bLeftTurn)
			m_bLeftTurn = false;
		if (m_bDownTurn)
			m_bDownTurn = false;
		if (m_bUpTurn)
			m_bUpTurn = false;
	}
}

void CPlayer::Turn(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return;

	if (pManagement->Key_Pressing(VK_LEFT))
	{
		for (_uint i = 0; i < 2; ++i)
			m_pTransformCom[i]->Turn(CTransform::AXIS_Y, -_fDeltaTime);
	}

	if (pManagement->Key_Pressing(VK_RIGHT))
	{
		for (_uint i = 0; i < 2; ++i)
			m_pTransformCom[i]->Turn(CTransform::AXIS_Y, _fDeltaTime);
	}
}

void CPlayer::Move_Target(_float _fDeltaTime)
{
	//if (false == m_bIsTagetMove)
	//	return;

	//_vec3 vCurPos = m_pTransformCom->Get_Desc().vPosition;
	//_vec3 vMoveDir = m_vTargetPos - vCurPos;

	//_float fSpeedPerSecond = m_pTransformCom->Get_Desc().fSpeedPerSecond;
	//_float fDist = D3DXVec3Length(&vMoveDir);

	//if (0.6f >= fDist)
	//{
	//	m_bIsTagetMove = false;
	//	return;
	//}

	//D3DXVec3Normalize(&vMoveDir, &vMoveDir);

	//vCurPos += vMoveDir * fSpeedPerSecond * _fDeltaTime;
	//m_pTransformCom->Set_Position(vCurPos);
}

_int CPlayer::Initial_Update_GameObject()
{
	for (_uint iCnt = 0; iCnt < PART_END; ++iCnt)
	{
		m_vConstRot[iCnt] = m_pTransformCom[iCnt]->Get_Desc().vRotate;
	}
	m_bInitial = false;

	return GAMEOBJECT::NOEVENT;
}

_int CPlayer::Update_Parts()
{
	for (_uint iCnt = 0; iCnt < PART_END; ++iCnt)
	{
		if (iCnt > PART_BODY)
		{
			_vec3 vPos = m_pTransformCom[iCnt]->Get_Desc().vPosition;
			if (iCnt == PART_FOOT_LEFT || iCnt == PART_FOOT_RIGHT)
				vPos.y -= 1.5f;
			else
				vPos.y -= 1.f;
			m_pTransformCom[iCnt]->Set_Position(vPos);
		}

		if (FAILED(m_pTransformCom[iCnt]->Update_Transform()))
			return GAMEOBJECT::WARN;
	}

	// 공전 곱
	_vec3 vRotate = m_pTransformCom[PART_BODY]->Get_Desc().vRotate;
	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixRotationZ(&matWorld, vRotate.z);
	D3DXMatrixRotationY(&matWorld, vRotate.y);
	D3DXMatrixRotationX(&matWorld, vRotate.x);

	m_pTransformCom[PART_HAND_LEFT]->Set_WorldMatrix(
		m_pTransformCom[PART_HAND_LEFT]->Get_Desc().matWorld * matWorld);
	m_pTransformCom[PART_HAND_RIGHT]->Set_WorldMatrix(
		m_pTransformCom[PART_HAND_RIGHT]->Get_Desc().matWorld * matWorld);
	m_pTransformCom[PART_FOOT_LEFT]->Set_WorldMatrix(
		m_pTransformCom[PART_FOOT_LEFT]->Get_Desc().matWorld * matWorld);
	m_pTransformCom[PART_FOOT_RIGHT]->Set_WorldMatrix(
		m_pTransformCom[PART_FOOT_RIGHT]->Get_Desc().matWorld * matWorld);

	// 부모의 월드행렬 곱해주기
	m_pTransformCom[PART_HAND_LEFT]->Set_WorldMatrix(
		(m_pTransformCom[PART_HAND_LEFT]->Get_Desc().matWorld
			* m_pTransformCom[PART_BODY]->Get_Desc().matWorld));
	m_pTransformCom[PART_HAND_RIGHT]->Set_WorldMatrix(
		(m_pTransformCom[PART_HAND_RIGHT]->Get_Desc().matWorld
			* m_pTransformCom[PART_BODY]->Get_Desc().matWorld));
	m_pTransformCom[PART_FOOT_LEFT]->Set_WorldMatrix(
		(m_pTransformCom[PART_FOOT_LEFT]->Get_Desc().matWorld
			* m_pTransformCom[PART_BODY]->Get_Desc().matWorld));
	m_pTransformCom[PART_FOOT_RIGHT]->Set_WorldMatrix(
		(m_pTransformCom[PART_FOOT_RIGHT]->Get_Desc().matWorld
			* m_pTransformCom[PART_BODY]->Get_Desc().matWorld));

	return GAMEOBJECT::NOEVENT;
}

void CPlayer::Jump(_float fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return;

	int k = 0;
	if (pManagement->Key_Pressing(VK_SPACE))
	{
		m_bJump = true;
	}

	_vec3 vCurPos[PART_END];
	// 플레이어의 현재 위치를 계속 받아온다
	for (_uint i = 0; i < PART_END; ++i)
		vCurPos[i] = m_pTransformCom[i]->Get_Desc().vPosition;

	if (m_bJump)
	{
		// 시간 더해주기
		m_fJumpTime += fDeltaTime;
		for (_uint i = 0; i < 2; ++i)
		{
			// 점프
			vCurPos[i].y += (m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime * 0.5f);

			if (vCurPos[i].y <= m_pTransformCom[i]->Get_Desc().vPosition.y)
			{
				m_fJumpTime = 0.f;
				m_bJump = false;
				vCurPos[i].y = m_pTransformCom[i]->Get_Desc().vPosition.y;
			}

			m_pTransformCom[i]->Set_Position(vCurPos[i]);
		}
	}
}

void CPlayer::MoveMotion(_float fDeltaTime)
{
	if (m_bMove)
	{
		m_fMovingTime += fDeltaTime;

		if (m_fMovingTime >= 0.5f)
		{
			if (m_eMovingDir == CHANGE_LEFT)
				m_eMovingDir = CHANGE_RIGHT;
			else if (m_eMovingDir == CHANGE_RIGHT)
				m_eMovingDir = CHANGE_LEFT;
			m_fMovingTime = 0.f;

			for (_uint i = 2; i < PART_END; ++i)
				m_pTransformCom[i]->Set_Rotation(m_vConstRot[i]);
		}

		if (m_eMovingDir == CHANGE_LEFT)
		{
			m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, -fDeltaTime);
			m_pTransformCom[PART_HAND_LEFT]->Turn(CTransform::AXIS_X, fDeltaTime);

			m_pTransformCom[PART_FOOT_RIGHT]->Turn(CTransform::AXIS_X, fDeltaTime);
			m_pTransformCom[PART_FOOT_LEFT]->Turn(CTransform::AXIS_X, -fDeltaTime);
		}

		else if (m_eMovingDir == CHANGE_RIGHT)
		{	
			m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, fDeltaTime);
			m_pTransformCom[PART_HAND_LEFT]->Turn(CTransform::AXIS_X, -fDeltaTime);

			m_pTransformCom[PART_FOOT_RIGHT]->Turn(CTransform::AXIS_X, -fDeltaTime);
			m_pTransformCom[PART_FOOT_LEFT]->Turn(CTransform::AXIS_X, fDeltaTime);
		}
	}
	else if (!m_bMove)
	{
		m_fMovingTime = 0.f;
		for (_uint i = 2; i < PART_END; ++i)
		{
			m_pTransformCom[i]->Set_Rotation(m_vConstRot[i]);
		}
	}
}

HRESULT CPlayer::Universal_Key()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		E_FAIL;

	if (pManagement->Key_Pressing('G'))
	{
	}

	return S_OK;
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CPlayer* pInstance = new CPlayer(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create Player", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayer::Clone_GameObject(void * _pArg)
{
	CPlayer* pInstance = new CPlayer(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone Player", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	for (_uint i = 0; i < PART_END; ++i)
	{
		Safe_Release(m_pTextureCom[i]);
		Safe_Release(m_pTransformCom[i]);
		Safe_Release(m_pVIBufferCom[i]);
	}

	Safe_Release(m_pStatusCom);
	Safe_Release(m_pRaycastCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pDmgInfoCom);

	CGameObject::Free();
}

void CPlayer::Check_Skill(_float fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return;
	CSkillInven* pSkillInven = (CSkillInven*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_MainUI", 3);
	if (pSkillInven == nullptr)
		return;

	if (pManagement->Key_Pressing('Q'))
	{
		m_vInitialRot = m_pTransformCom[PART_HAND_RIGHT]->Get_Desc().vRotate;
		m_bUsingSkill = true;

		// 공격체 생성
		if (FAILED(pSkillInven->Use_Skill(0, fDeltaTime)))
		{
			//m_bUsingSkill = false;
			return;
		}

		// 모션
		eSkillID = pSkillInven->Get_SkillID(0);
		m_bOnece = true;
	}
	else if (pManagement->Key_Pressing('W'))
	{
		m_vInitialRot = m_pTransformCom[PART_HAND_RIGHT]->Get_Desc().vRotate;
		m_bUsingSkill = true;

		if (FAILED(pSkillInven->Use_Skill(1, fDeltaTime)))
		{
			m_bUsingSkill = false;
			return;
		}

		// 모션
		eSkillID = pSkillInven->Get_SkillID(0);
	}
	else if (pManagement->Key_Pressing('E'))
	{
		m_vInitialRot = m_pTransformCom[PART_HAND_RIGHT]->Get_Desc().vRotate;
		m_bUsingSkill = true;

		if (FAILED(pSkillInven->Use_Skill(2, fDeltaTime)))
		{
			m_bUsingSkill = false;
			return;
		}

		// 모션
		eSkillID = pSkillInven->Get_SkillID(2);
	}
	else if (pManagement->Key_Pressing('R'))
	{
		m_vInitialRot = m_pTransformCom[PART_HAND_RIGHT]->Get_Desc().vRotate;
		m_bUsingSkill = true;

		if (FAILED(pSkillInven->Use_Skill(3, fDeltaTime)))
		{
			m_bUsingSkill = false;
			return;
		}

		// 모션
		eSkillID = pSkillInven->Get_SkillID(3);
	}
	else if (pManagement->Key_Pressing('A'))
	{
		m_vInitialRot = m_pTransformCom[PART_HAND_RIGHT]->Get_Desc().vRotate;
		m_bUsingSkill = true;

		if (FAILED(pSkillInven->Use_Skill(4, fDeltaTime)))
		{
			m_bUsingSkill = false;
			return;
		}

		// 모션
		eSkillID = pSkillInven->Get_SkillID(4);
	}
	else if (pManagement->Key_Pressing('S'))
	{
		m_vInitialRot = m_pTransformCom[PART_HAND_RIGHT]->Get_Desc().vRotate;
		m_bUsingSkill = true;

		if (FAILED(pSkillInven->Use_Skill(5, fDeltaTime)))
		{
			m_bUsingSkill = false;
			return;
		}

		// 모션
		eSkillID = pSkillInven->Get_SkillID(5);
	}
	else if (pManagement->Key_Pressing('D'))
	{
		m_vInitialRot = m_pTransformCom[PART_HAND_RIGHT]->Get_Desc().vRotate;
		m_bUsingSkill = true;

		if (FAILED(pSkillInven->Use_Skill(6, fDeltaTime)))
		{
			m_bUsingSkill = false;
			return;
		}

		// 모션
		eSkillID = pSkillInven->Get_SkillID(6);
	}
	else if (pManagement->Key_Pressing('F'))
	{
		m_vInitialRot = m_pTransformCom[PART_HAND_RIGHT]->Get_Desc().vRotate;
		m_bUsingSkill = true;

		if (FAILED(pSkillInven->Use_Skill(7, fDeltaTime)))
		{
			m_bUsingSkill = false;
			return;
		}

		// 모션
		eSkillID = pSkillInven->Get_SkillID(7);
	}


	if (m_bUsingSkill)
	{
		if (eSkillID == ACTIVE_FLAME_WAVE)
			m_bFrameWaveStart = true;
		Move_SkillMotion(fDeltaTime, eSkillID);
	}
	else
		eSkillID = ACTIVE_SKILL_END;
}

void CPlayer::Move_SkillMotion(_float fDeltaTime, eActiveSkill_ID eSkillID)
{
	// MP 감소
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return;
	CEquip* pEquip = (CEquip*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_MainUI", 1);
	if (pEquip == nullptr)
		return;
	_bool bUseMp = true;

	// 마나 드리프트 버프 사용중이면 mp감소 X
	if (m_bActiveBuff[BUFF_MANA])
		bUseMp = false;

	switch (eSkillID)
	{
	case ACTIVE_ICE_STRIKE:	// 에너지 볼트
		if (bUseMp)
			pEquip->Set_PlayerMP(-10);
		Skill_Laser(fDeltaTime);
		break;
	case ACTIVE_MANA_DRIFT:	// 마나 드리프트
		if (bUseMp)
			pEquip->Set_PlayerMP(-20);
		m_bActiveBuff[BUFF_MANA] = true;
		break;
	case ACTIVE_ENERGY_EXPLOTIATION:	// 에너지 익스플로전
		if (bUseMp)
			pEquip->Set_PlayerMP(-30);
		m_bActiveBuff[BUFF_ATT] = true;
		break;
	case ACTIVE_FLAME_WAVE:	// 투사체
		//if (m_bFrameWaveStart)
		//{
		//	if (bUseMp)
		//		pEquip->Set_PlayerMP(-30);
		//	m_fFrameWaveCnt += fDeltaTime;

		//	if (m_fFrameWaveCnt >= m_fFrameWaveEnd)
		//	{
		//		m_bFrameWaveStart = false;
		//		m_fFrameWaveCnt = 0.f;

		//		Skill_ProjectileFall(fDeltaTime);

		//		//m_bUsingSkill = false;
		//	}
		//}
		m_fFrameWaveCnt += fDeltaTime;
		if (m_fFrameWaveCnt >= m_fFrameWaveEnd)
		{
			m_bFrameWaveStart = false;
			if (bUseMp)
				pEquip->Set_PlayerMP(-30);
			Skill_ProjectileFall(fDeltaTime);
			m_bUsingSkill = false;
			m_fFrameWaveCnt = 0.f;
		}
		break;
	case ACTIVE_ICE_SPEAR: // 레이저
		if (bUseMp)
			pEquip->Set_PlayerMP(-20);
		Skill_Laser(fDeltaTime);
		break;
	case ACTIVE_MAGIC_ARMOR:
		break;
	case ACTIVE_NORMAL_ATTACK:
		break;
	case ACTIVE_SKILL_END:
		break;
	default:
		break;
	}
}

void CPlayer::Check_QuickSlotItem()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return;
	CItemInventory* pItemInven = (CItemInventory*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_MainUI", 4);
	if (pItemInven == nullptr)
		return;

	if (pManagement->Key_Down('1'))
	{
		if (FAILED(pItemInven->Use_Item(0)))
			return;
	}
	else if (pManagement->Key_Down('2'))
	{
		if (FAILED(pItemInven->Use_Item(1)))
			return;
	}
	else if (pManagement->Key_Down('3'))
	{
		if (FAILED(pItemInven->Use_Item(2)))
			return;
	}
	else if (pManagement->Key_Down('4'))
	{
		if (FAILED(pItemInven->Use_Item(3)))
			return;
	}
}

void CPlayer::Normal_Attack(_float fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return;

	int k = 0;

	//if (GetAsyncKeyState('Z') & 0x8000)
	if (pManagement->Key_Pressing('Z'))
	{
		m_bUsingSkill = true;
		m_ePlayerSkillID = PLAYER_NORMAL_ATTACK;
		m_vInitialRot = m_pTransformCom[PART_HAND_RIGHT]->Get_Desc().vRotate;

		m_bIsNormalAtt = true;
		m_bRightAtt = false;
		m_bLeftAtt = false;
	}

	// 일반 공격은 팔만 움직이면 된다
	else if (m_bIsNormalAtt)
	{
		if (m_bRightAtt)
		{
			m_fAttTime += fDeltaTime;
			if (m_fAttTime >= 0.2f)
			{
				m_fAttTime = 0.f;
				//m_bIsNormalAtt = false;
				m_bRightAtt = false;
				m_bLeftAtt = true;
			}

			m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_Y, fDeltaTime * 5.f);

			//if (m_ePlayerDir == MOVING_UP || m_ePlayerDir == MOVING_DOWN)
			//	m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_Y, fDeltaTime * 5.f);
			//else if (m_ePlayerDir == MOVING_RIGHT)
			//	m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, fDeltaTime * 5.f);
		}

		else if (m_bLeftAtt)
		{
			m_fAttTime += fDeltaTime;
			m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_Y, -fDeltaTime * 5.f);

			//if (m_ePlayerDir == MOVING_UP || m_ePlayerDir == MOVING_DOWN)
			//	m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_Y, -fDeltaTime * 5.f);
			//else if (m_ePlayerDir == MOVING_RIGHT)
			//	m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, -fDeltaTime * 5.f);

			if (m_fAttTime >= 0.3f)
			{
				m_fAttTime = 0.f;
				m_bIsNormalAtt = false;
				m_bRightAtt = false;
				m_bLeftAtt = false;

				m_pTransformCom[PART_HAND_RIGHT]->Set_Rotation(m_vInitialRot);
				m_bUsingSkill = false;
				m_ePlayerSkillID = PLAYER_SKILL_END;
			}
		}

		else
		{
			m_fAttTime += fDeltaTime;
			m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, -fDeltaTime * 5.f);

			//if (m_ePlayerDir == MOVING_UP)
			//	m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, -fDeltaTime * 5.f);
			//else if (m_ePlayerDir == MOVING_DOWN)
			//	m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, fDeltaTime * 5.f);
			//else if (m_ePlayerDir == MOVING_RIGHT)
			//	m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_Z, fDeltaTime * 5.f);

			if (m_fAttTime >= 0.2f)
			{
				m_fAttTime = 0.f;
				m_bRightAtt = true;
				m_bIsNormalAtt = true;

				if (FAILED(Setup_Layer_EnergyBolt(L"Layer_EnergyBolt")))
					return;

				// 생성해서 쏘면 됨
			}
		}
	}




}

void CPlayer::Skill_Laser(_float fDeltaTime)
{
	if (GetAsyncKeyState('X') & 0x8000)
	{
		m_bStartLaser = true;
		m_bUsingLaser = false;

		m_vInitialRot = m_pTransformCom[PART_HAND_RIGHT]->Get_Desc().vRotate;
		m_bUsingSkill = true;
		m_ePlayerSkillID = PLAYER_SKILL_LASER;
	}

	/*else if (m_bStartLaser)
	{*/
		if (m_bUsingLaser)
		{
			m_fAttTime += fDeltaTime;
			if (m_fAttTime >= 3.f)
			{
				m_bUsingLaser = false;
				m_bStartLaser = false;
				m_fAttTime = 0.f;

				m_pTransformCom[PART_HAND_RIGHT]->Set_Rotation(m_vInitialRot);
				m_bUsingSkill = false;
				m_ePlayerSkillID = PLAYER_SKILL_END;
			}
		}

		else
		{
			m_fAttTime += fDeltaTime;
			m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, -fDeltaTime * 5.f);
			//if (m_ePlayerDir == MOVING_UP)
			//	m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, -fDeltaTime * 5.f);
			//else if (m_ePlayerDir == MOVING_DOWN)
			//	m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, fDeltaTime * 5.f);
			//else if (m_ePlayerDir == MOVING_RIGHT)
			//	m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_Z, fDeltaTime * 5.f);

			if (m_fAttTime >= 0.2f)
			{
				m_fAttTime = 0.f;
				m_bUsingLaser = true;
			}
		}
	//}
}

void CPlayer::Skill_ProjectileFall(_float fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return;

	CVIBuffer_TerrainTexture* pTerrainBuffer = (CVIBuffer_TerrainTexture*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Terrain", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
		return;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Camera");

	if (pManagement->Key_Down(VK_RBUTTON))
	{
		m_bStartFall = true;
		m_bIsFall = false;
		m_bDownHand = false;

		m_vInitialRot = m_pTransformCom[PART_HAND_RIGHT]->Get_Desc().vRotate;
		//m_bUsingSkill = true;
		m_ePlayerSkillID = PLAYER_SKILL_FALL;
	}

	_matrix mat;
	D3DXMatrixIdentity(&mat);
	_vec3 vGoalPos = {};
	if (true == m_pRaycastCom->IsSimulate<VTX_TEXTURE, INDEX16>(
		g_hWnd, WINCX, WINCY, pTerrainBuffer, &mat, pCamera, &vGoalPos))
	{
		if (!m_bRenderInven && !m_bRenderShop)
		{	
			Ready_Layer_Meteor(L"Layer_Meteor", vGoalPos);

			for (_uint iCnt = 0; iCnt < 5; ++iCnt)
			{
				if (FAILED(Ready_Layer_Meteor(L"Layer_Meteor", vGoalPos)))
					PRINT_LOG(L"Failed To Ready_Layer_Meteor in CPlayer", LOG::CLIENT);
			}
			
		}
	}

	if (m_bIsFall)
	{
		m_fAttTime += fDeltaTime;

		if (m_fAttTime >= 1.f)
		{
			m_fAttTime = 0.f;
			m_bIsFall = false;
			m_bDownHand = true;
		}
	}

	else if (m_bDownHand)
	{
		m_fAttTime += fDeltaTime;
		m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, fDeltaTime * 5.f);
		//if (m_ePlayerDir == MOVING_UP)
		//	m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, fDeltaTime * 5.f);
		//else if (m_ePlayerDir == MOVING_DOWN)
		//	m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, -fDeltaTime * 5.f);
		//else if (m_ePlayerDir == MOVING_RIGHT)
		//	m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_Z, -fDeltaTime * 5.f);

		if (m_fAttTime >= 0.2f)
		{
			m_bStartFall = false;
			m_bIsFall = false;
			m_bDownHand = false;
			m_fAttTime = 0.f;

			m_pTransformCom[PART_HAND_RIGHT]->Set_Rotation(m_vInitialRot);
			m_bUsingSkill = false;
			m_ePlayerSkillID = PLAYER_SKILL_END;
		}
	}
	else
	{
		m_fAttTime += fDeltaTime;
		m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, -fDeltaTime * 5.f);
		//if (m_ePlayerDir == MOVING_UP)
		//	m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, -fDeltaTime * 5.f);
		//else if (m_ePlayerDir == MOVING_DOWN)
		//	m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, fDeltaTime * 5.f);
		//else if (m_ePlayerDir == MOVING_RIGHT)
		//	m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_Z, fDeltaTime * 5.f);

		if (m_fAttTime >= 0.4f)
		{
			m_bIsFall = true;
			m_fAttTime = 0.f;
		}
	}
	
}

void CPlayer::Buff_ManaDrift(_float fDeltaTime)
{
	m_fManaDriftTime += fDeltaTime;
	if (m_fManaDriftTime >= 60.f)
	{
		m_fManaDriftTime = 0.f;
		m_bActiveBuff[BUFF_MANA] = false;
		PRINT_LOG(L"마나 드리프트 끝", LOG::CLIENT);
		eSkillID = ACTIVE_SKILL_END;
	}
}

void CPlayer::Buff_EnergyExploitation(_float fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return;
	CEquip* pEquip = (CEquip*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_MainUI", 1);
	if (pEquip == nullptr)
		return;

	m_fEnergyExploitationSkill += fDeltaTime;

	if (m_fEnergyExploitationSkill >= 10.f)
	{
		m_fEnergyExploitationSkill = 0.f;
		m_bActiveBuff[BUFF_ATT] = false;
		pEquip->Set_PlayerAtt(-m_iAttBuff);
		PRINT_LOG(L"에너지 익스플로전 끝", LOG::CLIENT);
		eSkillID = ACTIVE_SKILL_END;
	}
}

HRESULT CPlayer::Ready_Layer_Meteor(const wstring& _strLayerTag, _vec3 vGoalPos)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	m_fRand[0] = (_float)(rand() % 2 - 4);
	m_fRand[1] = (_float)(rand() % 2 - 4);
	INSTANTIMPACT tImpact;
	tImpact.pAttacker = this;
	tImpact.pStatusComp = m_pStatusCom;
	tImpact.vPosition = vGoalPos + _vec3(m_fRand[0], 0.f, m_fRand[1]);

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE0, L"GameObject_Meteor", SCENE_STAGE0, _strLayerTag, &tImpact)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Setup_Layer_PlaneSkill(const wstring & LayerTag , _vec3 _vMouse)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE0, L"GameObject_PlanSkill", SCENE_STAGE0, LayerTag , &_vMouse)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Setup_Layer_EnergyBolt(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	INSTANTIMPACT tImpact;
	tImpact.pAttacker = this;
	tImpact.pStatusComp = m_pStatusCom;

	//CTransform* pWandTransform = (CTransform*)pManagement->Get_Component(pManagement->Get_CurrentSceneID(), L"Layer_Wand", L"Com_Transform2");
	_vec3 vWandPos = {};
	memcpy_s(&vWandPos , sizeof(_vec3),&m_pTransformCom[PART_HAND_RIGHT]->Get_Desc().matWorld._41,sizeof(_vec3));
	_vec3 vPlayerLook = m_pTransformCom[PART_HEAD]->Get_Look();
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);
	
	tImpact.vPosition = vWandPos;
	tImpact.vDirection = vPlayerLook;
	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE0, L"GameObject_EnergyBolt", SCENE_STAGE0, LayerTag , &tImpact)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Setup_Layer_Wand(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	_vec3 vPlayer_RightHand_Pos = m_pTransformCom[PART_HAND_RIGHT]->Get_Desc().vPosition;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE0, L"GameObject_Wand", SCENE_STAGE0, LayerTag , &vPlayer_RightHand_Pos)))
		return E_FAIL;

	return S_OK;
}
