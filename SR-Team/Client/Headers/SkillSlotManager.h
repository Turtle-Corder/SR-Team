#pragma once

#ifndef __SKILLMANAGER_H__
#define __SKILLMANAGER_H__


BEGIN(Client)
USING(Engine)
class CPlayerSkill;

class CSkillSlotManager 
{
private:
	CSkillSlotManager();
	virtual ~CSkillSlotManager() = default;

public:
	CSkillSlotManager* Get_PlayerSkill(eActiveSkill_ID eSkillID);

public:
	static CSkillSlotManager* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CSkillSlotManager;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}

private:
	static CSkillSlotManager*	m_pInstance;

	CPlayerSkill*			m_pLaserSkill = nullptr;
	CPlayerSkill*			m_pMeteoSkill = nullptr;
	CPlayerSkill*			m_pManaDriftSkill = nullptr;
	CPlayerSkill*			m_pEnergyExplotiationSkill = nullptr;
	CPlayerSkill*			m_pIceCrystalSkill = nullptr;
};

END
#endif