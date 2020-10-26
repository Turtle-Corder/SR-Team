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
	enum CAMERA_MODE
	{
		CAMERA_NORMAL, CARERA_WIGGING, CAMERA_2D, CAMERA_DYNAMIC, MODE_END
	};

	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void* _pArg) override;

	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;

private:
	HRESULT Movement(_float _fDeltaTime);
	HRESULT ZoomInOut(_float _fDeltaTime);
	HRESULT Wigging(_float _fDeltaTime);
	
	//	HRESULT UpDown(_float _fDeltaTime);

public:
	enum WIG_TYPE
	{
		DAMPED, HARMONIC, MIXED, WIG_END
	};

	// 카메라 진동함수, Option은 2를 넘기지 않음, 강도와 진동수는 음수일 수 없음.
	HRESULT Set_Camera_Wigging(_float _Magnitude, _float _Frequency, _float _Time, WIG_TYPE Option = WIG_TYPE::DAMPED);

public:
	static CMainCamera* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject* Clone_GameObject(void* _pArg) override;
	virtual void Free() override;



private:
	struct SHAKEINFO
	{
		_float m_fTimeFlow = 0.f;
		_float m_fSettingTime = 0.f;
		_float m_fMagnitude = 0.f;
		_float m_fFrequency = 0.f;
		WIG_TYPE m_eWigType = WIG_TYPE::DAMPED;
	};

	_float m_fZoomInOutSpeedPerSecond = 10.f;
	_float m_fCameraAngle = D3DXToRadian(45.f);
	_float m_fUpDownSpeedPerSecond = D3DXToRadian(45.f);
	_float m_fStartHeight = 4.f;
	_float m_fStartDistance = 1.f;
	_float m_fHeight = m_fStartHeight;
	_float m_fDistance = m_fStartDistance;
	CAMERA_MODE m_eMode = CAMERA_MODE::CAMERA_NORMAL;
	SHAKEINFO m_tShakeInfo;
};

END

#endif
