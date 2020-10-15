#pragma once
#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CBase
{
private:
	explicit CComponent(LPDIRECT3DDEVICE9 _pDevice);
	explicit CComponent(const CComponent& rOther);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Setup_Component_Prototype() = 0;
	virtual HRESULT Setup_Component(void* pArg) = 0;

public:
	virtual CComponent* Clone_Component(void* pArg) = 0;
	virtual void Free(void) override;



protected:
	LPDIRECT3DDEVICE9 m_pDevice;
};

END

#endif

