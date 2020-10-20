#include "stdafx.h"
#include "Status.h"
#include "KeyManager.h"
#include "Inventory.h"
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

CPlayer::CPlayer(const CPlayer & _rOther)
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
	m_bUsingLaser = true;
	m_fLaserTime = 0.f;

	//---------------------------------------------
	// 스킬2 - 투사체 낙하
	//---------------------------------------------
	m_bStartFall = false;
	m_bIsFall = false;
	m_bDownHand = false;
	m_fFallTime = 0.f;

	return S_OK;
}

_int CPlayer::Update_GameObject(_float _fDeltaTime)
{
	if (m_bInitial)
		Initial_Update_GameObject();

	if (FAILED(Movement(_fDeltaTime)))
		return GAMEOBJECT::WARN;
	
	if (FAILED(Wnd_OpenClose()))
		return GAMEOBJECT::WARN;


	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (Update_Parts())
		return GAMEOBJECT::WARN;

	if (FAILED(m_pColliderCom->Update_Collider(m_pTransformCom[PART_BODY]->Get_Desc().vPosition)))
		return GAMEOBJECT::WARN;

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

HRESULT CPlayer::Take_Damage()
{
	return S_OK;
}

HRESULT CPlayer::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;

	for (_uint iCnt = 0; iCnt < PART_END; ++iCnt)
	{
		TCHAR szVIBuffer[MAX_STR] = L"";
		wsprintf(szVIBuffer, L"Com_VIBuffer%d", iCnt);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", szVIBuffer, (CComponent**)&m_pVIBufferCom[iCnt])))
			return E_FAIL;


		if (iCnt == PART_HEAD)
		{
			tTransformDesc.vPosition = { 5.f, 5.f, 5.f };
		}
		else if (iCnt == PART_BODY)
		{
			D3DXVECTOR3 vHeadPos = m_pTransformCom[PART_HEAD]->Get_Desc().vPosition;
			tTransformDesc.vPosition = { vHeadPos.x, vHeadPos.y - 1.5f, vHeadPos.z };
			tTransformDesc.vScale = { 0.9f, 0.9f, 0.9f };
		}
		else if (iCnt == PART_HAND_LEFT)
		{
			D3DXVECTOR3 vBodyPos = m_pTransformCom[PART_BODY]->Get_Desc().vPosition;
			tTransformDesc.vPosition
				= { vBodyPos.x - 0.8f, vBodyPos.y - 2.5f, vBodyPos.z };
			tTransformDesc.vScale = { 0.2f, 0.7f, 0.2f };
		}
		else if (iCnt == PART_HAND_RIGHT)
		{
			D3DXVECTOR3 vBodyPos = m_pTransformCom[PART_BODY]->Get_Desc().vPosition;
			tTransformDesc.vPosition
				= { vBodyPos.x + 0.8f, vBodyPos.y - 2.5f, vBodyPos.z };
			tTransformDesc.vScale = { 0.2f, 0.7f, 0.2f };
		}
		else if (iCnt == PART_FOOT_LEFT)
		{
			D3DXVECTOR3 vBodyPos = m_pTransformCom[PART_BODY]->Get_Desc().vPosition;
			tTransformDesc.vPosition = { vBodyPos.x - 0.2f, vBodyPos.y - 2.5f, vBodyPos.z };
			tTransformDesc.vScale = { 0.3f, 0.5f, 0.5f };
		}
		else if (iCnt == PART_FOOT_RIGHT)
		{
			D3DXVECTOR3 vBodyPos = m_pTransformCom[PART_BODY]->Get_Desc().vPosition;
			tTransformDesc.vPosition = { vBodyPos.x + 0.2f, vBodyPos.y - 2.5f, vBodyPos.z };
			tTransformDesc.vScale = { 0.3f, 0.5f, 0.5f };
		}
		tTransformDesc.fSpeedPerSecond = 10.f;
		tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);

		TCHAR szTransform[MAX_STR] = L"";
		wsprintf(szTransform, L"Com_Transform%d", iCnt);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", szTransform, (CComponent**)&m_pTransformCom[iCnt], &tTransformDesc)))
			return E_FAIL;

		// For.Com_Texture
		TCHAR szTexture[MAX_STR] = L"", szTextureName[MAX_STR] = L"";
		wsprintf(szTexture, L"Com_Texture%d", iCnt);
		if (iCnt == PART_HEAD)
			wsprintf(szTextureName, L"Component_Texture_Monster");
		else if (iCnt == PART_BODY)
			wsprintf(szTextureName, L"Component_Texture_PlayerHead");
		else if (iCnt == PART_HAND_LEFT || iCnt == PART_HAND_RIGHT)
			wsprintf(szTextureName, L"Component_Texture_PlayerHand");
		else
			wsprintf(szTextureName, L"Component_Texture_PlayerFoot");

		if (FAILED(CGameObject::Add_Component(SCENE_STAGE0, szTextureName, szTexture, (CComponent**)&m_pTextureCom[iCnt])))
			return E_FAIL;
	}

	// For.Com_Picking
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Raycast", L"Com_Ray", (CComponent**)&m_pRaycastCom)))
		return E_FAIL;

	// For.Com_Stat
	CStatus::STAT tStat;
	tStat.iCriticalRate = 20;	tStat.iCriticalHit = 10;
	tStat.iDef = 50;
	tStat.iHp = 100;			tStat.iMp = 100;
	tStat.iMinAtt = 10;			tStat.iMaxAtt = 50;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Status", L"Com_Stat", (CComponent**)&m_pStatusCom, &tStat)))
		return E_FAIL;

	// For.ItemMgr
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_ItemManagement", L"Com_ItemMgr", (CComponent**)&m_pItemMgrCom)))
		return E_FAIL;

	CCollider::COLLIDER_DESC tColDesc;
	tColDesc.vPosition = tTransformDesc.vPosition;
	tColDesc.fRadius = 0.7f;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider", L"Com_Collider", (CComponent**)&m_pColliderCom, &tColDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Movement(_float _fDeltaTime)
{
	m_bMove = false;
	if (!m_bUsingSkill)
	{
		Move_Vertical(_fDeltaTime);
		Move_Horizontal(_fDeltaTime);
		Turn(_fDeltaTime);

		Normal_Attack(_fDeltaTime);
		Skill_Laser(_fDeltaTime);
		Skill_ProjectileFall(_fDeltaTime);

		MoveMotion(_fDeltaTime);
	}

	else
	{
		switch (m_ePlayerSkillID)
		{
		case PLAYER_NORMAL_ATTACK:
			Normal_Attack(_fDeltaTime);
			break;
		case PLAYER_SKILL_LASER:
			Skill_Laser(_fDeltaTime);
			break;
		case PLAYER_SKILL_FALL:
			Skill_ProjectileFall(_fDeltaTime);
			break;
		case PLAYER_SKILL_END:
			break;
		}
	}

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
		D3DXVECTOR3 vPosition = m_pTransformCom[iCnt]->Get_Desc().vPosition;
		if (true == pTerrainBuffer->IsOnTerrain(&vPosition))
		{
			m_pTransformCom[iCnt]->Set_Position(vPosition);
		}
	}

	float fLeftFootPosY = m_pTransformCom[PART_FOOT_LEFT]->Get_Desc().vPosition.y;
	// 변화한 다리의 위치에 따라 몸의 위치도 변경해준다
	D3DXVECTOR3 vBodyPos = m_pTransformCom[PART_BODY]->Get_Desc().vPosition;
	vBodyPos.y = fLeftFootPosY + 1.f;
	m_pTransformCom[PART_BODY]->Set_Position(vBodyPos);

	D3DXVECTOR3 vHeadPos = m_pTransformCom[PART_HEAD]->Get_Desc().vPosition;
	vHeadPos.y = vBodyPos.y + 0.5f;
	m_pTransformCom[PART_HEAD]->Set_Position(vHeadPos);

	D3DXVECTOR3 vLeftHandPos = m_pTransformCom[PART_HAND_LEFT]->Get_Desc().vPosition;
	vLeftHandPos.y = fLeftFootPosY + 1.f;
	m_pTransformCom[PART_HAND_LEFT]->Set_Position(vLeftHandPos);

	D3DXVECTOR3 vRightHandPos = m_pTransformCom[PART_HAND_RIGHT]->Get_Desc().vPosition;
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
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);

	// 반환받을 마우스 위치값
	D3DXVECTOR3 vOutPos;

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		if (m_pRaycastCom->IsSimulate<VTX_TEXTURE, INDEX16>(
			g_hWnd, WINCX, WINCY, pTerrainBuffer, &mat, pCamera, &vOutPos))
		{

		}
	}

	return S_OK;
}

void CPlayer::Move_Vertical(_float _fDeltaTime)
{
	if (CKeyManager::Get_Instance()->Key_Pressing(VK_UP))
	{
		m_ePlayerDir = MOVING_UP;
		if (!m_bUpTurn)
		{
			for (_uint i = 0; i < 2; ++i)
			{
				D3DXVECTOR3 vRot = m_pTransformCom[i]->Get_Desc().vRotate;
				vRot.y = 0.f;
				m_pTransformCom[i]->Set_Rotation(vRot);
			}
			m_bUpTurn = true;
		}

		// 위로 이동
		for (_uint i = 0; i < PART_END; ++i)
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

	else if (CKeyManager::Get_Instance()->Key_Pressing(VK_DOWN))
	{
		m_ePlayerDir = MOVING_DOWN;
		if (!m_bDownTurn)
		{
			for (_uint i = 0; i < 2; ++i)
			{

				D3DXVECTOR3 vRot = m_pTransformCom[i]->Get_Desc().vRotate;
				vRot.y = -3.f;
				m_pTransformCom[i]->Set_Rotation(vRot);
			}
			m_bDownTurn = true;
		}

		for (_uint i = 0; i < PART_END; ++i)
			m_pTransformCom[i]->Move_Vertical(_fDeltaTime);

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
	if (CKeyManager::Get_Instance()->Key_Pressing(VK_LEFT))
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

		for (_uint i = 0; i < PART_END; ++i)
			m_pTransformCom[i]->Move_Vertical(_fDeltaTime);

		m_bMove = true;
		if (m_bRightTurn)
			m_bRightTurn = false;
		if (m_bDownTurn)
			m_bDownTurn = false;
		if (m_bUpTurn)
			m_bUpTurn = false;
	}

	else if (CKeyManager::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		m_ePlayerDir = MOVING_RIGHT;
		if (!m_bRightTurn)
		{
			for (_uint i = 0; i < 2; ++i)
			{
				D3DXVECTOR3 vRot = m_pTransformCom[i]->Get_Desc().vRotate;
				vRot.y = 1.57079637f;
				m_pTransformCom[i]->Set_Rotation(vRot);
			}
			m_bRightTurn = true;
		}

		for (_uint i = 0; i < PART_END; ++i)
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
	if (CKeyManager::Get_Instance()->Key_Pressing('Q'))
	{
		for (_uint i = 0; i < 6; ++i)
			m_pTransformCom[i]->Turn(CTransform::AXIS_Y, -_fDeltaTime);
	}

	if (CKeyManager::Get_Instance()->Key_Pressing('E'))
	{
		for (_uint i = 0; i < 6; ++i)
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
}

_int CPlayer::Update_Parts()
{
	_vec3 vLook = {};
	_vec3 vPos = {};
	_vec3 vRight = {};
	_vec3 vLeft = {};
	_vec3 vRealUp = {};
	vPos = m_pTransformCom[PART_BODY]->Get_Desc().vPosition;
	memcpy_s(&vLook, sizeof(_vec3), &m_pTransformCom[PART_BODY]->Get_Desc().matWorld.m[2][0], sizeof(_vec3));

	D3DXVec3Cross(&vRight, &_vec3(0.f, 1.f, 0.f), &vLook);
	D3DXVec3Cross(&vLeft, &vLook, &_vec3(0.f, 1.f, 0.f));
	D3DXVec3Cross(&vRealUp, &vLook, &vRight);

	// 오른쪽 손
	_vec3 vRightHand = vPos + (vRight * m_fHandDis);
	m_pTransformCom[PART_HAND_RIGHT]->Set_Position(vRightHand);
	// 왼쪽 손
	_vec3 vLeftHand = vPos + (vLeft * m_fHandDis);
	m_pTransformCom[PART_HAND_LEFT]->Set_Position(vPos + (vLeft * m_fHandDis));
	// 오른쪽 발
	_vec3 vRightFoot = vPos + (vRight * 0.3f);
	vRightFoot.y = vPos.y - 1.f;
	m_pTransformCom[PART_FOOT_RIGHT]->Set_Position(vRightFoot);
	// 왼쪽 발
	_vec3 vLeftFoot = vPos + (vLeft * 0.3f);
	vLeftFoot.y = vPos.y - 1.f;
	m_pTransformCom[PART_FOOT_LEFT]->Set_Position(vLeftFoot);

	for (_uint iCnt = 0; iCnt < PART_END; ++iCnt)
	{
		if (FAILED(m_pTransformCom[iCnt]->Update_Transform()))
			return GAMEOBJECT::WARN;
	}

	return GAMEOBJECT::NOEVENT;
}

void CPlayer::Jump(float fDeltaTime)
{
	int k = 0;
	if (CKeyManager::Get_Instance()->Key_Pressing(VK_SPACE))
	{
		m_bJump = true;
	}

	D3DXVECTOR3 vCurPos[PART_END];
	// 플레이어의 현재 위치를 계속 받아온다
	for (_uint i = 0; i < PART_END; ++i)
		vCurPos[i] = m_pTransformCom[i]->Get_Desc().vPosition;

	if (m_bJump)
	{
		// 시간 더해주기
		m_fJumpTime += fDeltaTime;
		for (_uint i = 0; i < PART_END; ++i)
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

void CPlayer::MoveMotion(float fDeltaTime)
{
	if (m_bMove)
	{
		m_fMovingTime += fDeltaTime;

		if (m_fMovingTime >= 1.f/*0.5f*/)
		{
			if (m_eMovingDir == CHANGE_LEFT)
				m_eMovingDir = CHANGE_RIGHT;
			else if (m_eMovingDir == CHANGE_RIGHT)
				m_eMovingDir = CHANGE_LEFT;
			m_fMovingTime = 0.f;

			for (_uint i = 2; i < PART_END; ++i)
			{
				m_pTransformCom[i]->Set_Rotation(m_vConstRot[i]);
			}
		}

		if (m_eMovingDir == CHANGE_LEFT)
		{

			if (m_ePlayerDir == MOVING_UP || m_ePlayerDir == MOVING_DOWN)
			{
				m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, -fDeltaTime);
				m_pTransformCom[PART_HAND_LEFT]->Turn(CTransform::AXIS_X, fDeltaTime);
			}
			else if (m_ePlayerDir == MOVING_LEFT || m_ePlayerDir == MOVING_RIGHT)
			{
				m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_Z, -fDeltaTime);
				m_pTransformCom[PART_HAND_LEFT]->Turn(CTransform::AXIS_Z, fDeltaTime);
			}
		}

		else if (m_eMovingDir == CHANGE_RIGHT)
		{	
			if (m_ePlayerDir == MOVING_UP || m_ePlayerDir == MOVING_DOWN)
			{
				m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, fDeltaTime);
				m_pTransformCom[PART_HAND_LEFT]->Turn(CTransform::AXIS_X, -fDeltaTime);
			}
			else if (m_ePlayerDir == MOVING_LEFT || m_ePlayerDir == MOVING_RIGHT)
			{
				m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_Z, fDeltaTime);
				m_pTransformCom[PART_HAND_LEFT]->Turn(CTransform::AXIS_Z, -fDeltaTime);
			}
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

	if (CKeyManager::Get_Instance()->Key_Pressing('G'))
	{
		// 일단은 그냥 아이템 먹는거로.. -> 나중에 수정
		list<INVEN_ITEM*> pInvenList = m_pItemMgrCom->Get_DropItem(L"금 검", 1, 200);
		//PRINT_LOG(L"검1 획득!");

		CInventory* pInven = (CInventory*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Inven");
		if (nullptr == pInven)
			return E_FAIL;

		pInven->Set_InvenItemList(&pInvenList);
	}

	return S_OK;
}

HRESULT CPlayer::Wnd_OpenClose()
{
	return E_NOTIMPL;
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

	Safe_Release(m_pItemMgrCom);
	Safe_Release(m_pItemMgrCom);
	Safe_Release(m_pRaycastCom);
	Safe_Release(m_pColliderCom);

	CGameObject::Free();
}

void CPlayer::Normal_Attack(float fDeltaTime)
{
	int k = 0;
	if (GetAsyncKeyState('Z') & 0x8000)
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

			if (m_ePlayerDir == MOVING_UP || m_ePlayerDir == MOVING_DOWN)
				m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_Y, fDeltaTime * 2.3f);
			else if (m_ePlayerDir == MOVING_RIGHT)
				m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, fDeltaTime * 2.3f);
		}

		else if (m_bLeftAtt)
		{
			m_fAttTime += fDeltaTime;
			if (m_ePlayerDir == MOVING_UP || m_ePlayerDir == MOVING_DOWN)
				m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_Y, -fDeltaTime * 2.3f);
			else if (m_ePlayerDir == MOVING_RIGHT)
				m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, -fDeltaTime * 2.3f);

			if (m_fAttTime >= 0.6f)
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
			if (m_ePlayerDir == MOVING_UP)
				m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, -fDeltaTime * 2.5f);
			else if (m_ePlayerDir == MOVING_DOWN)
				m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, fDeltaTime * 2.5f);
			else if (m_ePlayerDir == MOVING_RIGHT)
				m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_Z, fDeltaTime * 2.5f);
			//m_pTransformCom[PLAYER_RIGHT_HAND]->Turn(CTransform::AXIS_Y, -fDeltaTime * 0.35f);

			if (m_fAttTime >= 0.4f)
			{
				m_fAttTime = 0.f;
				m_bRightAtt = true;
				m_bIsNormalAtt = true;
			}
		}
	}
}

void CPlayer::Skill_Laser(float fDeltaTime)
{
	if (GetAsyncKeyState('X') & 0x8000)
	{
		m_bStartLaser = true;
		m_bUsingLaser = false;

		m_vInitialRot = m_pTransformCom[PART_HAND_RIGHT]->Get_Desc().vRotate;
		m_bUsingSkill = true;
		m_ePlayerSkillID = PLAYER_SKILL_LASER;
	}

	else if (m_bStartLaser)
	{
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
			if (m_ePlayerDir == MOVING_UP)
				m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, -fDeltaTime * 2.7f);
			else if (m_ePlayerDir == MOVING_DOWN)
				m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, fDeltaTime * 2.7f);
			else if (m_ePlayerDir == MOVING_RIGHT)
				m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_Z, fDeltaTime * 2.7f);

			if (m_fAttTime >= 0.4f)
			{
				m_fAttTime = 0.f;
				m_bUsingLaser = true;
			}
		}
	}
}

void CPlayer::Skill_ProjectileFall(float fDeltaTime)
{
	if (GetAsyncKeyState('C') & 0x8000)
	{
		m_bStartFall = true;
		m_bIsFall = false;
		m_bDownHand = false;

		m_vInitialRot = m_pTransformCom[PART_HAND_RIGHT]->Get_Desc().vRotate;
		m_bUsingSkill = true;
		m_ePlayerSkillID = PLAYER_SKILL_FALL;
	}

	else if (m_bStartFall)
	{
		if (m_bIsFall)
		{
			m_fAttTime += fDeltaTime;

			if (m_fAttTime >= 2.f)
			{
				m_fAttTime = 0.f;
				m_bIsFall = false;
				m_bDownHand = true;
			}
		}

		else if (m_bDownHand)
		{
			m_fAttTime += fDeltaTime;
			if (m_ePlayerDir == MOVING_UP)
				m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, fDeltaTime * 2.5f);
			else if (m_ePlayerDir == MOVING_DOWN)
				m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, -fDeltaTime * 2.5f);
			else if (m_ePlayerDir == MOVING_RIGHT)
				m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_Z, -fDeltaTime * 2.5f);

			if (m_fAttTime >= 0.5f)
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
			if (m_ePlayerDir == MOVING_UP)
				m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, -fDeltaTime * 3.f);
			else if (m_ePlayerDir == MOVING_DOWN)
				m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_X, fDeltaTime * 3.f);
			else if (m_ePlayerDir == MOVING_RIGHT)
				m_pTransformCom[PART_HAND_RIGHT]->Turn(CTransform::AXIS_Z, fDeltaTime * 3.f);

			if (m_fAttTime >= 0.4f)
			{
				m_bIsFall = true;
				m_fAttTime = 0.f;
			}
		}
	}
}

HRESULT CPlayer::Draw_HpBar()
{
	return E_NOTIMPL;
}
