#pragma once

#ifndef __METEOSKILL_H__
#define __METEOSKILL_H__

#include "PlayerSkill.h"
USING(Engine)
BEGIN(Client)

class CMeteoSkill : public CPlayerSkill
{
public:
	explicit CMeteoSkill(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CMeteoSkill(const CMeteoSkill& _rOther);
	virtual ~CMeteoSkill() = default;

public:
	// CPlayerSkill을(를) 통해 상속됨
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;

public:
	static CMeteoSkill* Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;

public:
	virtual HRESULT Use_Skill(float fDeltaTime) override;
};

END
#endif