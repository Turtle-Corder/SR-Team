#pragma once
#ifndef __SNOW_H__
#define __SNOW_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CStatus;
class CSnow final : public CGameObject
{
private:
	explicit CSnow(LPDIRECT3DDEVICE9 pDevice);
	explicit CSnow(const CSnow& other);
	virtual ~CSnow() = default;

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * pArg) override;
	virtual int Update_GameObject(float _fDeltaTime) override;
	virtual int LateUpdate_GameObject(float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;
private:
	HRESULT Add_Component();
	HRESULT Movement(float _fDeltaTime);
	HRESULT Setting_Dir();
public:
	static CSnow* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;
public:
	HRESULT Throwing_Snow(float _fDeltaTime);



private:
	CVIBuffer*	m_pVIBufferCom = nullptr;
	CTransform*	m_pTransformCom = nullptr;
	CTexture*	m_pTextureCom = nullptr;
	CStatus*	m_pStatCom = nullptr;

	bool		m_bDead = false;
	bool		m_bJump = false;
	float		m_fJumpPower = 1.1f;
	float		m_fJumpTime = 0.f;
	_vec3		m_vPos = {};
	_vec3		m_vDir = {};
	bool		m_bCheck = false;
	bool		m_bFallDown = false;
	bool		m_bGround = false;
	float		m_fGravity = 0.f;
	_vec3		m_vCompare = {};
};

END

#endif // !__SNOW_H__
