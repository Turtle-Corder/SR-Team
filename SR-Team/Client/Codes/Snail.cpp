#include "stdafx.h"
#include "..\Headers\Snail.h"

USING(Client)

CSnail::CSnail(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
	for (_int iAll = 0; iAll < SNAIL_END; ++iAll)
	{
		m_pTransformCom[iAll] = nullptr;
		m_pVIBufferCom[iAll] = nullptr;
		m_pTextureCom[iAll] = nullptr;
	}
}

CSnail::CSnail(const CSnail& _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CSnail::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CSnail::Setup_GameObject(void* pArg)
{
	if (pArg)
		m_vStartPos = *(_vec3*)pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

int CSnail::Update_GameObject(_float _fDeltaTime)
{
	if (FAILED(Movement(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	for (_int iAll = 0; iAll < SNAIL_END; ++iAll)
	{
		if (FAILED(m_pTransformCom[iAll]->Update_Transform()))
			return 0;
	}

	if (FAILED(Setting_Part()))
		return 0;

	return GAMEOBJECT::NOEVENT;
}

int CSnail::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;


	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return 0;

	return 0;
}

HRESULT CSnail::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	for (_int iAll = 0; iAll < SNAIL_END; ++iAll)
	{
		if (FAILED(m_pVIBufferCom[iAll]->Set_Transform(&m_pTransformCom[iAll]->Get_Desc().matWorld, pCamera)))
			return E_FAIL;

		if (FAILED(m_pTextureCom[iAll]->SetTexture(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom[iAll]->Render_VIBuffer()))
			return E_FAIL;
	}
	/*
	
		if (FAILED(SetUp_Layer_InstantImpact(L"Layer_Instant_Impact")))
		return E_FAIL;
	*/
	return S_OK;
}

HRESULT CSnail::Add_Component()
{
	// For.Com_VIBuffer
	TCHAR szName[MAX_PATH] = L"";
	TCHAR szPartName[MAX_PATH] = L"";
	_vec3 vHeadPos = {};

	// For.Com_Texture
	for (_int iAll = 0; iAll < SNAIL_END; ++iAll)
	{
		StringCchPrintf(szName, sizeof(TCHAR) * MAX_PATH, L"Com_VIBuffer%d", iAll);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", szName, (CComponent**)&m_pVIBufferCom[iAll]))) //생성 갯수
			return E_FAIL;

		//경우마다 그거에 맞게 복사해서 최종적으로 문자열 들어가게만들기
		if (iAll == SNAIL_HEAD)
		{
			StringCchPrintf(szPartName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_SnailHead");
		}
		else if (iAll == SNAIL_BODY)
		{
			StringCchPrintf(szPartName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_SnailBody");
		}

		StringCchPrintf(szName, sizeof(TCHAR) * MAX_PATH, L"Com_Texture%d", iAll);

		if (FAILED(CGameObject::Add_Component(SCENE_STAGE0, szPartName, szName, (CComponent**)&m_pTextureCom[iAll]))) ////생성 갯수
			return E_FAIL;

		CTransform::TRANSFORM_DESC tTransformDesc[SNAIL_END];
		if (iAll == SNAIL_HEAD)
		{
			tTransformDesc[SNAIL_HEAD].vPosition = { 0.f, 0.f, 0.5f };
			tTransformDesc[SNAIL_HEAD].fSpeedPerSecond = 10.f;
			tTransformDesc[SNAIL_HEAD].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[SNAIL_HEAD].vScale = { 0.5f , 0.5f , 0.5f };
			vHeadPos = tTransformDesc[iAll].vPosition;
		}
		else if (iAll == SNAIL_BODY)
		{
			tTransformDesc[SNAIL_BODY].vPosition = { m_vStartPos.x , 0.f, m_vStartPos.z };
			tTransformDesc[SNAIL_BODY].fSpeedPerSecond = 10.f;
			tTransformDesc[SNAIL_BODY].fRotatePerSecond = D3DXToRadian(90.f);
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

	//CCollider::COLLIDER_DESC tColDesc;
	//tColDesc.vPosition = tTransformDesc[SNAIL_BODY].vPosition;
	//tColDesc.fRadius = 0.7f;

	//if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider", L"Com_Collider", (CComponent**)&m_pColliderCom, &tColDesc)))
	//	return E_FAIL;

	//CDamageInfo::DAMAGE_DESC tDmgInfo;
	//tDmgInfo.iAttack = 10;
	//tDmgInfo.pOwner = this;

	//if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_DamageInfo", L"Com_DmgInfo", (CComponent**)&m_pDmgInfoCom, &tDmgInfo)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CSnail::Movement(_float _fDeltaTime)
{
	if (FAILED(IsOnTerrain()))
		return E_FAIL;

	if (GetAsyncKeyState(VK_NUMPAD4) & 0x8000)
		m_bAttack = true;

	if (m_bAttack)
	{
		if (FAILED(LookAtPlayer(_fDeltaTime)))
			return E_FAIL;

		if (FAILED(Move(_fDeltaTime)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CSnail::IsOnTerrain()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CVIBuffer_TerrainTexture* pTerrainBuffer = (CVIBuffer_TerrainTexture*)pManagement->Get_Component(SCENE_STAGE0, L"Layer_Terrain", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	_vec3 vPosition = m_pTransformCom[SNAIL_BODY]->Get_Desc().vPosition;

	if (true == pTerrainBuffer->IsOnTerrain(&vPosition))
	{
		m_pTransformCom[SNAIL_BODY]->Set_Position(vPosition);
	}


	return S_OK;
}
HRESULT CSnail::Move(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(SCENE_STAGE0, L"Layer_Player", L"Com_Transform0");

	if (nullptr == pPlayerTransform)
		return E_FAIL;

	_vec3 vPlayerPos = pPlayerTransform->Get_Desc().vPosition;
	_vec3 m_vPos = m_pTransformCom[SNAIL_BODY]->Get_Desc().vPosition;
	m_vDir = vPlayerPos - m_vPos;
	_float m_fLength = D3DXVec3Length(&m_vDir);
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	if (!m_bAttack)
		return S_OK;

	if (0.f <= m_fLength)
	{
		m_vPos += m_vDir * _fDeltaTime;
		m_pTransformCom[SNAIL_BODY]->Set_Position(m_vPos);
	}

	return S_OK;
}

HRESULT CSnail::LookAtPlayer(_float _fDeltaTime)
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
	_vec3 vMonPos = m_pTransformCom[SNAIL_BODY]->Get_Desc().vPosition;
	_vec3 vMonLook = {};

	////--------------------------------------------------
	//// Look 과 목적지 - 출발지를 내적
	////--------------------------------------------------

	memcpy_s(&vMonLook, sizeof(_vec3), &m_pTransformCom[SNAIL_BODY]->Get_Desc().matWorld._31, sizeof(_vec3));

	_vec3 vMonToPlayer = vPlayerPos - vMonPos;

	D3DXVec3Normalize(&vMonLook, &vMonLook);
	D3DXVec3Normalize(&vMonToPlayer, &vMonToPlayer);

	_float fDot = 0.f;
	_float fRad = 0.f;

	fDot = D3DXVec3Dot(&vMonLook, &vMonToPlayer);
	fRad = (_float)acos(fDot);

	_vec3 vMonRight = {};
	D3DXVec3Cross(&vMonRight, &vMonLook, &_vec3(0.f, 1.f, 0.f));

	D3DXVec3Dot(&vMonRight, &vMonToPlayer);

	_float fLimit = D3DXVec3Dot(&vMonRight, &vMonToPlayer);

	if (fabsf(fLimit) < 0.2f)
		return S_OK;

	if (fLimit > 0)
		m_pTransformCom[SNAIL_BODY]->Turn(CTransform::AXIS_Y, -_fDeltaTime * fRad);
	else
		m_pTransformCom[SNAIL_BODY]->Turn(CTransform::AXIS_Y, _fDeltaTime * fRad);

	return S_OK;
}

HRESULT CSnail::Setting_Part()
{
	m_pTransformCom[SNAIL_HEAD]->Set_WorldMatrix(m_pTransformCom[SNAIL_HEAD]->Get_Desc().matWorld * m_pTransformCom[SNAIL_BODY]->Get_Desc().matWorld);

	return S_OK;
}

HRESULT CSnail::SetUp_Layer_InstantImpact(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	m_pInstantImpact->vPosition = m_pTransformCom[SNAIL_BODY]->Get_Desc().vPosition;
	m_pInstantImpact->pAttacker = this;
	//m_pInstantImpact->iAttack = m_pStatusCom->Get_Status().iMaxAtt;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE0, L"GameObject_Instant_Impact", SCENE_STAGE0, LayerTag , m_pInstantImpact)))/*여기 StartPos*/
		return E_FAIL;

	return S_OK;
}

CSnail* CSnail::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CSnail* pInstance = new CSnail(_pDevice);

	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CSnail", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSnail::Clone_GameObject(void * pArg)
{
	CSnail* pInstance = new CSnail(*this);

	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CSnail", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSnail::Free()
{
	for (_int iAll = 0; iAll < SNAIL_END; ++iAll)
	{
		Safe_Release(m_pTransformCom[iAll]);
		Safe_Release(m_pVIBufferCom[iAll]);
		Safe_Release(m_pTextureCom[iAll]);
	}

	Safe_Release(m_pStatusCom);

	CGameObject::Free();
}

HRESULT CSnail::Take_Damage(const CComponent* _pDamageComp)
{
	if (!_pDamageComp)
		return S_OK;

	PRINT_LOG(L"아얏", LOG::CLIENT);

	return S_OK;
}

