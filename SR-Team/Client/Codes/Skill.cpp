#include "stdafx.h"
#include "..\Headers\Skill.h"

#include "KeyManager.h"
#include "MainUI.h"
USING(Client)


CSkill::CSkill(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
	for (_uint i = 0; i < SKILL_END; i++)
	{
		m_pTransformWnd[i] = nullptr;
		m_pTextureWnd[i] = nullptr;
	}
}

CSkill::CSkill(const CSkill & other)
	: CUIObject(other)
{
}

HRESULT CSkill::Setup_GameObject_Prototype()
{

	return S_OK;
}

HRESULT CSkill::Setup_GameObject(void * _pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformWnd[SKILL_ACTIVE_WND]->Set_Position(m_vCenterPos);
	m_pTransformWnd[SKILL_PASSIVE_WND]->Set_Position(
		_vec3(m_vCenterPos.x, m_vCenterPos.y - 100.f, 0.f));

	_vec3 vActiveCenterPos = { m_vCenterPos.x - 65.f, m_vCenterPos.y - 220.f, 0.f };
	_vec3 vPassiveCenterPos = { m_vCenterPos.x + 60.f, m_vCenterPos.y - 220.f, 0.f };

	m_tSkillSortCollRt[ACTIVE].left = (LONG)(vActiveCenterPos.x - 60.f);
	m_tSkillSortCollRt[ACTIVE].right = (LONG)(vActiveCenterPos.x + 60.f);
	m_tSkillSortCollRt[ACTIVE].top = (LONG)(vActiveCenterPos.y - 30.f);
	m_tSkillSortCollRt[ACTIVE].bottom = (LONG)(vActiveCenterPos.y + 30.f);

	m_tSkillSortCollRt[PASSIVE].left = (LONG)(vPassiveCenterPos.x - 60.f);
	m_tSkillSortCollRt[PASSIVE].right = (LONG)(vPassiveCenterPos.x + 60.f);
	m_tSkillSortCollRt[PASSIVE].top = (LONG)(vPassiveCenterPos.y - 30.f);
	m_tSkillSortCollRt[PASSIVE].bottom = (LONG)(vPassiveCenterPos.y + 30.f);

	for (_uint i = 0; i < 6; i++)
	{
		_vec3 vPos = { 0.f, 0.f, 0.f };
		_vec3 vSize = { 8.f, 8.f, 0.f };
		vPos.x = m_vCenterPos.x - 95.f;
		vPos.y = (i * 73.f) + (m_vCenterPos.y - 140.f);

		m_tActiveCollRt[i].left = (LONG)(vPos.x - vSize.x);
		m_tActiveCollRt[i].right = (LONG)(vPos.x + vSize.x);
		m_tActiveCollRt[i].top = (LONG)(vPos.y - vSize.y);
		m_tActiveCollRt[i].bottom = (LONG)(vPos.y + vSize.y);
	}

	return S_OK;
}

_int CSkill::Update_GameObject(_float _fDeltaTime)
{
	if (CKeyManager::Get_Instance()->Key_Down('Y'))
		m_bRender = !m_bRender;

	if (m_bRender)
	{
		if (CKeyManager::Get_Instance()->Key_Pressing(VK_LBUTTON))
		{
			POINT ptMouse = {};
			TCHAR szBuff[MAX_PATH] = L"";
			GetCursorPos(&ptMouse);
			ScreenToClient(g_hWnd, &ptMouse);

			wsprintf(szBuff, L"X : %d, Y : %d", ptMouse.x, ptMouse.y);
			//PRINT_LOG(szBuff, LOG::CLIENT);
		}

		if (FAILED(Set_SkillSort()))
			return GAMEOBJECT::WARN;
		if (FAILED(Show_ActiveSkill_Info()))
			return GAMEOBJECT::WARN;
		if (FAILED(Move_To_QuickSlot()))
			return GAMEOBJECT::WARN;
			
		for (_uint i = 0; i < SKILL_END; ++i)
			m_pTransformWnd[i]->Update_Transform();
	}

	return GAMEOBJECT::NOEVENT;
}

_int CSkill::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_UI, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CSkill::Render_UI()
{
	if (m_bRender)
	{
		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return E_FAIL;

		// 스킬 윈도우 그리기
		for (_uint i = (_uint)m_eSkillSort; i < SKILL_END;)
		{
			const D3DXIMAGE_INFO* pTexInfo = m_pTextureWnd[i]->Get_TexInfo(0);
			_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };
			_vec3 vPos = m_pTransformWnd[i]->Get_Desc().vPosition;

			m_tWndCollRt[i].left = (LONG)(vPos.x - vCenter.x);
			m_tWndCollRt[i].right = (LONG)(vPos.x + vCenter.x);
			m_tWndCollRt[i].top = (LONG)(vPos.y - vCenter.y);
			m_tWndCollRt[i].bottom = (LONG)(vPos.y + vCenter.y);

			m_pSprite->SetTransform(&m_pTransformWnd[i]->Get_Desc().matWorld);
			m_pSprite->Draw(
				(LPDIRECT3DTEXTURE9)m_pTextureWnd[i]->GetTexture(0),
				nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
			if (i == ACTIVE)
				i += 2;
			else
				++i;
		}
	}

	return S_OK;
}

HRESULT CSkill::Set_SkillSort()
{
	POINT ptMouse = {};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (CKeyManager::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		_int k = 0;
		if (PtInRect(&m_tSkillSortCollRt[ACTIVE], ptMouse))
			m_eSkillSort = ACTIVE;
		else if (PtInRect(&m_tSkillSortCollRt[PASSIVE], ptMouse))
			m_eSkillSort = PASSIVE;
	}

	return S_OK;
}

HRESULT CSkill::Show_ActiveSkill_Info()
{
	for (_uint i = 0; i < 6; i++)
	{
		POINT ptMouse = {};
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		if (PtInRect(&m_tActiveCollRt[i], ptMouse))
		{
			//PRINT_LOG(L"스킬 아이콘 선택함", LOG::CLIENT);
		}
	}

	return S_OK;
}

HRESULT CSkill::Move_To_QuickSlot()
{
	_int iIndex = 0;
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return E_FAIL;
	CMainUI* pMainUI = (CMainUI*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_MainUI", 0);

	for (_uint i = 0; i < 6; i++)
	{
		if (CKeyManager::Get_Instance()->Key_Pressing(VK_LBUTTON))
		{
			POINT ptMouse = {};
			GetCursorPos(&ptMouse);
			ScreenToClient(g_hWnd, &ptMouse);
			if (PtInRect(&m_tActiveCollRt[i], ptMouse))
			{
				_int k = 0;
				if (FAILED(pMainUI->Get_QuickSlotSkill(i)))
					return E_FAIL;
				return S_OK;

			}
		}
		
	}

	return S_OK;
}

HRESULT CSkill::Add_Component()
{
	for (_uint i = 0; i < SKILL_END; ++i)
	{
		// 2. Transform
		TCHAR szTransform[MAX_PATH] = L"";
		StringCchPrintf(szTransform, sizeof(TCHAR) * MAX_PATH,
			L"Com_Transform%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_Transform"
			, szTransform, (CComponent**)&m_pTransformWnd[i])))
			return E_FAIL;

		// 3. Texture
		TCHAR szTexture[MAX_PATH] = L"";
		TCHAR szTextureName[MAX_PATH] = L"";
		wstring strTexture = L"";
		wstring strTextureName = L"";

		StringCchPrintf(szTexture, sizeof(TCHAR) * MAX_PATH,
			L"Com_Texture%d", i);
		strTexture = szTexture;
		if (i == SKILL_ACTIVE_WND)
			StringCchPrintf(szTextureName, sizeof(TCHAR) * MAX_PATH,
				L"Component_Texture_Skill_ActiveWnd");
		if (i == SKILL_PASSIVE_WND)
			StringCchPrintf(szTextureName, sizeof(TCHAR) * MAX_PATH,
				L"Component_Texture_Skill_PassiveWnd");
		strTextureName = szTextureName;

		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, strTextureName
			, strTexture, (CComponent**)&m_pTextureWnd[i])))
			return E_FAIL;
	}

	return S_OK;
}

CGameObject * CSkill::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == _pDevice)
		return nullptr;

	CSkill* pInstance = new CSkill(_pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSkill::Clone_GameObject(void * _pArg)
{
	CSkill* pInstance = new CSkill(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CSkill", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill::Free()
{
	for (_uint i = 0; i < SKILL_END; i++)
	{
		Safe_Release(m_pTransformWnd[i]);
		Safe_Release(m_pTextureWnd[i]);
	}

	CUIObject::Free();
}
