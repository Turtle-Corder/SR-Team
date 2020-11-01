#include "stdafx.h"
#include "..\Headers\Quest1.h"
#include "Mouse.h"


USING(Client)


CQuest1::CQuest1(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
	for (_uint i = 0; i < QUEST1_END; i++)
	{
		m_pTextureCom[i] = nullptr;
	}
}

CQuest1::CQuest1(const CQuest1 & other)
	: CUIObject(other)
{
}

HRESULT CQuest1::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CQuest1::Setup_GameObject(void * _pArg)
{
	if (FAILED(Add_Component()))
		return S_OK;

	m_tGreetingCollRt.left = 300;
	m_tGreetingCollRt.top = 800;
	m_tGreetingCollRt.right = 932;
	m_tGreetingCollRt.bottom = 930;

	return S_OK;
}

_int CQuest1::Update_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	CMouse* pMouse = (CMouse*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Mouse");
	if (pMouse == nullptr)
		return E_FAIL;

	if (pManagement->Key_Down('G'))
	{
		if (m_eSituation == QUEST1_END)
			m_eSituation = GREETINGS;
	}

	if (pManagement->Key_Down(VK_LBUTTON))
	{
		if (m_eSituation == GREETINGS &&
			PtInRect(&m_tGreetingCollRt, pMouse->Get_Point()))
		{
			m_eSituation = REQUEST;
		}
	}

	return GAMEOBJECT::NOEVENT;
}

_int CQuest1::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_UI, this)))
		return GAMEOBJECT::WARN;


	return GAMEOBJECT::NOEVENT;
}

HRESULT CQuest1::Render_UI()
{
	if (m_eSituation != QUEST1_END)
	{
		D3DXMATRIX matTrans, matWorld;
		const D3DXIMAGE_INFO* pTexInfo = m_pTextureCom[m_eSituation]->Get_TexInfo(0);
		if (nullptr == pTexInfo)
			return E_FAIL;

		_vec3 vCenter = { (_float)(pTexInfo->Width >> 1), (_float)(pTexInfo->Height >> 1), 0.f };

		D3DXMatrixTranslation(&matTrans, 950.f, 800.f, 0.f);
		matWorld = matTrans;

		m_pSprite->SetTransform(&matWorld);
		m_pSprite->Draw(
			(LPDIRECT3DTEXTURE9)m_pTextureCom[m_eSituation]->GetTexture(0),
			nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	return S_OK;
}

HRESULT CQuest1::Add_Component()
{
	TCHAR szTextureNameTag[][MAX_STR] = 
	{
		L"Component_Texture_Quest1_One",
		L"Component_Texture_Quest1_Two",
		L"Component_Texture_Quest1_ThreeNo",
		L"Component_Texture_Quest1_ThreeOk",
		L"Component_Texture_Quest1_FourClear",
		L"Component_Texture_Quest1_FourNoClear",
	};
	TCHAR szTexture[MIN_STR] = L"Com_Texture%d";
	TCHAR szCombine[MIN_STR] = L"";

	for (_uint i = 0; i < QUEST1_END; i++)
	{
		StringCchPrintf(szCombine, _countof(szCombine), szTexture, i);
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, szTextureNameTag[i], 
			szCombine, (CComponent**)&m_pTextureCom[i])))
			return E_FAIL;
	}

	return S_OK;
}

CQuest1 * CQuest1::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == _pDevice)
		return nullptr;

	CQuest1* pInstance = new CQuest1(_pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CQuest1", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CQuest1::Clone_GameObject(void * _pArg)
{
	CQuest1* pInstance = new CQuest1(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CQuest1", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQuest1::Free()
{
	for (_uint i = 0; i < QUEST1_END; i++)
	{
		Safe_Release(m_pTextureCom[i]);
	}

	CUIObject::Free();
}
