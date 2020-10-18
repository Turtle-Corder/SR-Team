#pragma once
#ifndef __MAINCAMERA_H__
#define __MAINCAMERA_H__

#include "Camera.h"

BEGIN(Client)
USING(Engine)

class CMainCamera final : public CCamera
{
private:
	explicit CMainCamera(LPDIRECT3DDEVICE9 _pDevice);
	explicit CMainCamera(const CMainCamera& _rOther);
	virtual ~CMainCamera() = default;

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void* _pArg) override;

	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;

private:
	HRESULT Movement(_float _fDeltaTime);
	HRESULT ZoomInOut(_float _fDeltaTime);
//	HRESULT UpDown(_float _fDeltaTime);

public:
	static CMainCamera* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject* Clone_GameObject(void* _pArg) override;
	virtual void Free() override;



private:
	_float m_fDistanceToTarget = 10.f;
	_float m_fZoomInOutSpeedPerSecond = 10.f;
	_float m_fCameraAngle = D3DXToRadian(45.f);
	_float m_fUpDownSpeedPerSecond = D3DXToRadian(45.f);
};

END

#endif
