#include "stdafx.h"
#include "..\Headers\Status.h"

USING(Client)

CStatus::CStatus(LPDIRECT3DDEVICE9 _pDevice)
	: CComponent(_pDevice)
{
}

CStatus::CStatus(const CStatus & _rOther)
	: CComponent(_rOther)
{
}

const CStatus::STAT& CStatus::Get_Status() const
{
	return m_tStat;
}

void CStatus::Set_Status(const STAT & _rStatus)
{
	m_tStat = _rStatus;
}

void CStatus::Set_Att(_int iAtt)
{
	m_tStat.iMinAtt += iAtt;
	m_tStat.iMaxAtt += iAtt;
}

void CStatus::Set_HP(_int _iAtt)
{
	m_tStat.iHp -= _iAtt;
}

void CStatus::Set_MP(_int _iMP)
{
	m_tStat.iMp -= _iMP;
}

_int CStatus::Increase_FireStack()
{
	m_tStat.iCurFireStack = min(m_tStat.iMaxFireStack, m_tStat.iCurFireStack + 1);

	return m_tStat.iCurFireStack;
}

_int CStatus::Increase_IceStack()
{
	m_tStat.iCurIceStack = min(m_tStat.iMaxIceStack, m_tStat.iCurIceStack + 1);

	return m_tStat.iCurIceStack;
}

void CStatus::Clear_FireStack()
{
	m_tStat.iCurFireStack = 0;
}

void CStatus::Clear_IceStack()
{
	m_tStat.iCurIceStack = 0;
}

HRESULT CStatus::Setup_Component_Prototype()
{
	return S_OK;
}

HRESULT CStatus::Setup_Component(void * _pArg)
{
	if (_pArg != nullptr)
		memcpy_s(&m_tStat, sizeof(STAT), _pArg, sizeof(STAT));

	return S_OK;
}

CStatus * CStatus::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CStatus* pInstance = new CStatus(_pDevice);
	if (FAILED(pInstance->Setup_Component_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CStat", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CStatus::Clone_Component(void * pArg)
{
	CStatus* pInstance = new CStatus(*this);
	if (FAILED(pInstance->Setup_Component(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CStat", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatus::Free()
{
	CComponent::Free();
}
