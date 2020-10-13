#pragma once
#ifndef __MANAGEMENT_H__
#define __MANAGEMENT_H__

#include "Device_Manager.h"
#include "Scene_Manager.h"

BEGIN(Engine)

class ENGINE_DLL CManagement final : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	CManagement(void);
	virtual ~CManagement(void) = default;



	//----------------------------------------------------------------------------------------------------
	// �Ϲ� �Լ�
	//----------------------------------------------------------------------------------------------------
public:
	HRESULT Setup_Engine(HWND _hWnd, _uint _iWinCX, _uint _iWinCY, CDevice_Manager::DISPLAY_MODE _eDisplayMode, _uint _iSceneCnt, const wstring& _strAppTimerTag);
	_int	Update_Engine(void);
	HRESULT Render_Engine(void);
	static _uint Release_Engine();



	//----------------------------------------------------------------------------------------------------
	// �������̽�
	//----------------------------------------------------------------------------------------------------
public:
	//--------------------------------------------------
	// ����
	//--------------------------------------------------
	HRESULT Clear_ForScene(_uint _iSceneID);


	//--------------------------------------------------
	// ��ġ
	//--------------------------------------------------
	LPDIRECT3DDEVICE9 Get_Device(void) const;


	//--------------------------------------------------
	// Ÿ�̸�
	//--------------------------------------------------
	HRESULT Add_Timer(const wstring& _strTimerTag, bool _bStart = true);
	HRESULT Get_DeltaTime(const wstring& _strTimerTag, float& _fDeltaTime);

	HRESULT Pause(const wstring& _strTimerTag);
	HRESULT UnPause(const wstring& _strTimerTag);


	//--------------------------------------------------
	// ������
	//--------------------------------------------------
//	HRESULT Add_RendererList(CRender)


	//--------------------------------------------------
	// �ݶ��̴�
	//--------------------------------------------------
//	HRESULT Add_ColliderList();


	//--------------------------------------------------
	// ��
	//--------------------------------------------------
	HRESULT Change_CurrentScene(_uint _iSceneID, CScene* pCurrentScene);


	//--------------------------------------------------
	// ������Ʈ
	//--------------------------------------------------
//	CGameObject*


	//--------------------------------------------------
	// ������Ʈ
	//--------------------------------------------------



public:
	virtual void Free() override;



	//----------------------------------------------------------------------------------------------------
	// ����
	//----------------------------------------------------------------------------------------------------
private:
	CDevice_Manager*	m_pDevice_Manager = nullptr;
	CScene_Manager*		m_pScene_Manager = nullptr;

	_int				m_iUpdate_Result = 0;
	wstring				m_strAppTimerTag;
};

#endif
