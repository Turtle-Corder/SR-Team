#include "stdafx.h"
#include "..\Headers\ItemSlotManager.h"


USING(Client)
CItemSlotManager* CItemSlotManager::m_pInstance = nullptr;

CItemSlotManager::CItemSlotManager()
{
	CManagement* pManagement = CManagement::Get_Instance();

	m_pRedPotion = (CPlayerItem*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_PlayerItem");
	m_pBluePotion = (CPlayerItem*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_PlayerItem", 1);
	m_pRedElixir = (CPlayerItem*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_PlayerItem", 2);
	m_pBlueElixir = (CPlayerItem*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_PlayerItem", 3);
}

CPlayerItem * CItemSlotManager::Get_PlayerItem(ePotion_ID ePotionID)
{
	switch (ePotionID)
	{
	case RED_POTION:
		return m_pRedPotion;
	case RED_ELIXIR:
		return m_pRedElixir;
	case BLUE_POTION:
		return m_pBluePotion;
	case BLUE_ELIXIR:
		return m_pBlueElixir;
	case POTION_END:
		break;
	default:
		break;
	}

	return nullptr;
}