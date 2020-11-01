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
	m_iCanUseCnt = m_iMaxUseCnt = 1;
	m_fEachDelay = 10.f;

	m_iConsumeMP = 10;

	return S_OK;
}

_int CMeteoSkill::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CMeteoSkill::LateUpdate_GameObject(_float _fDeltaTime)
{
	if (FAILED(Update_Delay(_fDeltaTime)))
		return GAMEOBJECT::WARN;

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

_bool CMeteoSkill::Actual_UseSkill(void* _pArg)
{
	// 한번 더 검사
	if (!Can_UseSkill())
		return false;

	if (_pArg)
	{
		INSTANTIMPACT* pImpact = (INSTANTIMPACT*)_pArg;
		CStatus* pStatus = (CStatus*)pImpact->pStatusComp;
		if (pStatus)
			pStatus->Increase_FireStack();
	}

	//--------------------------------------------------
	// TODO : 메테오 소환, 5개 한방에 소환
	//--------------------------------------------------
	for (_uint iCnt = 0; iCnt < 5; ++iCnt)
	{

	}
	
	--m_iCanUseCnt;
	return true;
}