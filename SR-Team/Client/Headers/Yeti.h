#pragma once
#ifndef __YETI_H__
#define __YETI_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CStatus;
class CDamageInfo;
class CYeti final : public CGameObject
{
	enum YETI { YETI_BODY, YETI_HEAD, YETI_LEFT, YETI_RIGHT, YETI_LEFTLEG, YETI_RIGHTLEG, YETI_END };

private:
	explicit CYeti(LPDIRECT3DDEVICE9 pDevice);
	explicit CYeti(const CYeti& other);
	virtual ~CYeti() = default;

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * pArg) override;
	virtual int Update_GameObject(float _fDeltaTime) override;
	virtual int LateUpdate_GameObject(float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;

private:
	HRESULT Add_Component();
	HRESULT Movement(float _fDeltaTime);
	HRESULT IsOnTerrain();
	HRESULT Setting_Part();
	HRESULT Moving(float _fDeltaTime);
	HRESULT Attack(float _fDeltaTime);

public:
	static CYeti* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone_GameObject(void * pArg) override;
	virtual void Free() override;
	HRESULT LookAtPlayer(float _fDeltaTime);
	HRESULT Create_Snow(const wstring& LayerTag);

private:
	CVIBuffer*		m_pVIBufferCom[YETI_END];
	CTransform*		m_pTransformCom[YETI_END];
	CTexture*		m_pTextureCom[YETI_END];
	CStatus*		m_pStatusCom = nullptr;
	

	_bool		m_bJump = false;
	_float		m_fJumpPower = 5.f;
	_float		m_fJumpTime = 0.f;
	_float		m_fAngle = 0.f;
	_bool		m_bAttack = false;
	_bool		m_bAttackDelay = false;
	_bool		m_bRHandDown = false;
	_bool		m_bHighestCheck = false;
	_float		m_fAttackTime = 0.f;
	_vec3		m_vStartPos = {};
};

END

#endif // !__YETI_H__
