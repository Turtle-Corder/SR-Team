#pragma once

#ifndef __REDPOTION_H__
#define __REDPOTION_H__

#include "PlayerItem.h"
USING(Engine)
BEGIN(Client)

class CRedPotion : public CPlayerItem
{
public:
	explicit CRedPotion(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CRedPotion(const CRedPotion& _rOther);
	virtual ~CRedPotion() = default;

public:
	// CPlayerItem��(��) ���� ��ӵ�
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;

public:
	static CRedPotion* Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;

public:
	virtual _bool Actual_UseItem() override;
};

END
#endif