#include "stdafx.h"
#include "..\Headers\IceStrikeSkill.h"

USING(Client)



CIceStrikeSkill::CIceStrikeSkill(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CPlayerSkill(_pDevice, _pSprite, _pFont)
{
}

CIceStrikeSkill::CIceStrikeSkill(const CIceStrikeSkill & _rOther)
	: CPlayerSkill(_rOther)
{
}

HRESULT CIceStrikeSkill::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CIceStrikeSkill::Setup_GameObject(void * _pArg)
{
	m_iCanUseCnt = m_iMaxUseCnt = 1;
	m_fEachDelay = 5.f;

	m_iConsumeMP = 10;

	return S_OK;
}

_int CIceStrikeSkill::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CIceStrikeSkill::LateUpdate_GameObject(_float _fDeltaTime)
{
	if (FAILED(Update_Delay(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

CIceStrikeSkill * CIceStrikeSkill::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == _pDevice)
		return nullptr;

	CIceStrikeSkill* pInstance = new CIceStrikeSkill(_pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CIceStrikeSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CIceStrikeSkill::Clone_GameObject(void * _pArg)
{
	CIceStrikeSkill* pInstance = new CIceStrikeSkill(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CIceStrikeSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

_bool CIceStrikeSkill::Actual_UseSkill(void * _pArg)
{
	if (!Can_UseSkill())
		return false;

	// TODO : ¼ÒÈ¯..
	if (_pArg)
	{
		INSTANTIMPACT* pImpact = (INSTANTIMPACT*)_pArg;
		CStatus* pStatus = (CStatus*)pImpact->pStatusComp;
		if (pStatus)
			pStatus->Increase_IceStack();
	}


	--m_iCanUseCnt;
	return true;
}
