#pragma once
#ifndef __CLIENT_ENUM_H__
#define __CLIENT_ENUM_H__

enum eSCENE_ID
{
	SCENE_STATIC,
	SCENE_LOGO,
	SCENE_STAGE0,
	SCENE_STAGE1,
	SCENE_STAGE2,
	SCENE_STAGE3,
	SCENE_STAGE4,
	SCENE_STAGE5,
	SCENE_END,
};





//----------------------------------------------------------------------------------------------------
// �ΰ� ���� ����
//----------------------------------------------------------------------------------------------------
enum eDROID_PART
{
	PART_START,
	PART_HEAD = PART_START,
	PART_BODY,
	PART_HAND_LEFT, PART_HAND_RIGHT,
	PART_FOOT_LEFT, PART_FOOT_RIGHT,
	PART_END
};





//----------------------------------------------------------------------------------------------------
// �÷��̾� ��ų ID
//----------------------------------------------------------------------------------------------------
enum ePLAYER_SKILL_ID
{
	PLAYER_NORMAL_ATTACK,
	PLAYER_SKILL_LASER,
	PLAYER_SKILL_FALL,
	PLAYER_SKILL_END
};





//----------------------------------------------------------------------------------------------------
// ���� UI
//----------------------------------------------------------------------------------------------------
enum eMAINUI_ID
{
	MAINUI_START, 
	MAINUI_MAIN = MAINUI_START,
	MAINUI_HP, MAINUI_MP,
	MAINUI_QUICKSLOT_LFFT, MAINUI_QUICKSLOT_RIGHT,
	MAINUI_END
};


//--------------------------------------------------
// ���� UI
//--------------------------------------------------
enum eSHOP_ID
{
	SHOP_START,
	SHOP_WND = SHOP_START,
	SHOP_SCROLLBAR,
	SHOP_END
};


//--------------------------------------------------
// �κ��丮 UI
//--------------------------------------------------
enum eINVEN_ID
{
	INVEN_START,
	INVEN_WND = INVEN_START,
	INVEN_SELL_BUTTON,
	INVEN_GOLD,
	INVEN_END
};

//--------------------------------------------------
// ���â UI
//--------------------------------------------------
enum eEQUIP_ID
{
	EQUIP_START,
	EQUIP_WND = EQUIP_START, 
	EQUIP_EQUIPMENT,
	EQUIP_INFO,
	EQUIP_END
};


//--------------------------------------------------
// ��ųâ UI
//--------------------------------------------------
enum eSKILL_ID
{
	SKILL_START,
	SKILL_ACTIVE_WND = SKILL_START,
	SKILL_PASSIVE_WND,
	SKILL_END
};


//--------------------------------------------------
// ��Ƽ�� ��ų ����
//--------------------------------------------------
enum eActiveSkill_ID
{
	ACTIVE_ICE_STRIKE, 
	ACTIVE_MANA_DRIFT, 
	ACTIVE_ENERGY_EXPLOTIATION, 
	ACTIVE_FLAME_WAVE,
	ACTIVE_ICE_SPEAR,
	ACTIVE_MAGIC_ARMOR, 
	ACTIVE_NORMAL_ATTACK,
	ACTIVE_SKILL,
	ACTIVE_SKILL_END
};


//--------------------------------------------------
// ���� ������ ����
//--------------------------------------------------
enum ePotion_ID
{
	RED_POTION, RED_ELIXIR,
	BLUE_POTION, BLUE_ELIXIR,
	POTION_END
};


//--------------------------------------------------
// ������ ����
//--------------------------------------------------
enum eITEM_SORT
{
	CAP, NECKLACE1, TOP, PANTS, SUIT, STAFF1, 
	EARRING, CLOAK, GLOVES, NECKLACE2, SHOES, STAFF2,
	ITEMSORT_END, 
	POTION, SKILL_ICON
};

#endif
