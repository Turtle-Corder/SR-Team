#pragma once
#ifndef __FRAME_MANAGER_H__
#define __FRAME_MANAGER_H__

#include "Base.h"

BEGIN(Engine)

class CFrame_Manager final : public CBase
{
	DECLARE_SINGLETON(CFrame_Manager)

private:
	CFrame_Manager();
	virtual ~CFrame_Manager() = default;

public:
	HRESULT Setup_FrameManager(HWND _hWnd, _uint _iFramePerSec);

	_bool Lock_FrameManager();

	void Render_FrameManager();

public:
	virtual void Free() override;



private:
	LARGE_INTEGER m_Frequency;
	LARGE_INTEGER m_PrevTime;
	LARGE_INTEGER m_CurrTime;

	// 1ȸ update �ϱ� ���� �ʿ��� ������ �ð�
	double m_dSecondPerFrame;

	// ���� ������Ʈ ������ ���� ������Ʈ ���� ������ �ð�
	double m_dDeltaTime;

	// 1ȸ update �ϱ� ���� �帥 �ð��� ����
	double m_dElapseTime;


	//--------------------------------------------------
	// ��¿�
	//--------------------------------------------------
	HWND	m_hWnd = NULL;
	int		m_iFPS;
	TCHAR	m_szFPS[32];

	// 1�ʸ� Ȯ�� �ϱ� ���� ����
	double m_dSecElapseTime;

};

END

#endif // !__FRAME_MANAGER_H__
