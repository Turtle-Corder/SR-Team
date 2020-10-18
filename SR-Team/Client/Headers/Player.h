#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "GameObject.h"

BEGIN(Client)
USING(Engine)

class CPlayer final : public CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 _pDevice);
	explicit CPlayer(const CPlayer& _rOther);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void* _pArg) override;

	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;

private:
	HRESULT Add_Component();

	HRESULT IsOnTerrain();
	HRESULT Movement(_float _fDeltaTime);
	void Move_Vertical(_float _fDeltaTime);
	void Move_Horizontal(_float _fDeltaTime);
	void Turn(_float _fDeltaTime);

	HRESULT RaycastOnTerrain();
	void Move_Target(_float _fDeltaTime);

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject* Clone_GameObject(void* _pArg) override;
	virtual void Free() override;



private:
	CVIBuffer*	m_pVIBufferCom	= nullptr;
	CTransform* m_pTransformCom = nullptr;
	CTexture*	m_pTextureCom	= nullptr;
	CRaycast*	m_pRaycastCom	= nullptr;

	bool		m_bIsTagetMove = false;
	_vec3		m_vTargetPos = {};
};

END

#endif // !__PLAYER_H__
