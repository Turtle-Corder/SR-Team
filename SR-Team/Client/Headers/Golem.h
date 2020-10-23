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
		GOLEM_ROOT = GOLEM_BASE,
		GOLEM_CENTER,
		GOLEM_BODY,
		GOLEM_HEAD,
		GOLEM_LEFT_ARM,
		GOLEM_RIGHT_ARM,
		GOLEM_LEFT_LEG,
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
	//HRESULT	Late_Setup();
	HRESULT Attack(_float _fDeltaTime);
	HRESULT Add_Component();
	HRESULT Movement(float _fDeltaTime);
	HRESULT IsOnTerrain();
	HRESULT Setting_Part();
	HRESULT Setting_Center();
	HRESULT Create_Bomb(_float _fDeltaTime);
	HRESULT Create_MonSub(_float _fDeltaTime);
	HRESULT Setup_LayerBomb(const wstring & LayerTag, _vec3 _vPos);
	HRESULT Setup_Layer_MonSub(const wstring & LayerTag);
private:
	CVIBuffer*		m_pVIBufferCom[GOLEM_END] = {};
	CTransform*		m_pTransformCom[GOLEM_END] = {};
	CTexture*		m_pTextureCom[GOLEM_END] = {};

	//----------------------------------------------
	//----------------------------------------------
	_float			m_fAttackTime = 0.f;
	_int			m_iRandAttack = -1;
	_uint			m_iCreateCnt = 0;
	_vec3			m_vStartPos = {};
	_vec3			m_vTemp = {};
	_vec3 			m_vRand[5] = {};
	_bool			m_bDead = false;
	_bool			m_bAttack = false;
	_bool			m_bAttackStart = false;
	_bool			m_bAttackDelay = false;
	_bool			m_bCreate_Bomb = false;
	_bool			m_bMonSubOn = false;
	//list<CGameObject*> m_vecSub = {}; //핑크빈에서 쓸거
	//_bool			m_bLateSetUp = false;
	// 기준이 될녀석	=> Transform만
	// 나머지는 싹다
};


END
#endif //__GOLEM_H__