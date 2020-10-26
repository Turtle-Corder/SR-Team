#pragma once
#ifndef __SNAIL_H__
#define __SNAIL_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CStatus;
class CDamageInfo;

class CSnail final : public CGameObject
{
	enum SNAIL { SNAIL_HEAD, SNAIL_BODY, SNAIL_END };

private:
	explicit CSnail(LPDIRECT3DDEVICE9 _pDevice);
	explicit CSnail(const CSnail& _rOther);
	virtual ~CSnail() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * pArg) override;
	virtual int Update_GameObject(_float _fDeltaTime) override;
	virtual int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;

private:
	HRESULT Add_Component();
	HRESULT Movement(_float _fDeltaTime);
	HRESULT IsOnTerrain();
	HRESULT	Move(_float _fDeltaTime);
	HRESULT LookAtPlayer(_float _fDeltaTime);
	HRESULT Setting_Part();
	HRESULT SetUp_Layer_InstantImpact(const wstring& LayerTag);
public:
	static CSnail* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;
	virtual HRESULT Take_Damage(const CComponent* _pDamageComp) override;
private:
	CVIBuffer*		m_pVIBufferCom[SNAIL_END] = {};
	CTransform*		m_pTransformCom[SNAIL_END] = {};
	CTexture*		m_pTextureCom[SNAIL_END] = {};
	CCollider*		m_pColliderCom = nullptr;
	CStatus*		m_pStatusCom = nullptr;
	CDamageInfo*	m_pDmgInfoCom = nullptr;
private:
	_vec3		m_vStartPos = {};
	_bool		m_bAttack = false;
	_vec3		m_vDir = {};
	INSTANTIMPACT*	m_pInstantImpact = nullptr;
};

END

#endif // !__SNAIL_H__
