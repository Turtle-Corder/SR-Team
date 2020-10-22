#pragma once
#ifndef __SLIME_H__
#define __SLIME_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CStatus;
class CSlime final : public CGameObject
{
	enum SLIME 
	{ 
		SLIME_BODY,
		SLIME_BASE,
		SLIME_JELLY = SLIME_BASE,
		SLIME_END 
	};
private:
	explicit CSlime(LPDIRECT3DDEVICE9 pDevice);
	explicit CSlime(const CSlime& other);
	virtual ~CSlime() = default;

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_BlendAlpha() override;
	virtual HRESULT Render_NoneAlpha() override;

private:
	HRESULT Add_Component();
	HRESULT Movement(float _fDeltaTime);
	HRESULT IsOnTerrain();
	void	Jumping(float _fDeltaTime);
	HRESULT LookAtPlayer(float _fDeltaTime);
	HRESULT Divide_Cube(const wstring& LayerTag);
public:
	static CSlime* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone_GameObject(void * pArg) override;
	virtual void Free() override;
	HRESULT Create_Item(const wstring& LayerTag);
	HRESULT Setting_SlimeBody();
	HRESULT Setting_SlimeJelly();

private:
	CVIBuffer*	m_pVIBufferCom[SLIME_END] = {};
	CTransform*	m_pTransformCom[SLIME_END] = {};
	CTexture*	m_pTextureCom = nullptr;
//	CStatus*	m_pStatCom = nullptr;

	_bool		m_bDead = false;
	_bool		m_bJump = false;
	_float		m_fJumpPower = 5.f;
	_float		m_fJumpTime = 0.f;
	_int			m_iMaxCount = 4;
	_int			m_iCurCount = 1;
	_vec3		m_vStartPos = {};
};

END

#endif // !__SLIME_H__
