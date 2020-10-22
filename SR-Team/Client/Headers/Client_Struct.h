#pragma once
#ifndef __CLIENT_STRUCT_H__
#define __CLIENT_STRUCT_H__

USING(Engine)

typedef struct tagInvenItem
{
	WCHAR			szItemTag[MIN_STR];
	eITEM_SORT		eSort = SORT_END;
	int				iCnt = 0;
	int				iPrice = 0;
	int				iInvneInsertOrder = -1;
	_vec3			vInvenPos = { 0.f, 0.f, 0.f };
} INVEN_ITEM;



typedef struct tagTerrainInfo
{
	_int iFloor;
	_int iIndex;
	_int iTextureID;
	_int iX_Index;
	_int iZ_Index;
} TERRAININFO;



typedef struct tagDamageEvent
{
	_vec3			vPosition;
	CComponent*		pDmgInfoComp;
} DAMAGE_EVENT;


//----------------------------
// 
//----------------------------
#endif // __CLINET_STRUCT_H__
