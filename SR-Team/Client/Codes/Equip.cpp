#include "stdafx.h"
#include "..\Headers\Equip.h"

#include "KeyManager.h"
#include "Item.h"
USING(Client)


CEquip::CEquip(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
	for (_uint i = 0; i < EQUIP_END; ++i)
	{
		m_pTransformCom[i] = nullptr;
		m_pTextureCom[i] = nullptr;
	}
	for (_uint i = 0; i < ITEMSORT_END; i++)
	{
		m_pStatItem[i] = nullptr;
		m_pTextureItem[i] = nullptr;
		m_pTransformItem[i] = nullptr;
	}
}

CEquip::CEquip(const CEquip & other)
	: CUIObject(other)
{
}

void CEquip::Equip_Item(INVEN_ITEM & _tItem)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return;
	CItem* pItem = (CItem*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Item");
	if (pItem == nullptr)
		return;

	m_pStatItem[_tItem.eSort] = pItem->Get_ItemStat(_tItem.szItemTag);
	m_bEquip[_tItem.eSort] = true;
}



HRESULT CEquip::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CEquip::Setup_GameObject(void * _pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom[EQUIP_WND]->Set_Position(_vec3(350.f, 300.f, 0.f));
	m_pTransformCom[EQUIP_EQUIPMENT]->Set_Position(_vec3(150.f, 300.f, 0.f));

	return S_OK;
}

_int CEquip::Update_GameObject(_float _fDeltaTime)
{
	if (CKeyManager::Get_Instance()->Key_Down('U'))
		m_bRender = !m_bRender;

	for (_uint i = 0; i < EQUIP_END; ++i)
	{
		m_pTransformCom[i]->Update_Transform();
	}

	for (_int i = 0; i < ITEMSORT_END; i++)
	{
		if (m_bEquip[i])
			m_pTransformItem[i]->Update_Transform();
	}

	return GAMEOBJECT::NOEVENT;
}

_int CEquip::LateUpdate_GameObject(_float _fDeltaTime)
{
	_int i = 0;

	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_UI, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CEquip::Render_UI()
{
	_int k = 0;
	if (m_bRender)
	{
		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return E_FAIL;

		for (_uint i = 0; i < EQUIP_END; ++i)
		{
			const D3DXIMAGE_INFO* pTexInfo = m_pTextureCom[i]->Get_TexInfo(0);
			_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };
			_vec3 vPos = m_pTransformCom[i]->Get_Desc().vPosition;
			D3DXMATRIX matWorld, matTrans, matScale;

			m_tEquipWndCollRt[i].left = (LONG)(vPos.x - vCenter.x);
			m_tEquipWndCollRt[i].right = (LONG)(vPos.x + vCenter.x);
			m_tEquipWndCollRt[i].top = (LONG)(vPos.y - vCenter.y);
			m_tEquipWndCollRt[i].bottom = (LONG)(vPos.y + vCenter.y);

			D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, 0.f);
			D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);
			matWorld = matScale * matTrans;

			m_pSprite->SetTransform(&matWorld);
			m_pSprite->Draw(
				(LPDIRECT3DTEXTURE9)m_pTextureCom[i]->GetTexture(0),
				nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		if (FAILED(Render_EquipItem()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CEquip::Render_EquipItem()
{
	for (_uint i = 0; i < ITEMSORT_END; ++i)
	{
		if (m_bEquip[i])
		{
			const D3DXIMAGE_INFO* pTexInfo = m_pTextureItem[i]->Get_TexInfo(0);
			_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };

			m_pSprite->SetTransform(&m_pTransformItem[i]->Get_Desc().matWorld);
			m_pSprite->Draw(
				(LPDIRECT3DTEXTURE9)m_pTextureItem[i]->GetTexture(0),
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
		else if (i == EQUIP_EQUIPMENT)
			wsprintf(szTextureName, L"Component_Texture_Equip_Equipment");
		wsprintf(szTexture, L"Com_Texture%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, szTextureName,
			szTexture, (CComponent**)&m_pTextureCom[i])))
			return E_FAIL;
	}

	for (_uint i = 0; i < ITEMSORT_END; i++)
	{
		// 2. Stat
		TCHAR szStat[MAX_STR] = L"";
		wsprintf(szStat, L"Com_ItemStat%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_Status"
			, szStat, (CComponent**)&m_pStatItem[i])))
			return E_FAIL;

		// 2. Texture
		TCHAR szStat1[MAX_STR] = L"";
		wsprintf(szStat1, L"Com_ItemTexture%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_Texture_Equip_BackGround"
			, szStat1, (CComponent**)&m_pTextureItem[i])))
			return E_FAIL;

		// 2. Transform
		TCHAR szStat2[MAX_STR] = L"";
		CTransform::TRANSFORM_DESC tTransform;
		wsprintf(szStat2, L"Com_ItemTransform%d", i);

		if (i == 5)
			tTransform.vPosition = _vec3(300.f, 200.f, 0.f);

		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_Transform"
			, szStat2, (CComponent**)&m_pTransformItem[i], &tTransform)))
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

	for (_uint i = 0; i < ITEMSORT_END; i++)
	{
		Safe_Release(m_pStatItem[i]);
		Safe_Release(m_pTextureItem[i]);
		Safe_Release(m_pTransformItem[i]);
	}

	CUIObject::Free();
}
