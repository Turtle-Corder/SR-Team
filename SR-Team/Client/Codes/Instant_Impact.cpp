#include "stdafx.h"
#include "..\Headers\Instant_Impact.h"
#include "DamageInfo.h"

USING(Client)

CInstant_Impact::CInstant_Impact(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CInstant_Impact::CInstant_Impact(const CInstant_Impact & _rOther)
	:CGameObject(_rOther)
{
}

HRESULT CInstant_Impact::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CInstant_Impact::Setup_GameObject(void* _pArg)
{
	if (_pArg)
		m_vPos = *(_vec3*)_pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CInstant_Impact::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	m_pColliderCom->Update_Collider((m_pTransformCom->Get_Desc().vPosition));

	return GAMEOBJECT::NOEVENT;
}

_int CInstant_Impact::LateUpdate_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

HRESULT CInstant_Impact::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.vPosition = { m_vPos.x , m_vPos.y , m_vPos.z };
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);
	tTransformDesc.vScale = { 0.5f , 0.5f , 0.5f };

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;

	CCollider::COLLIDER_DESC tColDesc;
	tColDesc.vPosition = tTransformDesc.vPosition;
	tColDesc.fRadius = 0.7f;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider", L"Com_Collider", (CComponent**)&m_pColliderCom, &tColDesc)))
		return E_FAIL;

	CDamageInfo::DAMAGE_DESC tDmgInfo;
	tDmgInfo.iAttack = 10;
	tDmgInfo.pOwner = this;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_DamageInfo", L"Com_DmgInfo", (CComponent**)&m_pDmgInfoCom, &tDmgInfo)))
		return E_FAIL;
}

CGameObject * CInstant_Impact::Clone_GameObject(void * _pArg)
{
	CInstant_Impact* pInstance = new CInstant_Impact(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Create InstantImpact Clone", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInstant_Impact::Free()
{
	Safe_Release(m_pTransformCom);
}

CInstant_Impact * CInstant_Impact::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if(nullptr == _pDevice)
		return nullptr;

	CInstant_Impact* pInstance = new CInstant_Impact(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create InstantImpact", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CInstant_Impact::Take_Damage(const CComponent * _pDamageComp)
{
	if (!_pDamageComp)
		return E_FAIL;


}
