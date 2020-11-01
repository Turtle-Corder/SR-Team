#pragma once
#ifndef __ICESTRIKESKILL_H__
#define __ICESTRIKESKILL_H__

#include "PlayerSkill.h"

USING(Engine)
BEGIN(Client)

class CIceStrikeSkill final : public CPlayerSkill
{
private:
	explicit CIceStrikeSkill(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CIceStrikeSkill(const CIceStrikeSkill& _rOther);
	virtual ~CIceStrikeSkill() = default;

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void* _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;

public:
	static CIceStrikeSkill* Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject* Clone_GameObject(void* _pArg) override;

public:
	virtual _bool Actual_UseSkill(void* _pArg = nullptr) override;

};

END

#endif
