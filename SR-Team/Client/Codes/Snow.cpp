#include "stdafx.h"
#include "..\Headers\Snow.h"

USING(Client)

CSnow::CSnow(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)

{
}

CSnow::CSnow(const CSnow & other)
	: CGameObject(other)
{
}

HRESULT CSnow::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CSnow::Setup_GameObject(void * pArg)
{
	if(pArg)
		m_vPos = *(_vec3*)pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

int CSnow::Update_GameObject(float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (FAILED(Movement(_fDeltaTime)))
		return GAMEOBJECT::WARN;


	return GAMEOBJECT::NOEVENT;
}

int CSnow::LateUpdate_GameObject(float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(m_pTransformCom->Update_Transform()))
		return GAMEOBJECT::WARN;

	//if (FAILED(LookAtPlayer(_fDeltaTime)))
	//	return E_FAIL;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CSnow::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Set_Transform(&m_pTransformCom->Get_Desc().matWorld, pCamera)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetTexture(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSnow::Add_Component()
{
	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE0, L"Component_Texture_Snow", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// For.Com_Transform
	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.vPosition = { m_vPos.x , m_vPos.y , m_vPos.z };
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);
	tTransformDesc.vScale = { 0.5f , 0.5f , 0.5f };

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;


	if (FAILED(Setting_Dir()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSnow::Movement(float _fDeltaTime)
{
	if (FAILED(Throwing_Snow(_fDeltaTime)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSnow::Setting_Dir()
{
	_vec3 vPos = m_pTransformCom->Get_Desc().vPosition;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(SCENE_STAGE0, L"Layer_Player", L"Com_Transform1");

	if (nullptr == pPlayerTransform)
		return E_FAIL;

	_vec3 vPlayerPos = pPlayerTransform->Get_Desc().vPosition;

	m_vDir = vPlayerPos - vPos;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_bFallDown = true;

	return S_OK;
}

CSnow * CSnow::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CSnow* pInstance = new CSnow(pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CSnow", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSnow::Clone_GameObject(void * pArg)
{
	CSnow* pInstance = new CSnow(*this);
	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CSnow", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSnow::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}

HRESULT CSnow::Throwing_Snow(float _fDeltaTime)
{
	D3DXVECTOR3 vPos = m_pTransformCom->Get_Desc().vPosition;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CVIBuffer_TerrainTexture* pTerrainBuffer = (CVIBuffer_TerrainTexture*)pManagement->Get_Component(SCENE_STAGE0, L"Layer_Terrain", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	D3DXVECTOR3 vPosition = m_pTransformCom->Get_Desc().vPosition;

	if (true == pTerrainBuffer->IsOnTerrain(&vPosition))
	{
		m_pTransformCom->Set_Position(vPosition);
	}

	if (m_bFallDown)
	{
		vPos.y += m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime;
		m_fJumpTime += _fDeltaTime;

		vPos += _vec3(m_vDir.x, 0.f, m_vDir.z) * (_fDeltaTime * 18.f);
	}

	if (vPosition.y > vPos.y)
	{
		m_bFallDown = false;
		m_bDead = true;
	}
	m_pTransformCom->Set_Position(_vec3(vPos.x, vPos.y, vPos.z));

	return S_OK;
}
