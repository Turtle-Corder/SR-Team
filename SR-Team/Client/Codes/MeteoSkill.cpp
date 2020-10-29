#include "stdafx.h"
#include "..\Headers\MeteoSkill.h"

USING(Client)

CMeteoSkill::CMeteoSkill(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CPlayerSkill(_pDevice, _pSprite, _pFont)
{
}

CMeteoSkill::CMeteoSkill(const CMeteoSkill & _rOther)
	: CPlayerSkill(_rOther)
{
}

HRESULT CMeteoSkill::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CMeteoSkill::Setup_GameObject(void * _pArg)
{
	m_fStartTime = 0.f;
	m_fEndTime = 7.f;

	return S_OK;
}

_int CMeteoSkill::Update_GameObject(_float _fDeltaTime)
{
	if (!m_bInitial)
		m_fStartTime += _fDeltaTime;

	return GAMEOBJECT::NOEVENT;
}

_int CMeteoSkill::LateUpdate_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

CMeteoSkill * CMeteoSkill::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == _pDevice)
		return nullptr;

	CMeteoSkill* pInstance = new CMeteoSkill(_pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CMeteoSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMeteoSkill::Clone_GameObject(void * _pArg)
{
	CMeteoSkill* pInstance = new CMeteoSkill(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CMeteoSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CMeteoSkill::Use_Skill(float fDeltaTime)
{
	if (m_bInitial)
	{
		PRINT_LOG(L"메테오 스킬 사용", LOG::CLIENT);
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
		PRINT_LOG(L"쿨타임 끝 / 메테오 스킬 사용", LOG::CLIENT);
	}

	// 공격체 생성

	return S_OK;
}
