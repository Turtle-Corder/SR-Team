#pragma once
#ifndef __STATUS_H__
#define __STATUS_H__

#include "Component.h"

USING(Engine)
BEGIN(Client)

class CStatus final : public CComponent
{
public:
	typedef struct tagStat
	{
		_int	iMinAtt = 0;
		_int	iMaxAtt = 0;
		_int	iCriticalRate = 0;
		_int	iCriticalHit = 0;
		_int	iDef = 0;
		_int	iMaxHp = 0;
		_int	iMaxMp = 0;
		_int	iHp = 0;
		_int	iMp = 0;
		_int	iSort = 0;

		//_bool	bShop = false;
		//_int	iShopCnt = 0;
		//_int	iPrice = 0;
		//_vec3	vShopPos = { 0.f, 0.f, 0.f };
		//_vec3	vShopNamePos = { 0.f, 0.f, 0.f };
		//_vec3	vShopPricePos = { 0.f, 0.f, 0.f };
		//_vec3	vShopScale = { 80.f, 90.f, 1.f };
		//_vec3	vShopNameScale = { 80.f, 90.f, 1.f };
		//_vec3	vShopPriceScale = { 80.f, 90.f, 1.f };

		//_bool	bInven = false;
		//_int	iInvenCnt = 0;
		//_vec3	vInvenPos = { 0.f, 0.f, 0.f };
		//_vec3	vInvenCntPos = { 0.f, 0.f, 0.f };
		//_vec3	vInvenScale = { 80.f, 90.f, 1.f };
		//_vec3	vInvenCntScale = { 80.f, 90.f, 1.f };
	} STAT;

private:
	explicit CStatus(LPDIRECT3DDEVICE9 _pDevice);
	explicit CStatus(const CStatus& _rOther);
	virtual ~CStatus() = default;

public:
	const STAT& Get_Status() const;

public:
	void Set_Status(const STAT& _rStatus);
	void Set_HP(_int _iAtt);
	void Set_MP(_int _iMP);

public:
	virtual HRESULT Setup_Component_Prototype() override;
	virtual HRESULT Setup_Component(void* _pArg) override;

public:
	static CStatus* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CComponent * Clone_Component(void * _pArg) override;
	virtual void Free() override;
public:
	//s
	const STAT& Get_Status() { return m_tStat; }


private:
	STAT m_tStat;
};

END

#endif
