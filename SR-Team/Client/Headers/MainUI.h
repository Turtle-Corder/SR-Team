#pragma once
#ifndef __MAINUI_H__
#define __MAINUI_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CMainUI : public CGameObject
{
private:
	explicit CMainUI(LPDIRECT3DDEVICE9 _pDevice);
	explicit CMainUI(const CMainUI& _rOther);
	virtual ~CMainUI() = default;

	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * pArg) override;
	virtual int Update_GameObject(float DeltaTime) override;
	virtual int LateUpdate_GameObject(float DeltaTime) override;
	virtual HRESULT Render_UI() override;

private:
	HRESULT Add_Component();

public:
	static CMainUI* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	CVIBuffer*		m_pVIBufferCom[MAINUI_END];
	CTransform*		m_pTransformCom[MAINUI_END];
	CTexture*		m_pTextureCom[MAINUI_END];

	D3DXVECTOR3		m_vPos[MAINUI_END];
	D3DXVECTOR3		m_vScale[MAINUI_END];

	bool			m_bRender = false;
};

END

#endif // !__MAINUI_H__
