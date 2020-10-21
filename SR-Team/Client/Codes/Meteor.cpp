#include "stdafx.h"
#include "..\Headers\Meteor.h"

USING(Client)

CMeteor::CMeteor(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)

{
}

CMeteor::CMeteor(const CMeteor & other)
	: CGameObject(other)

{
}

HRESULT CMeteor::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CMeteor::Setup_GameObject(void * pArg)
{
	if(pArg)
		m_vGoalPos = *(_vec3*)pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

int CMeteor::Update_GameObject(float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (FAILED(Movement(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	if (FAILED(m_pTransformCom->Update_Transform()))
		return 0;

	return GAMEOBJECT::NOEVENT;
}

int CMeteor::LateUpdate_GameObject(float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return  GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CMeteor::Render_NoneAlpha()
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

HRESULT CMeteor::Add_Component()
{
	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE0, L"Component_Texture_Meteor", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform::TRANSFORM_DESC tTransformDesc;

	// TerrainY 로 부터 높이 구해서 20.f

	tTransformDesc.vPosition = { m_vGoalPos.x - 10.f, m_vGoalPos.y + 14.f, m_vGoalPos.z + 10.f };
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;

	//CStat::STAT tStat;
	//tStat.iCurHp = 100;


	return S_OK;
}

HRESULT CMeteor::Movement(float _fDeltaTime)
{
	if (FAILED(FallDown_Meteor(_fDeltaTime)))
		return E_FAIL;

	if (FAILED(IsOnTerrain()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMeteor::FallDown_Meteor(float _fDeltaTime)
{
	m_fDownTime += _fDeltaTime * 1.f;

	_vec3 vPos = m_pTransformCom->Get_Desc().vPosition;

	_vec3 vDir = m_vGoalPos - vPos;
	D3DXVec3Normalize(&vDir, &vDir);

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CVIBuffer_TerrainTexture* pTerrainBuffer = (CVIBuffer_TerrainTexture*)pManagement->Get_Component(SCENE_STAGE0, L"Layer_Terrain", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
		return E_FAIL;


	if (vPos.y < m_vGoalPos.y)
	{
		m_bDead = true;
	}
	else
	{
		vPos += vDir * (m_fDownTime);
		m_pTransformCom->Set_Position(vPos);
	}

	return S_OK;
}

HRESULT CMeteor::IsOnTerrain()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CVIBuffer_TerrainTexture* pTerrainBuffer = (CVIBuffer_TerrainTexture*)pManagement->Get_Component(SCENE_STAGE0, L"Layer_Terrain", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	D3DXVECTOR3 vPosition = m_pTransformCom->Get_Desc().vPosition;


	return S_OK;
}

CMeteor * CMeteor::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CMeteor* pInstance = new CMeteor(pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CMeteor", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMeteor::Clone_GameObject(void * pArg)
{
	CMeteor* pInstance = new CMeteor(*this);
	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CMeteor", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMeteor::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}
