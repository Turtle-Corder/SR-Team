#pragma once
#ifndef __INSTANT_IMPACT_H__
#define __INSTANT_IMPACT_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CDamageInfo;

class CInstant_Impact final : public CGameObject
{
private:
	explicit CInstant_Impact(LPDIRECT3DDEVICE9 _pDevice);
	explicit CInstant_Impact(const CInstant_Impact& _rOther);
	virtual ~CInstant_Impact() = default;
public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
private:
	HRESULT Add_Component();
public:
	virtual CGameObject * Clone_GameObject(void * _pArg) override;
	virtual void Free() override;
	static CInstant_Impact* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual HRESULT Take_Damage(const CComponent* _pDamageComp) override;



private:
	CTransform*		m_pTransformCom = nullptr;
	CCollider*		m_pColliderCom = nullptr;
	CDamageInfo*	m_pDmgInfoCom = nullptr;


	_vec3			m_vPos = {};
	_bool			m_bDead = false;
	INSTANTIMPACT	m_pInstant;
};

END

#endif //__INSTANT_IMPACT_H__