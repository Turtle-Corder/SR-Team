#pragma once
#ifndef __GOLEM_H__
#define __GOLEM_H__
#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CGolem final : public CGameObject
{
enum GOLEM
{
	GOLEM_BASE,
	GOLEM_BODY = GOLEM_BASE,
	GOLEM_HEAD ,
	GOLEM_LEFT_ARM ,
	GOLEM_RIGHT_ARM,
	GOLEM_LEFT_LEG ,
	GOLEM_RIGHT_LEG,
	GOLEM_END
};
private:
	explicit CGolem(LPDIRECT3DDEVICE9 _pDevice);
	explicit CGolem(const CGolem& _rOther);
	virtual ~CGolem() = default;
public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;
public:
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual void Free() override;
	static  CGolem* Create(LPDIRECT3DDEVICE9 _pDevice);
private:
	HRESULT Add_Component();
	HRESULT Movement(float _fDeltaTime);
	HRESULT IsOnTerrain();
	HRESULT Setting_Part();
private:
	CVIBuffer*		m_pVIBufferCom[GOLEM_END];
	CTransform*		m_pTransformCom[GOLEM_END];
	CTexture*		m_pTextureCom[GOLEM_END];


	_vec3			m_vStartPos = {};
	_bool			m_bDead = false;
};


END
#endif //__GOLEM_H__