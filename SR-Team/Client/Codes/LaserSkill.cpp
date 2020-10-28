#include "stdafx.h"
#include "..\Headers\LaserSkill.h"

USING(Client)


CLaserSkill::CLaserSkill(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CPlayerSkill(_pDevice, _pSprite, _pFont)
{
}

CLaserSkill::CLaserSkill(const CLaserSkill & _rOther)
	: CPlayerSkill(_rOther)
{
}

HRESULT CLaserSkill::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CLaserSkill::Setup_GameObject(void * _pArg)
{
	CManagement* pManagement = CManagement::Get_Instance();
	m_fStartTime = 0.f;
	m_fEndTime = 5.f;

	if (pManagement == nullptr)
		return E_FAIL;
	if (FAILED(pManagement->Add_Timer(L"Laser_Timer", true)))
		return E_FAIL;
	if (FAILED(pManagement->Pause(L"Laser_Timer")))
		return E_FAIL;
	return S_OK;
}

_int CLaserSkill::Update_GameObject(_float _fDeltaTime)
{
	if (!m_bInitial)
		m_fStartTime += _fDeltaTime;

	return GAMEOBJECT::NOEVENT;
}

_int CLaserSkill::LateUpdate_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

CLaserSkill * CLaserSkill::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == pDevice)
		return nullptr;

	CLaserSkill* pInstance = new CLaserSkill(pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CLaserSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CLaserSkill::Clone_GameObject(void * _pArg)
{
	CLaserSkill* pInstance = new CLaserSkill(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CLaserSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CLaserSkill::Use_Skill(float fDeltaTime)
{
	if (m_bInitial)
	{
		PRINT_LOG(L"레이저 스킬 사용", LOG::CLIENT);
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
		PRINT_LOG(L"쿨타임 끝 / 레이저 스킬 사용", LOG::CLIENT);
	}

	return S_OK;
}