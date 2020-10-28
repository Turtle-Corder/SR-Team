#include "stdafx.h"
#include "..\Headers\SkillInven.h"
#include "SkillManager.h"
#include "PlayerSkill.h"

USING(Client)

CSkillInven::CSkillInven(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
	for (_uint i = 0; i < 8; i++)
	{
		m_pPlayerSkill[i] = nullptr;
		m_pPlayerSkillID[i] = ACTIVE_SKILL_END;
	}
}

CSkillInven::CSkillInven(const CSkillInven & _rOther)
	: CUIObject(_rOther)
{
}

HRESULT CSkillInven::Set_SkillIndex(_int iIndex, eActiveSkill_ID eSkillID)
{
	m_pPlayerSkill[iIndex] = CSkillManager::Get_Instance()->Get_PlayerSkill(eSkillID);
	m_pPlayerSkillID[iIndex] = eSkillID;

	_int k = 0;
	return S_OK;
}

HRESULT CSkillInven::Use_Skill(_int iIndex, _float fDeltaTime)
{
	if (m_pPlayerSkill[iIndex])
		return m_pPlayerSkill[iIndex]->Use_Skill(fDeltaTime);
	return E_FAIL;
}

eActiveSkill_ID CSkillInven::Get_SkillID(_int iIndex)
{
	if (m_pPlayerSkillID[iIndex] != ACTIVE_SKILL_END)
		return m_pPlayerSkillID[iIndex];
	return ACTIVE_SKILL_END;
}

HRESULT CSkillInven::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CSkillInven::Setup_GameObject(void * _pArg)
{
	for (_uint i = 0; i < 8; i++)
	{
		m_pPlayerSkill[i] = nullptr;
		m_pPlayerSkillID[i] = ACTIVE_SKILL_END;
	}
	return S_OK;
}

_int CSkillInven::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CSkillInven::LateUpdate_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

CSkillInven * CSkillInven::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == pDevice)
		return nullptr;

	CSkillInven* pInstance = new CSkillInven(pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CSkillInven", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSkillInven::Clone_GameObject(void * _pArg)
{
	CSkillInven* pInstance = new CSkillInven(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CSkillInven", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}
