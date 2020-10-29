#include "stdafx.h"
#include "..\Headers\EnergyVoltSkill.h"

USING(Client)

CEnergyVoltSkill::CEnergyVoltSkill(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CPlayerSkill(_pDevice, _pSprite, _pFont)
{
}

CEnergyVoltSkill::CEnergyVoltSkill(const CEnergyVoltSkill & _rOther)
	: CPlayerSkill(_rOther)
{
}

HRESULT CEnergyVoltSkill::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CEnergyVoltSkill::Setup_GameObject(void * _pArg)
{
	m_fStartTime = 0.f;
	m_fEndTime = 3.f;

	return S_OK;
}

_int CEnergyVoltSkill::Update_GameObject(_float _fDeltaTime)
{
	if (!m_bInitial)
		m_fStartTime += _fDeltaTime;

	return GAMEOBJECT::NOEVENT;
}

_int CEnergyVoltSkill::LateUpdate_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

CEnergyVoltSkill * CEnergyVoltSkill::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == pDevice)
		return nullptr;

	CEnergyVoltSkill* pInstance = new CEnergyVoltSkill(pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CEnergyVoltSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEnergyVoltSkill::Clone_GameObject(void * _pArg)
{
	CEnergyVoltSkill* pInstance = new CEnergyVoltSkill(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CEnergyVoltSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CEnergyVoltSkill::Use_Skill(float fDeltaTime)
{
	// 쿨타임 검사
	if (m_bInitial)
	{
		PRINT_LOG(L"에너지 볼트 스킬 사용", LOG::CLIENT);
		m_bInitial = false;
	}
	else if (m_fStartTime <= m_fEndTime)
	{
		PRINT_LOG(L"쿨타임중", LOG::CLIENT);
		return E_FAIL;
	}
	else if (m_fStartTime >= m_fEndTime)
	{
		m_fStartTime = 0.f;
		PRINT_LOG(L"쿨타임 끝 / 에너지 볼트 스킬 사용", LOG::CLIENT);
	}

	return S_OK;
}
