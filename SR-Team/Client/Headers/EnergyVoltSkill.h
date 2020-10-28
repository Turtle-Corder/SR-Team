#pragma once

#ifndef __ENERGYVOLTSKILL_H__
#define __ENERGYVOLTSKILL_H__

#include "PlayerSkill.h"
USING(Engine)
BEGIN(Client)

class CEnergyVoltSkill : public CPlayerSkill
{
public:
	explicit CEnergyVoltSkill(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CEnergyVoltSkill(const CEnergyVoltSkill& _rOther);
	virtual ~CEnergyVoltSkill() = default;

public:
	// CPlayerSkill을(를) 통해 상속됨
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;

public:
	static CEnergyVoltSkill* Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;

public:
	virtual HRESULT Use_Skill(float fDeltaTime) override;
};

END
#endif