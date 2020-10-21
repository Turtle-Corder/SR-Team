#pragma once
#ifndef __SLIME_H__
#define __SLIME_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CStatus;
class CSlime final : public CGameObject
{
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



private:
	CVIBuffer*	m_pVIBufferCom = nullptr;
	CTransform*	m_pTransformCom = nullptr;
	CTexture*	m_pTextureCom = nullptr;
//	CStatus*	m_pStatCom = nullptr;

	bool		m_bDead;
	bool		m_bJump = false;
	float		m_fJumpPower = 5.f;
	float		m_fJumpTime = 0.f;
	int			m_iMaxCount = 4;
	int			m_iCurCount = 1;
};

END

#endif // !__SLIME_H__