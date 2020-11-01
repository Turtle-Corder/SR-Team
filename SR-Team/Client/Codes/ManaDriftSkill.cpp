#include "stdafx.h"
#include "..\Headers\ManaDriftSkill.h"
#include "Equip.h"

USING(Client)


CManaDriftSkill::CManaDriftSkill(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CPlayerSkill(_pDevice, _pSprite, _pFont)
{
}

CManaDriftSkill::CManaDriftSkill(const CManaDriftSkill & _rOther)
	: CPlayerSkill(_rOther)
{
}

HRESULT CManaDriftSkill::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CManaDriftSkill::Setup_GameObject(void * _pArg)
{
	m_iCanUseCnt = m_iMaxUseCnt = 1;
	m_fEachDelay = 10.f;

	m_iConsumeMP = 10;

	return S_OK;
}

_int CManaDriftSkill::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CManaDriftSkill::LateUpdate_GameObject(_float _fDeltaTime)
{
	if (FAILED(Update_Delay(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

CManaDriftSkill * CManaDriftSkill::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == pDevice)
		return nullptr;

	CManaDriftSkill* pInstance = new CManaDriftSkill(pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CManaDriftSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CManaDriftSkill::Clone_GameObject(void * _pArg)
{
	CManaDriftSkill* pInstance = new CManaDriftSkill(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CManaDriftSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

_bool CManaDriftSkill::Actual_UseSkill(void* _pArg)
{
	// 한번 더 검사
	if (!Can_UseSkill())
		return false;

	//--------------------------------------------------
	// TODO : 버프 이펙트 소환
	//--------------------------------------------------


	--m_iCanUseCnt;
	return true;
}
