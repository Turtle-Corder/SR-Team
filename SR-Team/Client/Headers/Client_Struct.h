#pragma once
#ifndef __CLIENT_STRUCT_H__
#define __CLIENT_STRUCT_H__

#include "Status.h"
USING(Engine)

typedef struct tagInvenItem
{
	WCHAR			szItemTag[MIN_STR];
	eITEM_SORT		eSort = eITEM_SORT::ITEMSORT_END;
	int				iCnt = 0;
	int				iPrice = 0;
	int				iInvneInsertOrder = -1;
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


typedef struct tagFileInfo
{
	TCHAR wstrFilePath[MID_STR];
	TCHAR wstrDimenKey[MID_STR];
	TCHAR wstrLayerKey[MID_STR];
	TCHAR wstrObjectKey[MID_STR];
	TCHAR wstrSaveTypeKey[MID_STR];
	int		iCount;
}MYFILEINFO;


//----------------------------
// 
//----------------------------
#endif // __CLINET_STRUCT_H__
