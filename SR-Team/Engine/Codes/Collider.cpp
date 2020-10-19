#include "..\Headers\Collider.h"

USING(Engine)

CCollider::CCollider(LPDIRECT3DDEVICE9 _pDevice)
	: CComponent(_pDevice)
{
}

CCollider::CCollider(const CCollider & _rOther)
	: CComponent(_rOther)
{
}

const CCollider::COLLIDER_DESC & CCollider::Get_Desc() const
{
	return m_tColliderDesc;
}

HRESULT CCollider::Setup_Component_Prototype()
{
	return S_OK;
}

HRESULT CCollider::Setup_Component(void * _pArg)
{
	if (_pArg)
		memcpy_s(&m_tColliderDesc, sizeof(COLLIDER_DESC), _pArg, sizeof(COLLIDER_DESC));

	return S_OK;
}

HRESULT CCollider::Update_Collider(const _vec3 & vPosition)
{
	m_tColliderDesc.vPosition = vPosition;

	return S_OK;
}

void CCollider::Set_Radius(_float _fRadius)
{
	m_tColliderDesc.fRadius = _fRadius;
}

CCollider * CCollider::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CCollider* pInstance = new CCollider(_pDevice);
	if (FAILED(pInstance->Setup_Component_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CCollider", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollider::Clone_Component(void * _pArg)
{
	CCollider* pInstance = new CCollider(*this);
	if (FAILED(pInstance->Setup_Component(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CCollider", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider::Free()
{
	CComponent::Free();
}
