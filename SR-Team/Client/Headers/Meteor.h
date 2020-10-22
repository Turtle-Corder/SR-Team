#pragma once
#ifndef __METEOR_H__
#define __METEOR_H__

#include"GameObject.h"

USING(Engine)
BEGIN(Client)

class CStatus;
class CMeteor final : public CGameObject
{
private:
	explicit CMeteor(LPDIRECT3DDEVICE9 pDevice);
	explicit CMeteor(const CMeteor& other);
	virtual ~CMeteor() = default;

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void* pArg) override;
	virtual int Update_GameObject(float _fDeltaTime) override;
	virtual int LateUpdate_GameObject(float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;

private:
	HRESULT Add_Component();
	HRESULT Movement(float _fDeltaTime);
	HRESULT FallDown_Meteor(float _fDeltaTime);
	HRESULT IsOnTerrain();
public:
	static CMeteor* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;



private:
	CVIBuffer*	m_pVIBufferCom = nullptr;
	CTransform*	m_pTransformCom = nullptr;
	CTexture*	m_pTextureCom = nullptr;
	
	bool  m_bDead = false;
	_vec3 m_vPlayerPos = {};
	_vec3 m_vGoalPos = {};
	float m_fDownTime = 0.f;
};

END

#endif // !__METEOR_H__
