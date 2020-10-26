#pragma once
#ifndef __FLOOR_H__
#define __FLOOR_H__

#include "GameObject.h"

BEGIN(Client)
USING(Engine)

class CFloor final : public CGameObject
{
private:
	explicit CFloor(LPDIRECT3DDEVICE9 _pDevice);
	explicit CFloor(const CFloor& _rOther);
	virtual ~CFloor() = default;

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void* _pArg) override;

	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;

private:
	HRESULT Add_Component();

public:
	static CFloor* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject* Clone_GameObject(void* _pArg) override;
	virtual void Free() override;



private:
	CVIBuffer*	m_pVIBufferCom = nullptr;
	CTexture*	m_pTextureCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	_vec3		m_vPos = {};
};

END

#endif //__FLOOR_H__
