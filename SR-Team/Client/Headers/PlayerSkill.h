#pragma once

#ifndef __PLAYERSKILL_H__
#define __PLAYERSKILL_H__

#include "GameObject.h"
BEGIN(Client)
USING(Engine)

class CPlayerSkill : public CGameObject
{
protected:
	explicit CPlayerSkill(LPDIRECT3DDEVICE9 pDevice);
	explicit CPlayerSkill(const CPlayerSkill& _rOther);
	virtual ~CPlayerSkill() = default;

public:
	virtual HRESULT Use_Skill(_float fDeltaTime, CTransform& rPlayerRight) = 0;

public:
	virtual void Free() override;


protected:
	_float				m_fAttTime = 0.f;
	_float				m_fStartTime = 0.f;
	_float				m_fEndTime = 0.f;
};

END
#endif