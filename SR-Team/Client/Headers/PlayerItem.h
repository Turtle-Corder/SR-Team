#pragma once

#ifndef __PLAYERITEM_H__
#define __PLAYERITEM_H__

#include "UIObject.h"
BEGIN(Client)
USING(Engine)

class CPlayerItem : public CUIObject
{
public:
	explicit CPlayerItem(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CPlayerItem(const CPlayerItem& _rOther);
	virtual ~CPlayerItem() = default;

public:
	virtual HRESULT Use_Item() = 0;

public:
	virtual void Free() override;
};

END
#endif