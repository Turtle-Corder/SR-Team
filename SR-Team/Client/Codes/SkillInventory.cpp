#include "stdafx.h"
#include "..\Headers\SkillInventory.h"


USING(Client)


CSkillInventory::CSkillInventory(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
}

CSkillInventory::CSkillInventory(const CSkillInventory & _rOther)
	: CGameObject(_rOther)
{
}

void CSkillInventory::Free()
{
	CGameObject::Free();
}
