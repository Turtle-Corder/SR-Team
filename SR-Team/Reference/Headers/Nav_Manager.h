#pragma once
#ifndef __NAV_MANAGER_H__
#define __NAV_MANAGER_H__

#include "Base.h"

BEGIN(Engine)
class CNavAgent;
class CNavigation_Manager : public CBase
{
private:
	explicit CNavigation_Manager(LPDIRECT3DDEVICE9 _rDevice);
	virtual ~CNavigation_Manager() = default;

public:
	HRESULT Setup_Navigation_Manager();
	HRESULT Add_NavAgent();
};

END

#endif // !__NAV_MANAGER_H__
