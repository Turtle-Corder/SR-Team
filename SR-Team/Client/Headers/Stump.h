#pragma once
#ifndef __STUMP_H__
#define __STUMP_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CStump final : public CGameObject
{
private:
	explicit CStump(LPDIRECT3DDEVICE9 _pDevice);
	explicit CStump(const CStump& _rOther);
	virtual ~CStump() = default;
public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
public:

};

END

#endif //__STUMP_H__