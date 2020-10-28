#include "stdafx.h"
#include "..\Headers\PlayerSkill.h"


USING(Client)


CPlayerSkill::CPlayerSkill(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
}

CPlayerSkill::CPlayerSkill(const CPlayerSkill & _rOther)
	: CUIObject(_rOther)
{
}

HRESULT CPlayerSkill::Use_Skill(float fDeltaTime)
{
	return S_OK;
}

void CPlayerSkill::Free()
{
	CUIObject::Free();
}
