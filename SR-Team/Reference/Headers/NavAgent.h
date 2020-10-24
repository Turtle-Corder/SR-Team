#pragma once
#ifndef __NAV_AGENT_H__
#define __NAV_AGENT_H__

#include "Component.h"

BEGIN(Engine)

class CNavAgent final : public CComponent
{
public:
	typedef struct tagNavDesc
	{
		_ivec2	tStart;
		_ivec2	tGoal;
	} NAV_DESC;

private:
	explicit CNavAgent(LPDIRECT3DDEVICE9 _pDevice);
	explicit CNavAgent(const CNavAgent& _rOther);
	virtual ~CNavAgent() = default;

public:
	virtual HRESULT Setup_Component_Prototype() override;
	virtual HRESULT Setup_Component(void * _pArg) override;

	void Set_NavDesc(const NAV_DESC& _tNavDesc);
	void Set_NavDesc(_int _iStartX, _int _iStartY, _int _iGoalX, _int _iGoalY);
	void Set_NavDesc(const _ivec2& _rStart, const _ivec2& _rGoal);	
	void Add_Branch(const _ivec2& _rBranch);

public:
	const NAV_DESC& Get_NavDesc() const;
	bool Get_NextGoal(_ivec2* _pBranch);

public:
	static CNavAgent* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CComponent * Clone_Component(void * _pArg) override;
	virtual void Free() override;



private:
	NAV_DESC		m_tNavDesc;
	list<_ivec2>	m_BranchList;
};

END

#endif // !__NAV_AGENT_H__
