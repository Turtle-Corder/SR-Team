#pragma once
#ifndef __VIBUFFER_TRICOLOR_H__
#define __VIBUFFER_TRICOLOR_H__

#include "VIBuffer.h"

BEGIN(Engine)

class CVIBuffer_TriColor final : public CVIBuffer
{
private:
	explicit CVIBuffer_TriColor(LPDIRECT3DDEVICE9 _pDevice);
	explicit CVIBuffer_TriColor(const CVIBuffer_TriColor& _rOther);
	virtual ~CVIBuffer_TriColor() = default;

public: 
	virtual HRESULT Setup_Component_Prototype() override;
	virtual HRESULT Setup_Component(void* _pArg) override;

public:
	static CVIBuffer_TriColor* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CComponent* Clone_Component(void* _pArg) override;
	virtual void Free() override;
};

END

#endif
