#include "..\Headers\Component.h"

USING(Engine)

CComponent::CComponent(LPDIRECT3DDEVICE9 _pDevice)
	: m_pDevice(_pDevice)
{
	Safe_AddRef(m_pDevice);
}

CComponent::CComponent(const CComponent & rOther)
	: m_pDevice(rOther.m_pDevice)
{
	Safe_AddRef(m_pDevice);

	m_bIsClone = true;
}

HRESULT CComponent::Setup_Component_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CComponent::Setup_Component(void * pArg)
{
	return E_NOTIMPL;
}


void CComponent::Free(void)
{
	Safe_Release(m_pDevice);
}
