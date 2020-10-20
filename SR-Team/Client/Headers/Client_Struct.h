#pragma once
#ifndef __CLIENT_STRUCT_H__
#define __CLIENT_STRUCT_H__



typedef struct tagInvenItem
{
	WCHAR	szItemTag[MIN_STR];
	int		iSort = -1;
	int		iCnt = 0;
	int		iPrice = 0;
	int		iInvneInsertOrder = -1;
	_vec3	vInvenPos = { 0.f, 0.f, 0.f };
} INVEN_ITEM;



typedef struct tagTerrainInfo
{
	_int iFloor;
	_int iIndex;
	_int iTextureID;
	_int iX_Index;
	_int iZ_Index;
} TERRAININFO;




#endif // __CLINET_STRUCT_H__
