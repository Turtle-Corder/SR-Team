#pragma once

#ifndef __PLAYERSKILL_H__
#define __PLAYERSKILL_H__

#include "UIObject.h"
BEGIN(Client)
USING(Engine)

class CPlayerSkill : public CUIObject
{
protected:
	explicit CPlayerSkill(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CPlayerSkill(const CPlayerSkill& _rOther);
	virtual ~CPlayerSkill() = default;

public:
	virtual HRESULT Use_Skill(float fDeltaTime) = 0;

public:
	virtual void Free() override;


protected:
	_bool				m_bInitial = true;
	_float				m_fAttTime = 0.f;
	_float				m_fStartTime = 0.f;
	_float				m_fEndTime = 0.f;

	_int				m_iProjectileCnt = 0;
};

END
#endif