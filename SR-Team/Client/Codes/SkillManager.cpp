#include "stdafx.h"
#include "..\Headers\SkillManager.h"

USING(Client)
CSkillManager* CSkillManager::m_pInstance = nullptr;

CSkillManager::CSkillManager()
{
	CManagement* pManagement = CManagement::Get_Instance();

	m_pLaserSkill = (CPlayerSkill*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_PlayerSkill", 0);
	m_pMeteoSkill = (CPlayerSkill*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_PlayerSkill", 1);
	m_pEnergyVoltSkill = (CPlayerSkill*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_PlayerSkill", 2);
}

CPlayerSkill * CSkillManager::Get_PlayerSkill(eActiveSkill_ID eSkillID)
{
	switch (eSkillID)
	{
	case ACTIVE_ICE_STRIKE:
		break;
	case ACTIVE_MANA_DRIFT:
		break;
	case ACTIVE_ENERGY_EXPLOTIATION:
		break;
	case ACTIVE_FLAME_WAVE:
		return m_pMeteoSkill;
	case ACTIVE_ICE_SPEAR:
		// ∑π¿Ã¿˙
		return m_pLaserSkill;
	case ACTIVE_MAGIC_ARMOR:
		break;
	case ACTIVE_NORMAL_ATTACK:
		break;
	case ACTIVE_SKILL_END:
		break;
	default:
		break;
	}

	return nullptr;
}

