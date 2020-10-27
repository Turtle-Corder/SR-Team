#pragma once

#ifndef __NORMALATT_H__
#define __NORMALATT_H__

#include "PlayerSkill.h"
BEGIN(Client)
USING(Engine)

class CNormalAtt : public CPlayerSkill
{
public:
	explicit CNormalAtt(LPDIRECT3DDEVICE9 _pDevice);
	explicit CNormalAtt(const CNormalAtt& _rOther);
	virtual ~CNormalAtt() = default;

public:
	// CPlayerSkill을(를) 통해 상속됨
	virtual HRESULT Use_Skill(_float fDeltaTime, CTransform& rPlayerRight) override;

private:
	_bool		m_bRightAtt = false;
	_bool		m_bLeftAtt = false;
	_bool		m_bIsNormalAtt = false;
};

END
#endif