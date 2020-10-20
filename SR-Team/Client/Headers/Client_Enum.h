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
// 인간 형태 파츠
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
// 플레이어 스킬 ID
//----------------------------------------------------------------------------------------------------
enum ePLAYER_SKILL_ID
{
	PLAYER_NORMAL_ATTACK,
	PLAYER_SKILL_LASER,
	PLAYER_SKILL_FALL,
	PLAYER_SKILL_END
};





//----------------------------------------------------------------------------------------------------
// 메인 UI
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
// 상점 UI
//--------------------------------------------------
enum eSHOP_ID
{
	SHOP_START,
	SHOP_WND = SHOP_START,
	SHOP_END
};


//--------------------------------------------------
// 인벤토리 UI
//--------------------------------------------------
enum eINVEN_ID
{
	INVEN_START,
	INVEN_WND = INVEN_START,
	INVEN_END
};





#endif
