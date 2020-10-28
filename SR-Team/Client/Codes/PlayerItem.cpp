#include "stdafx.h"
#include "..\Headers\PlayerItem.h"


USING(Client)


CPlayerItem::CPlayerItem(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
}

CPlayerItem::CPlayerItem(const CPlayerItem & _rOther)
	: CUIObject(_rOther)
{
}

HRESULT CPlayerItem::Use_Item()
{
	return S_OK;
}

void CPlayerItem::Free()
{
	CUIObject::Free();
}
