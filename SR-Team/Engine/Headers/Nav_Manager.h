#pragma once
#ifndef __NAV_MANAGER_H__
#define __NAV_MANAGER_H__

#include "Base.h"

BEGIN(Engine)
class CGameObject;
class CNavigation_Manager : public CBase
{
	DECLARE_SINGLETON(CNavigation_Manager)

private:
	CNavigation_Manager();
	virtual ~CNavigation_Manager() = default;

public:
	HRESULT Setup_Navigation_Manager();
	
	// component는 어차피 검색해서 얻어오면 되고,
	// refcnt도 올려야 하니 gameobject를 인자로 받자
	HRESULT Add_NavAgent(const wstring& _strNavAgentTag, CGameObject* _pGameObject);

public:
	virtual void Free() override;


private:
	queue<CGameObject*>	m_QueueWait;
	queue<CGameObject*>	m_QueueDone;
};

END

#endif // !__NAV_MANAGER_H__
