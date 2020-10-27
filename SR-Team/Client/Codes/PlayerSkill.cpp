#include "stdafx.h"
#include "..\Headers\PlayerSkill.h"


USING(Client)


CPlayerSkill::CPlayerSkill(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
}

CPlayerSkill::CPlayerSkill(const CPlayerSkill & _rOther)
	: CGameObject(_rOther)
{
}

void CPlayerSkill::Free()
{
	CGameObject::Free();
}
