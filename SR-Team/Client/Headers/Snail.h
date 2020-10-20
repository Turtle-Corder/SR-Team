#pragma once
#ifndef __SNAIL_H__
#define __SNAIL_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CSnail final : public CGameObject
{
	enum SNAIL { SNAIL_HEAD, SNAIL_BODY, SNAIL_END };

private:
	explicit CSnail(LPDIRECT3DDEVICE9 pDevice);
	explicit CSnail(const CSnail& other);
	virtual ~CSnail() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * pArg) override;
	virtual int Update_GameObject(float _fDeltaTime) override;
	virtual int LateUpdate_GameObject(float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;

private:
	HRESULT Add_Component();
	HRESULT Movement(float _fDeltaTime);
	HRESULT IsOnTerrain();
	HRESULT	Move(float _fDeltaTime);
	HRESULT LookAtPlayer(float _fDeltaTime);
	HRESULT Setting_Part();
public:
	static CSnail* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;



private:
	CVIBuffer*	m_pVIBufferCom[SNAIL_END] = {};
	CTransform*	m_pTransformCom[SNAIL_END] = {};
	CTexture*	m_pTextureCom[SNAIL_END] = {};
};

END

#endif // !__SNAIL_H__
