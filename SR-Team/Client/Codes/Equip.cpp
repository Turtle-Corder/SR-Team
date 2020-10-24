#include "stdafx.h"
#include "..\Headers\Equip.h"

#include "KeyManager.h"
USING(Client)


CEquip::CEquip(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
	for (_uint i = 0; i < EQUIP_END; ++i)
	{
		m_pTransformCom[i] = nullptr;
		m_pTextureCom[i] = nullptr;
	}
}

CEquip::CEquip(const CEquip & other)
	: CUIObject(other)
{
}

HRESULT CEquip::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CEquip::Setup_GameObject(void * _pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom[EQUIP_START]->Set_Position(_vec3(400.f, 300.f, 0.f));

	return S_OK;
}

_int CEquip::Update_GameObject(_float _fDeltaTime)
{
	if (CKeyManager::Get_Instance()->Key_Down('U'))
		m_bRender != m_bRender;


	return GAMEOBJECT::NOEVENT;
}

_int CEquip::LateUpdate_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

HRESULT CEquip::Render_UI()
{
	_int k = 0;
	if (m_bRender)
	{
		//m_DC = GetDC(g_hWnd);

		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return E_FAIL;

		// 상점 윈도우 그리기
		for (_uint i = 0; i < EQUIP_END; ++i)
		{
			const D3DXIMAGE_INFO* pTexInfo = m_pTextureCom[i]->Get_TexInfo(0);
			_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };
			_vec3 vPos = m_pTransformCom[i]->Get_Desc().vPosition;

			m_tEquipWndCollRt[i].left = (LONG)(vPos.x - vCenter.x);
			m_tEquipWndCollRt[i].right = (LONG)(vPos.x + vCenter.x);
			m_tEquipWndCollRt[i].top = (LONG)(vPos.y - vCenter.y);
			m_tEquipWndCollRt[i].bottom = (LONG)(vPos.y + vCenter.y);

			m_pSprite->SetTransform(&m_pTransformCom[i]->Get_Desc().matWorld);
			m_pSprite->Draw(
				(LPDIRECT3DTEXTURE9)m_pTextureCom[i]->GetTexture(0),
				nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

	}

	return S_OK;
}

HRESULT CEquip::Add_Component()
{
	for (_uint i = 0; i < EQUIP_END; ++i)
	{
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
		if (i == EQUIP_START)
			wsprintf(szTextureName, L"Component_Texture_Equip_Stat");
		wsprintf(szTexture, L"Com_Texture%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, szTextureName,
			szTexture, (CComponent**)&m_pTextureCom[i])))
			return E_FAIL;
	}

	return S_OK;
}

CGameObject * CEquip::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == _pDevice)
		return nullptr;

	CEquip* pInstance = new CEquip(_pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CEquip", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEquip::Clone_GameObject(void * _pArg)
{
	CEquip* pInstance = new CEquip(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CEquip", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEquip::Free()
{
	for (_uint i = 0; i < EQUIP_END; ++i)
	{
		Safe_Release(m_pTransformCom[i]);
		Safe_Release(m_pTextureCom[i]);
	}
}
