#include "stdafx.h"
#include "..\Headers\Snail.h"

USING(Client)

CSnail::CSnail(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
	for (int iAll = 0; iAll < SNAIL_END; ++iAll)
	{
		m_pTransformCom[iAll] = nullptr;
		m_pVIBufferCom[iAll] = nullptr;
		m_pTextureCom[iAll] = nullptr;
	}
}

CSnail::CSnail(const CSnail & other)
	: CGameObject(other)
{
}

HRESULT CSnail::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CSnail::Setup_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

int CSnail::Update_GameObject(float _fDeltaTime)
{
	if (FAILED(Movement(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	if (FAILED(LookAtPlayer(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

int CSnail::LateUpdate_GameObject(float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(Setting_Part()))
		return 0;

	for (int iAll = 0; iAll < SNAIL_END; ++iAll)
	{
		if (FAILED(m_pTransformCom[iAll]->Update_Transform()))
			return 0;
	}

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

	for (int iAll = 0; iAll < SNAIL_END; ++iAll)
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

HRESULT CSnail::Add_Component()
{
	// For.Com_VIBuffer
	TCHAR szName[MAX_PATH] = L"";
	TCHAR szPartName[MAX_PATH] = L"";
	_vec3 vHeadPos = {};

	// For.Com_Texture
	for (int iAll = 0; iAll < SNAIL_END; ++iAll)
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
			tTransformDesc[SNAIL_HEAD].vPosition = { 10.f, 0.5f, 10.f };
			tTransformDesc[SNAIL_HEAD].fSpeedPerSecond = 10.f;
			tTransformDesc[SNAIL_HEAD].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[SNAIL_HEAD].vScale = { 0.5f , 0.5f , 0.5f };
			vHeadPos = tTransformDesc[iAll].vPosition;
		}
		else if (iAll == SNAIL_BODY)
		{
			tTransformDesc[SNAIL_BODY].vPosition = { vHeadPos.x + 0.5f, 1.f, vHeadPos.z };
			tTransformDesc[SNAIL_BODY].fSpeedPerSecond = 10.f;
			tTransformDesc[SNAIL_BODY].fRotatePerSecond = D3DXToRadian(90.f);
		}
		StringCchPrintf(szName, sizeof(TCHAR) * MAX_PATH, L"Com_Transform%d", iAll);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", szName, (CComponent**)&m_pTransformCom[iAll], &tTransformDesc[iAll]))) ////생성 갯수
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CSnail::Movement(float _fDeltaTime)
{
	if (FAILED(IsOnTerrain()))
		return E_FAIL;

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
	D3DXVECTOR3 vPosition[SNAIL_END];

	for (int i = 0; i < SNAIL_END; ++i)
	{
		vPosition[i] = m_pTransformCom[i]->Get_Desc().vPosition;

		if (true == pTerrainBuffer->IsOnTerrain(&vPosition[i]))
		{
			m_pTransformCom[i]->Set_Position(vPosition[i]);
		}
	}

	return S_OK;
}

HRESULT CSnail::Move(float _fDeltaTime)
{
	return S_OK;
}

HRESULT CSnail::LookAtPlayer(float _fDeltaTime)
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
		m_pTransformCom[SNAIL_BODY]->Turn(CTransform::AXIS_Y, -_fDeltaTime * fRad);
	else
		m_pTransformCom[SNAIL_BODY]->Turn(CTransform::AXIS_Y, _fDeltaTime * fRad);

	return S_OK;
}

HRESULT CSnail::Setting_Part()
{
	_vec3 vBodyPos = m_pTransformCom[SNAIL_BODY]->Get_Desc().vPosition;

	return S_OK;
}

CSnail * CSnail::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CSnail* pInstance = new CSnail(pDevice);

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
	for (int iAll = 0; iAll < SNAIL_END; ++iAll)
	{
		Safe_Release(m_pTransformCom[iAll]);
		Safe_Release(m_pVIBufferCom[iAll]);
		Safe_Release(m_pTextureCom[iAll]);
	}

	CGameObject::Free();
}
