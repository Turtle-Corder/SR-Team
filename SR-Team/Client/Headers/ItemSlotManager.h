#pragma once

#ifndef __ITEMMANAGER_H__
#define __ITEMMANAGER_H__

BEGIN(Client)
USING(Engine)
class CPlayerItem;

class CItemSlotManager
{
private:
	CItemSlotManager();
	virtual ~CItemSlotManager() = default;

public:
	CPlayerItem* Get_PlayerItem(ePotion_ID ePotionID);

public:
	static CItemSlotManager* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CItemSlotManager;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}

private:
	static CItemSlotManager*	m_pInstance;

	CPlayerItem*			m_pRedPotion = nullptr;
	CPlayerItem*			m_pBluePotion = nullptr;
	CPlayerItem*			m_pRedElixir = nullptr;
	CPlayerItem*			m_pBlueElixir = nullptr;
};

END
#endif