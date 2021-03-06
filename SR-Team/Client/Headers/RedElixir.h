#pragma once

#ifndef __REDELIXER_H__
#define __REDELIXER_H__

#include "PlayerItem.h"
USING(Engine)
BEGIN(Client)

class CRedElixir : public CPlayerItem
{
public:
	explicit CRedElixir(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CRedElixir(const CRedElixir& _rOther);
	virtual ~CRedElixir() = default;

public:
	// CPlayerItem을(를) 통해 상속됨
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;

public:
	static CRedElixir* Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;

public:
	virtual HRESULT Use_Item() override;
};

END
#endif