#pragma once
#ifndef __MAINAPP_H__
#define __MAINAPP_H__

#include "Base.h"

BEGIN(Client)

USING(Engine)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT Setup_MainApp();
	_int	Update_MainApp();
	HRESULT Render_MainApp();

private:
	HRESULT Setup_DefaultSetting();
	HRESULT Setup_StaticResources();

public:
	static CMainApp* Create();
	virtual void Free() override;
	
private:
	CManagement*		m_pManagement	= nullptr;
	LPDIRECT3DDEVICE9	m_pDevice		= nullptr;
};

END

#endif
