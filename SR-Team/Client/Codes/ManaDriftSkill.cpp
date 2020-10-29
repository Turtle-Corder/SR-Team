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
	m_fStartTime = 0.f;
	m_fEndTime = 30.f;

	return S_OK;
}

_int CManaDriftSkill::Update_GameObject(_float _fDeltaTime)
{
	if (!m_bInitial)
		m_fStartTime += _fDeltaTime;

	return GAMEOBJECT::NOEVENT;
}

_int CManaDriftSkill::LateUpdate_GameObject(_float _fDeltaTime)
{
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

HRESULT CManaDriftSkill::Use_Skill(float fDeltaTime)
{
	// 쿨타임 검사
	if (m_bInitial)
	{
		PRINT_LOG(L"마나 드리프트 스킬 사용", LOG::CLIENT);
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
		PRINT_LOG(L"쿨타임 끝 / 마나 드리프트 스킬 사용", LOG::CLIENT);
	}

	// MP 감소
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return E_FAIL;
	CEquip* pEquip = (CEquip*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_MainUI", 1);
	if (pEquip == nullptr)
		return E_FAIL;

	//pEquip->Set_PlayerMP(-20);

	return S_OK;
}
