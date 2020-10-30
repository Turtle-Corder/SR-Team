#include "stdafx.h"
#include "..\Headers\Meteor.h"
#include "Status.h"
#include "DamageInfo.h"

USING(Client)

CMeteor::CMeteor(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)

{
}

CMeteor::CMeteor(const CMeteor& other)
	: CGameObject(other)
{
}

HRESULT CMeteor::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CMeteor::Setup_GameObject(void* _pArg)
{
	if (_pArg)
	{
		m_tInstant = *(INSTANTIMPACT*)_pArg;
	}

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

int CMeteor::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (FAILED(Movement(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	if (FAILED(m_pColliderCom->Update_Collider(m_pTransformCom->Get_Desc().vPosition)))
		return GAMEOBJECT::WARN;

	if (FAILED(m_pTransformCom->Update_Transform()))
		return GAMEOBJECT::WARN;



	return GAMEOBJECT::NOEVENT;                                                   
}

int CMeteor::LateUpdate_GameObject(_float _fDeltaTime)
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
	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.vPosition = { _vec3(m_tInstant.vPosition.x - 10.f , m_tInstant.vPosition.y + 14.f , m_tInstant.vPosition.z + 10.f) };/*{ m_vGoalPos.x - 10.f, m_vGoalPos.y + 14.f, m_vGoalPos.z + 10.f };*/
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);

	CSphereCollider::COLLIDER_DESC tCollDesc;
	tCollDesc.vPosition = tTransformDesc.vPosition;
	tCollDesc.fRadius = 0.7f;

	CStatus::STAT tStat;
	tStat.iCriticalHit = 0; tStat.iCriticalRate = 0;
	tStat.iMinAtt = 10; tStat.iMaxAtt = 10;

	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE0, L"Component_Texture_Meteor", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	// For.Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;
	
	// For.Collider
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider", (CComponent**)&m_pColliderCom, &tCollDesc)))
		return E_FAIL;
	
	// For.Status
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Status", L"Com_Stat", (CComponent**)&m_pStatusComp, &tStat)))
		return E_FAIL;

	CStatus* pOwnerStatusComp = (CStatus*)m_tInstant.pStatusComp;
	CDamageInfo::DAMAGE_DESC tDmgInfo;
	
	if (pOwnerStatusComp)
	{
		tDmgInfo.pOwner = m_tInstant.pAttacker;
		tDmgInfo.iMinAtt = pOwnerStatusComp->Get_Status().iMinAtt + m_pStatusComp->Get_Status().iMinAtt;
		tDmgInfo.iMaxAtt = pOwnerStatusComp->Get_Status().iMaxAtt + m_pStatusComp->Get_Status().iMaxAtt;
		tDmgInfo.iCriticalHit = pOwnerStatusComp->Get_Status().iCriticalHit + m_pStatusComp->Get_Status().iCriticalHit;
		tDmgInfo.iCriticalRate = pOwnerStatusComp->Get_Status().iCriticalRate + m_pStatusComp->Get_Status().iCriticalRate;
	}
	else
	{
		tDmgInfo.iMinAtt = m_pStatusComp->Get_Status().iMinAtt;
		tDmgInfo.iMaxAtt = m_pStatusComp->Get_Status().iMaxAtt;
		tDmgInfo.iCriticalHit = m_pStatusComp->Get_Status().iCriticalHit;
		tDmgInfo.iCriticalRate = m_pStatusComp->Get_Status().iCriticalRate;
	}

	tDmgInfo.eType = FIRE;

	//For.DamageInfo
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_DamageInfo", L"Com_DmgInfo", (CComponent**)&m_pDmgInfoCom, &tDmgInfo)))
		return E_FAIL;
	return S_OK;
}

HRESULT CMeteor::Movement(_float _fDeltaTime)
{
	if (m_bDead)
	return GAMEOBJECT::DEAD;

	if (FAILED(FallDown_Meteor(_fDeltaTime)))
		return E_FAIL;

	if (FAILED(IsOnTerrain()))
		return E_FAIL;

	m_fDeadTime += _fDeltaTime;

	if (m_fDeadTime >= 3.f)
		m_bDead = true;

	return S_OK;
}

HRESULT CMeteor::FallDown_Meteor(_float _fDeltaTime)
{
	m_fDownTime += _fDeltaTime * 1.f;

	_vec3 vPos = m_pTransformCom->Get_Desc().vPosition;

	_vec3 vDir = m_tInstant.vPosition - vPos;
	D3DXVec3Normalize(&vDir, &vDir);

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CVIBuffer_TerrainTexture* pTerrainBuffer = (CVIBuffer_TerrainTexture*)pManagement->Get_Component(SCENE_STAGE0, L"Layer_Terrain", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
		return E_FAIL;


	if (vPos.y < m_tInstant.vPosition.y)
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
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pStatusComp);
	Safe_Release(m_pDmgInfoCom);

	CGameObject::Free();
}

HRESULT CMeteor::Take_Damage(const CComponent * _pDamageComp)
{
	if(!_pDamageComp)
	return S_OK;



	return S_OK;
}
