#pragma once

#ifndef __SKILLMANAGER_H__
#define __SKILLMANAGER_H__


BEGIN(Client)
USING(Engine)
class CPlayerSkill;

class CSkillManager 
{
private:
	CSkillManager();
	virtual ~CSkillManager() = default;

public:
	CPlayerSkill* Get_PlayerSkill(eActiveSkill_ID eSkillID);

public:
	static CSkillManager* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CSkillManager;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}

private:
	static CSkillManager*	m_pInstance;

	CPlayerSkill*			m_pLaserSkill = nullptr;
	CPlayerSkill*			m_pMeteoSkill = nullptr;
	CPlayerSkill*			m_pEnergyVoltSkill = nullptr;
};

END
#endif