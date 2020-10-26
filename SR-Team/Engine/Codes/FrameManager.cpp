#include "..\Headers\FrameManager.h"

USING(Engine)

IMPLEMENT_SINGLETON(CFrame_Manager)
CFrame_Manager::CFrame_Manager()
	: m_dSecondPerFrame(0.0), m_dDeltaTime(0.0), m_dElapseTime(0.0)
	, m_iFPS(0), m_szFPS(L""), m_dSecElapseTime(0.0)
{
}

HRESULT CFrame_Manager::Setup_FrameManager(HWND _hWnd, _uint _iFramePerSec)
{
	if (nullptr == _hWnd)
		return E_FAIL;

	m_hWnd = _hWnd;
	m_dSecondPerFrame = 1.0 / _iFramePerSec;

	QueryPerformanceFrequency(&m_Frequency);

	QueryPerformanceCounter(&m_PrevTime);
	QueryPerformanceCounter(&m_CurrTime);

	return S_OK;
}

_bool CFrame_Manager::Lock_FrameManager()
{
	// �ð� ����
	QueryPerformanceCounter(&m_CurrTime);

	// �󸶳� �귶���� �ð��� ���Ѵ�.
	double dCurrElapseTime = (double)(m_CurrTime.QuadPart - m_PrevTime.QuadPart) / m_Frequency.QuadPart;

	// �帥 �ð��� �����Ѵ�.
	m_dElapseTime += dCurrElapseTime;

	// ���� �ð��� ���� �ð��� ����
	m_PrevTime = m_CurrTime;

	// 1ȸ Update �ϱ� ���� �ð��� �����Ǿ����� 
	if (m_dSecondPerFrame < m_dElapseTime)
	{
		// 1. �帥 �ð��� ���(����)�صΰ�,
		m_dDeltaTime = m_dElapseTime;

		// 2. �帥 �����ð��� �ʱ�ȭ ���ش�.
		// ���Ҹ� ��Ű�� ��� �и�
		//m_dElapseTime -= m_dSecondPerFrame;
		m_dElapseTime = 0.0;

		// ������Ʈ �����̶�� �˷��ش�. return true
		return true;
	}

	return false;
}

void CFrame_Manager::Render_FrameManager()
{
	m_dSecElapseTime += m_dDeltaTime;
	++m_iFPS;
	if (m_dSecElapseTime > 1.0)
	{
		StringCchPrintf(m_szFPS, _countof(m_szFPS), L"FPS:%d", m_iFPS);
		m_dSecElapseTime -= 1.0;
		m_iFPS = 0;

		SetWindowText(m_hWnd, m_szFPS);
	}
}

void CFrame_Manager::Free()
{
}
