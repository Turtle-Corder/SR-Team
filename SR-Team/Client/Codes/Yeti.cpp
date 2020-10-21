#include "stdafx.h"
#include "..\Headers\Yeti.h"
#include "Status.h"

USING(Client)

CYeti::CYeti(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
	for (int iAll = 0; iAll < YETI_END; ++iAll)
	{
		m_pTransformCom[iAll] = nullptr;
		m_pVIBufferCom[iAll] = nullptr;
		m_pTextureCom[iAll] = nullptr;
	}
}

CYeti::CYeti(const CYeti & other)
	: CGameObject(other)
{
}

HRESULT CYeti::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CYeti::Setup_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

int CYeti::Update_GameObject(float _fDeltaTime)
{
	if (FAILED(Movement(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	if (FAILED(Setting_Part()))
		return GAMEOBJECT::WARN;

	if (FAILED(Attack(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	for (int iAll = 0; iAll < YETI_END; ++iAll)
	{
		if (FAILED(m_pTransformCom[iAll]->Update_Transform()))
			return GAMEOBJECT::WARN;
	}

	return GAMEOBJECT::NOEVENT;
}

int CYeti::LateUpdate_GameObject(float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CYeti::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	for (int iAll = 0; iAll < YETI_END; ++iAll)
	{
		if (FAILED(m_pVIBufferCom[iAll]->Set_Transform(&m_pTransformCom[iAll]->Get_Desc().matWorld, pCamera)))
			return E_FAIL;

		if (FAILED(m_pTextureCom[iAll]->SetTexture(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom[iAll]->Render_VIBuffer()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CYeti::Add_Component()
{
	TCHAR szName[MAX_PATH] = L"";
	TCHAR szPartName[MAX_PATH] = L"";
	_vec3 vBodyPos = {};

	// For.Com_Texture
	for (int iAll = 0; iAll < YETI_END; ++iAll)
	{
		StringCchPrintf(szName, sizeof(TCHAR) * MAX_PATH, L"Com_VIBuffer%d", iAll);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", szName, (CComponent**)&m_pVIBufferCom[iAll]))) //생성 갯수
			return E_FAIL;

		//경우마다 그거에 맞게 복사해서 최종적으로 문자열 들어가게만들기
		if (iAll == YETI_BODY)
		{
			StringCchPrintf(szPartName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_YetiBody");
		}
		else if (iAll == YETI_HEAD)
		{
			StringCchPrintf(szPartName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_YetiHead");
		}
		else if (iAll == YETI_LEFT)
		{
			StringCchPrintf(szPartName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_YetiLeft");
		}
		else if (iAll == YETI_RIGHT)
		{
			StringCchPrintf(szPartName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_YetiRight");
		}
		else if (iAll == YETI_LEFTLEG)
		{
			StringCchPrintf(szPartName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_YetiLeftLeg");
		}
		else if (iAll == YETI_RIGHTLEG)
		{
			StringCchPrintf(szPartName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_YetiRightLeg");
		}
		StringCchPrintf(szName, sizeof(TCHAR) * MAX_PATH, L"Com_Texture%d", iAll);

		if (FAILED(CGameObject::Add_Component(SCENE_STAGE0, szPartName, szName, (CComponent**)&m_pTextureCom[iAll]))) ////생성 갯수
			return E_FAIL;

		CTransform::TRANSFORM_DESC tTransformDesc[YETI_END];

		if (iAll == YETI_BODY)
		{
			tTransformDesc[YETI_BODY].vPosition = { 20.f, 0.1f, 20.f };
			tTransformDesc[YETI_BODY].fSpeedPerSecond = 10.f;
			tTransformDesc[YETI_BODY].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[YETI_BODY].vScale = { 1.0f , 1.0f , 1.0f };
			vBodyPos = tTransformDesc[YETI_BODY].vPosition;
		}
		else if (iAll == YETI_HEAD)
		{
			tTransformDesc[YETI_HEAD].vPosition = { vBodyPos.x, vBodyPos.y - 1.f, vBodyPos.z };
			tTransformDesc[YETI_HEAD].fSpeedPerSecond = 10.f;
			tTransformDesc[YETI_HEAD].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[YETI_HEAD].vScale = { 0.5f , 0.5f , 0.5f };
		}
		else if (iAll == YETI_LEFT)
		{
			tTransformDesc[YETI_LEFT].vPosition = { vBodyPos.x - 1.f, vBodyPos.y, vBodyPos.z };
			tTransformDesc[YETI_LEFT].fSpeedPerSecond = 10.f;
			tTransformDesc[YETI_LEFT].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[YETI_LEFT].vScale = { 0.4f , 0.6f , 0.4f };
		}
		else if (iAll == YETI_RIGHT)
		{
			tTransformDesc[YETI_RIGHT].vPosition = { vBodyPos.x + 1.f, vBodyPos.y, vBodyPos.z };
			tTransformDesc[YETI_RIGHT].fSpeedPerSecond = 10.f;
			tTransformDesc[YETI_RIGHT].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[YETI_RIGHT].vScale = { 0.4f , 0.6f , 0.4f };
		}
		else if (iAll == YETI_LEFTLEG)
		{
			tTransformDesc[YETI_LEFTLEG].vPosition = { vBodyPos.x - 0.1f, vBodyPos.y, vBodyPos.z };
			tTransformDesc[YETI_LEFTLEG].fSpeedPerSecond = 10.f;
			tTransformDesc[YETI_LEFTLEG].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[YETI_LEFTLEG].vScale = { 0.3f , 0.7f , 0.3f };
		}
		else if (iAll == YETI_RIGHTLEG)
		{
			tTransformDesc[YETI_RIGHTLEG].vPosition = { vBodyPos.x + 0.1f, vBodyPos.y, vBodyPos.z };
			tTransformDesc[YETI_RIGHTLEG].fSpeedPerSecond = 10.f;
			tTransformDesc[YETI_RIGHTLEG].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[YETI_RIGHTLEG].vScale = { 0.3f , 0.7f , 0.3f };
		}


		StringCchPrintf(szName, sizeof(TCHAR) * MAX_PATH, L"Com_Transform%d", iAll);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", szName, (CComponent**)&m_pTransformCom[iAll], &tTransformDesc[iAll]))) ////생성 갯수
			return E_FAIL;

	}
		CStatus::STAT tStat;
		tStat.iCriticalRate = 20;	tStat.iCriticalHit = 10;
		tStat.iDef = 50;
		tStat.iHp = 100;			tStat.iMp = 100;
		tStat.iMinAtt = 10;			tStat.iMaxAtt = 50;

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Status", L"Com_Stat", (CComponent**)&m_pStatusCom, &tStat)))
			return E_FAIL;

	return S_OK;
}

HRESULT CYeti::Movement(float _fDeltaTime)
{
	if (FAILED(IsOnTerrain()))
		return E_FAIL;

	if (FAILED(Moving(_fDeltaTime)))
		return E_FAIL;

	return S_OK;
}

HRESULT CYeti::IsOnTerrain()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CVIBuffer_TerrainTexture* pTerrainBuffer = (CVIBuffer_TerrainTexture*)pManagement->Get_Component(SCENE_STAGE0, L"Layer_Terrain", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	D3DXVECTOR3 vLeftLegPos = m_pTransformCom[YETI_LEFTLEG]->Get_Desc().vPosition;

	if (true == pTerrainBuffer->IsOnTerrain(&vLeftLegPos))
	{
		m_pTransformCom[YETI_LEFTLEG]->Set_Position(vLeftLegPos);
	}

	D3DXVECTOR3 vRightLegPos = m_pTransformCom[YETI_RIGHTLEG]->Get_Desc().vPosition;

	if (true == pTerrainBuffer->IsOnTerrain(&vRightLegPos))
	{
		m_pTransformCom[YETI_RIGHTLEG]->Set_Position(vRightLegPos);
	}

	return S_OK;
}

HRESULT CYeti::Setting_Part()
{
	D3DXVECTOR3 vPosition[2];

	_vec3 vBodyLook = {};
	_vec3 vTransPos = {};
	_vec3 vBodyPos = m_pTransformCom[YETI_BODY]->Get_Desc().vPosition;

	float fLegY = m_pTransformCom[YETI_LEFTLEG]->Get_Desc().vPosition.y;

	memcpy_s(&vBodyLook, sizeof(_vec3), &m_pTransformCom[YETI_BODY]->Get_Desc().matWorld._31, sizeof(_vec3));

	_vec3 vMonRight = {};
	_vec3 vMonLeft = {};
	float fDis = 1.1f;
	D3DXVec3Cross(&vMonRight, &_vec3(0.f, 1.f, 0.f), &vBodyLook);
	D3DXVec3Cross(&vMonLeft, &vBodyLook, &_vec3(0.f, 1.f, 0.f));

	for (int iAll = 0; iAll < YETI_END; ++iAll)
	{
		if (iAll == YETI_BODY)
		{
			/*vTransPos = { vBodyPos.x , m_pTransformCom[YETI_LEFTLEG]->Get_Desc().vPosition.y + 0.6f , vBodyPos.z };*/
			vTransPos = vBodyPos;
			vTransPos.y = fLegY + 0.6f;
			m_pTransformCom[iAll]->Set_Position(vTransPos);
		}
		else if (iAll == YETI_HEAD)
		{
			//vTransPos = { vBodyPos.x , m_pTransformCom[YETI_LEFTLEG]->Get_Desc().vPosition.y + 1.2f , vBodyPos.z };
			vTransPos = vBodyPos;
			vTransPos.y = fLegY + 1.2f;
			m_pTransformCom[iAll]->Set_Position(vTransPos);
		}
		if (iAll == YETI_LEFT)
		{
			//vTransPos = { vBodyPos.x - 0.8f , m_pTransformCom[YETI_LEFTLEG]->Get_Desc().vPosition.y + 0.8f , vBodyPos.z };
			vTransPos = vBodyPos + vMonLeft * fDis;
			vTransPos.y = fLegY + 0.8f;
			m_pTransformCom[iAll]->Set_Position(vTransPos);
		}
		else if (iAll == YETI_RIGHT)
		{
			/*	vTransPos = { vBodyPos.x + 0.8f, m_pTransformCom[YETI_LEFTLEG]->Get_Desc().vPosition.y + 0.8f , vBodyPos.z };*/
			vTransPos = vBodyPos + vMonRight * fDis;
			vTransPos.y = fLegY + 0.8f;
			m_pTransformCom[iAll]->Set_Position(vTransPos);
		}
		else if (iAll == YETI_LEFTLEG)
		{
			//vTransPos = { vBodyPos.x - 0.4f, m_pTransformCom[YETI_LEFTLEG]->Get_Desc().vPosition.y , vBodyPos.z };
			vTransPos = vBodyPos + vMonLeft * 0.4f;
			vTransPos.y = fLegY;
			m_pTransformCom[iAll]->Set_Position(vTransPos);
		}
		else if (iAll == YETI_RIGHTLEG)
		{
			//vTransPos = { vBodyPos.x + 0.4f, m_pTransformCom[YETI_LEFTLEG]->Get_Desc().vPosition.y , vBodyPos.z };
			vTransPos = vBodyPos + vMonRight * 0.4f;
			vTransPos.y = fLegY;
			m_pTransformCom[iAll]->Set_Position(vTransPos);
		}
	}

	return S_OK;
}

HRESULT CYeti::Moving(float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(SCENE_STAGE0, L"Layer_Player", L"Com_Transform0");

	if (nullptr == pPlayerTransform)
		return E_FAIL;

	_vec3 vMonsterPos = m_pTransformCom[YETI_BODY]->Get_Desc().vPosition;
	_vec3 vPlayerPos = pPlayerTransform->Get_Desc().vPosition;

	_vec3 vDir = vPlayerPos - vMonsterPos;

	float fLength = D3DXVec3Length(&vDir); // 플레이어와의 거리비교

	if (fLength < 4.f)
	{
		LookAtPlayer(_fDeltaTime);

		if (!m_bAttackDelay)
		{
			m_bAttack = true;
		}
	}

	return S_OK;
}

HRESULT CYeti::Attack(float _fDeltaTime)
{
	_vec3 vRot = m_pTransformCom[YETI_RIGHT]->Get_Desc().vRotate;

	if (m_bAttack)
	{
		m_bAttackDelay = true;
		m_fAttackTime += _fDeltaTime;

		if (m_fAttackTime < 0.4f)
		{
			m_pTransformCom[YETI_RIGHT]->Turn(CTransform::AXIS_X, -_fDeltaTime * 3.f);
			m_bRHandDown = true;
		}
		else if (m_fAttackTime > 0.4f && m_bRHandDown)
		{
			if (FAILED(Create_Snow(L"Layer_Snow")))
				return E_FAIL;
			m_fAttackTime = 0.f;
			m_bHighestCheck = true;
			m_bAttack = false;
		}
	}
	else if (m_bHighestCheck)
	{
		m_fAttackTime += _fDeltaTime;

		if (m_fAttackTime < 0.4f)
		{
			m_pTransformCom[YETI_RIGHT]->Turn(CTransform::AXIS_X, +_fDeltaTime * 3.f);
		}
		else if (m_fAttackTime > 0.4f)
		{
			m_bAttackDelay = false;
			m_bHighestCheck = false;
			m_fAttackTime = 0.f;
		}
	}
	//else if(!m_bAttack)
	//{
	//	m_pTransformCom[YETI_RIGHT]->Set_Rotate(vRot);
	//}
	// 최종적으로다끝나서 돌아오면 m_bAttackDelay = false;
	return S_OK;
}

CYeti * CYeti::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CYeti* pInstance = new CYeti(pDevice);

	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CYeti", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CYeti::Clone_GameObject(void * pArg)
{
	CYeti* pInstance = new CYeti(*this);

	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CYeti", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CYeti::Free()
{
	for (int iAll = 0; iAll < YETI_END; ++iAll)
	{
		Safe_Release(m_pTransformCom[iAll]);
		Safe_Release(m_pVIBufferCom[iAll]);
		Safe_Release(m_pTextureCom[iAll]);
	}

	CGameObject::Free();
}

HRESULT CYeti::LookAtPlayer(float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(SCENE_STAGE0, L"Layer_Player", L"Com_Transform0");

	if (nullptr == pPlayerTransform)
		return E_FAIL;
	//--------------------------------------------------
	// 플레이어와 this => Pos
	//-------------------------------------------------- 
	_vec3 vPlayerPos = pPlayerTransform->Get_Desc().vPosition;
	_vec3 vMonPos = m_pTransformCom[YETI_BODY]->Get_Desc().vPosition;
	_vec3 vMonLook = {};

	////--------------------------------------------------
	//// Look 과 목적지 - 출발지를 내적
	////--------------------------------------------------

	memcpy_s(&vMonLook, sizeof(_vec3), &m_pTransformCom[YETI_BODY]->Get_Desc().matWorld._31, sizeof(_vec3));

	_vec3 vMonToPlayer = vPlayerPos - vMonPos;

	D3DXVec3Normalize(&vMonLook, &vMonLook);
	D3DXVec3Normalize(&vMonToPlayer, &vMonToPlayer);

	float fDot = 0.f;
	float fRad = 0.f;

	fDot = D3DXVec3Dot(&vMonLook, &vMonToPlayer);
	fRad = (float)acos(fDot);

	_vec3 vMonRight = {};
	D3DXVec3Cross(&vMonRight, &vMonLook, &_vec3(0.f, 1.f, 0.f));

	D3DXVec3Dot(&vMonRight, &vMonToPlayer);

	float fLimit = D3DXVec3Dot(&vMonRight, &vMonToPlayer);

	if (fabsf(fLimit) < 0.2f)
		return S_OK;


	CTransform* pMonsterTransform = (CTransform*)pManagement->Get_Component(SCENE_STAGE0, L"Layer_Monster", L"Com_Transform");

	if (nullptr == pMonsterTransform)
		return E_FAIL;

	if (fLimit > 0)
	{
		m_pTransformCom[YETI_BODY]->Turn(CTransform::AXIS_Y, -_fDeltaTime * fRad);
		m_pTransformCom[YETI_HEAD]->Turn(CTransform::AXIS_Y, -_fDeltaTime * fRad);
		m_pTransformCom[YETI_LEFT]->Turn(CTransform::AXIS_Y, -_fDeltaTime * fRad);
		m_pTransformCom[YETI_RIGHT]->Turn(CTransform::AXIS_Y, -_fDeltaTime * fRad);
	}
	else
	{
		/*for (int i = 0; i < YETI_END; ++i)
		{*/
		m_pTransformCom[YETI_BODY]->Turn(CTransform::AXIS_Y, _fDeltaTime * fRad);
		m_pTransformCom[YETI_HEAD]->Turn(CTransform::AXIS_Y, _fDeltaTime * fRad);
		m_pTransformCom[YETI_LEFT]->Turn(CTransform::AXIS_Y, _fDeltaTime * fRad);
		m_pTransformCom[YETI_RIGHT]->Turn(CTransform::AXIS_Y, _fDeltaTime * fRad);
		//}

	}
	//------------------------------------------
	// 공전 구현 순서생각하기 LateUpdate안에 월드구성하고 나서
	//------------------------------------------
	/*	D3DXMATRIX matTrans , matRevY , matParent;
	D3DXMatrixTranslation(&matTrans, 2.f, 0.f, 2.f);
	D3DXMatrixRotationY(&matRevY, D3DXToRadian(m_fAngle));
	D3DXMatrixTranslation(&matParent, vMonPos.x, vMonPos.y, vMonPos.z);
	m_pTransformCom->Set_WorldMatrix(matTrans * matRevY * matParent);*/
	//------------------------------------------
	return S_OK;
}

HRESULT CYeti::Create_Snow(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	_vec3 vRightHand = m_pTransformCom[YETI_RIGHT]->Get_Desc().vPosition;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE0, L"GameObject_Snow",
		SCENE_STAGE0, LayerTag, &m_pOwner)))
		return E_FAIL;

	return S_OK;
}
