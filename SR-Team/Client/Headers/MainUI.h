#pragma once
#ifndef __MAINUI_H__
#define __MAINUI_H__

#include "UIObject.h"

USING(Engine)
BEGIN(Client)

class CMainUI : public CUIObject
{
private:
	explicit CMainUI(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
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
	static CMainUI* Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	// Component-------------------------------------------------------------
	CVIBuffer*		m_pVIBufferCom[MAINUI_END];
	CTransform*		m_pTransformCom[MAINUI_END];
	CTexture*		m_pTextureCom[MAINUI_END];

	// 위치, 크기-------------------------------------------------------------
	_vec3			m_vPos[MAINUI_END];
	_vec3			m_vScale[MAINUI_END];

	// 충돌 렉트
	RECT			m_tCollRt[MAINUI_END];
};
END

#endif // !__MAINUI_H__
