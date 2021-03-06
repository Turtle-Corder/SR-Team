#pragma once

#ifndef __ITEMMANAGER_H__
#define __ITEMMANAGER_H__

BEGIN(Client)
USING(Engine)
class CPlayerItem;

class CItemManager
{
private:
	CItemManager();
	virtual ~CItemManager() = default;

public:
	CPlayerItem* Get_PlayerItem(ePotion_ID ePotionID);

public:
	static CItemManager* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CItemManager;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}

private:
	static CItemManager*	m_pInstance;

	CPlayerItem*			m_pRedPotion = nullptr;
	CPlayerItem*			m_pBluePotion = nullptr;
	CPlayerItem*			m_pRedElixir = nullptr;
	CPlayerItem*			m_pBlueElixir = nullptr;
};

END
#endif