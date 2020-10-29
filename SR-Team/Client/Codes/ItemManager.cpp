#include "stdafx.h"
#include "..\Headers\ItemManager.h"


USING(Client)
CItemManager* CItemManager::m_pInstance = nullptr;

CItemManager::CItemManager()
{
	CManagement* pManagement = CManagement::Get_Instance();

	m_pRedPotion = (CPlayerItem*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_PlayerItem");
}

CPlayerItem * CItemManager::Get_PlayerItem(ePotion_ID ePotionID)
{
	switch (ePotionID)
	{
	case RED_POTION:
		return m_pRedPotion;
	case RED_ELIXIR:
		break;
	case BLUE_POTION:
		break;
	case BLUE_ELIXIR:
		break;
	case POTION_END:
		break;
	default:
		break;
	}
	return nullptr;
}
