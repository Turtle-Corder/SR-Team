#pragma once
#ifndef __DROPITEM_H__
#define __DROPITEM_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CStatus;
class CDropItem final : public CGameObject
{
private:
	explicit CDropItem(LPDIRECT3DDEVICE9 pDevice);
	explicit CDropItem(const CDropItem& other);
	virtual ~CDropItem() = default;

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
	HRESULT Move(float _fDeltaTime);

public:
	static CDropItem* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone_GameObject(void * pArg) override;
	virtual void Free() override;



private:
	CVIBuffer*	m_pVIBufferCom = nullptr;
	CTransform*	m_pTransformCom = nullptr;
	CTexture*	m_pTextureCom = nullptr;
	CStatus*	m_pStatCom = nullptr;

	bool		m_bDead = false;
	_vec3		m_vPos = { 0.f , 0.f , 0.f };
	int			m_iRand = 0;
};

END

#endif // !__DROPITEM_H__
