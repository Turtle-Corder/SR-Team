#pragma once

#ifndef __PLAYERSKILL_H__
#define __PLAYERSKILL_H__

#include "UIObject.h"
BEGIN(Client)
USING(Engine)

class CPlayerSkill abstract : public CUIObject
{
protected:
	explicit CPlayerSkill(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CPlayerSkill(const CPlayerSkill& _rOther);
	virtual ~CPlayerSkill() = default;

protected:
	HRESULT Update_Delay(_float _fDeltaTime);

public:
	virtual _bool Can_UseSkill();
	virtual _bool Actual_UseSkill(void* _pArg = nullptr) = 0;

	_int Get_ConsumeMP();

public:
	virtual void Free() override;


protected:
	_int		m_iCanUseCnt = 0;
	_int		m_iMaxUseCnt = 0;

	_float		m_fDelayCnt = 0;
	_float		m_fEachDelay = 0;

	_int		m_iConsumeMP = 0;
};

END
#endif