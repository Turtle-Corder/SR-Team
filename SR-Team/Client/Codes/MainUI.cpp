#include "stdafx.h"
#include "UICamera.h"
#include "..\Headers\MainUI.h"

USING(Client)

CMainUI::CMainUI(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
	for (_uint i = 0; i < MAINUI_END; ++i)
	{
		m_pVIBufferCom[i] = nullptr;
		m_pTransformCom[i] = nullptr;
		m_pTextureCom[i] = nullptr;
	}
}

CMainUI::CMainUI(const CMainUI & _rOther)
	: CUIObject(_rOther)
{
}

HRESULT CMainUI::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CMainUI::Setup_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	for (_uint i = 0; i < MAINUI_END; ++i)
		m_vScale[i] = { 80.f, 90.f, 1.f };

	m_vPos[MAINUI_MAIN] = D3DXVECTOR3(WINCX * 0.5f, WINCY - 50.f, 0.f);
	m_pTransformCom[MAINUI_MAIN]->Set_Position(m_vPos[MAINUI_MAIN]);

	m_vPos[MAINUI_HP] = D3DXVECTOR3(365.f, 500.f, 0.f);
	m_pTransformCom[MAINUI_HP]->Set_Position(m_vPos[MAINUI_HP]);

	m_vPos[MAINUI_MP] = D3DXVECTOR3(445.f, 500.f, 0.f);
	m_pTransformCom[MAINUI_MP]->Set_Position(m_vPos[MAINUI_MP]);

	m_vPos[MAINUI_QUICKSLOT_LFFT] = D3DXVECTOR3(170.f, 500.f, 0.f);
	m_pTransformCom[MAINUI_QUICKSLOT_LFFT]->Set_Position(m_vPos[MAINUI_QUICKSLOT_LFFT]);

	m_vPos[MAINUI_QUICKSLOT_RIGHT] = D3DXVECTOR3(660.f, 500.f, 0.f);
	m_pTransformCom[MAINUI_QUICKSLOT_RIGHT]->Set_Position(m_vPos[MAINUI_QUICKSLOT_RIGHT]);

	return S_OK;
}

int CMainUI::Update_GameObject(float DeltaTime)
{
	if (GetAsyncKeyState('P') & 0x8000)
	{
		for (_uint i = 0; i < MAINUI_END; ++i)
		{
			m_vPos[i].y -= 10.f;
		}
	}

	if (GetAsyncKeyState('L') & 0x8000)
	{
		for (_uint i = 0; i < MAINUI_END; ++i)
		{
			m_vPos[i].y += 10.f;
		}
	}

	for (_uint i = 0; i < MAINUI_END; ++i)
		m_pTransformCom[i]->Update_Transform();

	return GAMEOBJECT::NOEVENT;
}

int CMainUI::LateUpdate_GameObject(float DeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_UI, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CMainUI::Render_UI()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	for (_uint i = 0; i < MAINUI_END; ++i)
	{
		auto pTexInfo = m_pTextureCom[i]->Get_TexInfo(0);
		_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };

		m_pSprite->SetTransform(&m_pTransformCom[i]->Get_Desc().matWorld);
		m_pSprite->Draw((LPDIRECT3DTEXTURE9)m_pTextureCom[i]->GetTexture(0), nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	return S_OK;
}

HRESULT CMainUI::Add_Component()
{
	for (_uint i = 0; i < MAINUI_END; ++i)
	{
		// 1. VIBuffer
		TCHAR szVIBuffer[MAX_STR] = L"";
		wsprintf(szVIBuffer, L"Com_VIBuffer%d", i);

		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_VIBuffer_RectTexture"
			, szVIBuffer, (CComponent**)&m_pVIBufferCom[i])))
			return E_FAIL;

		// 2. Transform
		TCHAR szTransform[MAX_STR] = L"";
		wsprintf(szTransform, L"Com_Transform%d", i);

		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_Transform"
			, szTransform, (CComponent**)&m_pTransformCom[i])))
			return E_FAIL;

		// 3. Texture
		TCHAR szTexture[MAX_STR] = L"";
		TCHAR szTextureName[MAX_STR] = L"";
		wstring strTexture = L"";
		wstring strTextureName = L"";

		wsprintf(szTexture, L"Com_Texture%d", i);
		strTexture = szTexture;
		if (i == MAINUI_MAIN)
			wsprintf(szTextureName, L"Component_Texture_MainUI_Main");
		else if (i == MAINUI_HP)
			wsprintf(szTextureName, L"Component_Texture_MainUI_Hp");
		else if (i == MAINUI_MP)
			wsprintf(szTextureName, L"Component_Texture_MainUI_Mp");
		else if (i == MAINUI_QUICKSLOT_LFFT)
			wsprintf(szTextureName, L"Component_Texture_MainUI_QuickSlot_Left");
		else if (i == MAINUI_QUICKSLOT_RIGHT)
			wsprintf(szTextureName, L"Component_Texture_MainUI_QuickSlot_Right");

		strTextureName = szTextureName;

		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, strTextureName
			, strTexture, (CComponent**)&m_pTextureCom[i])))
			return E_FAIL;
	}

	return S_OK;
}

CMainUI * CMainUI::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == pDevice)
		return nullptr;

	CMainUI* pInstance = new CMainUI(pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CMainUI", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMainUI::Clone_GameObject(void * pArg)
{
	CMainUI* pInstance = new CMainUI(*this);
	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CMainUI", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainUI::Free()
{
	for (_uint i = 0; i < MAINUI_END; ++i)
	{
		Safe_Release(m_pTextureCom[i]);
		Safe_Release(m_pTransformCom[i]);
		Safe_Release(m_pVIBufferCom[i]);
	}

	CUIObject::Free();
}
