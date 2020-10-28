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
	enum CAMERA_EVNETMODE
	{
		CAMERA_NORMAL, CARERA_WIGGING, CAMERA_DYNAMIC, MODE_END
	};

	enum CAMERA_VIEWMODE
	{
		CAMERA_3D, CAMERA_2D_X, CAMERA_2D_Z, VIEWMODE_END
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


	//-----------------------------------------------------------------------
	//Mag = ������ ��, Fre = ���� �󵵼�, Time = ���� �ð�, Option = ���� Ÿ��
	// ī�޶� �����Լ�, Option�� 2�� �ѱ��� ����, ������ �������� ������ �� ����.
	//DAMPED : ��������(������ ���� ������ �����ϴ� ������ ������ �������� ����)
	//HARMONIC : ��ȭ����(�ð����� ������ ������ ����)
	//MIXED : �����ȴ� ����
	//-----------------------------------------------------------------------
	HRESULT Set_Camera_Wigging(_float _Magnitude, _float _Frequency, _float _Time, WIG_TYPE Option = WIG_TYPE::DAMPED);
	HRESULT Set_Camera_Mode(CAMERA_VIEWMODE _ModeOption, void* _Option = nullptr);
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

	//-----------------------------------------------------------
	//3D State����
	//-----------------------------------------------------------
	_float m_fZoomInOutSpeedPerSecond = 10.f;
	_float m_fCameraAngle = D3DXToRadian(45.f);
	_float m_fUpDownSpeedPerSecond = D3DXToRadian(45.f);
	_float m_fStartHeight = 2.f;
	_float m_fStartDistance = 1.f;
	_float m_fHeight = m_fStartHeight;
	_float m_fDistance = m_fStartDistance;


	//-----------------------------------------------------------
	//2D State����
	//-----------------------------------------------------------
	_float m_fDistanceOn2DView = 3.f;

	CAMERA_EVNETMODE m_eEventMode = CAMERA_EVNETMODE::CAMERA_NORMAL;
	CAMERA_VIEWMODE m_eViewMode = CAMERA_VIEWMODE::CAMERA_3D;
	SHAKEINFO m_tShakeInfo;
};

END

#endif
