#include "stdafx.h"
#include "Item.h"
#include "UICamera.h"
#include "..\Headers\Shop.h"

USING(Client)

CShop::CShop(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
}

CShop::CShop(const CShop & other)
	: CGameObject(other)
{
}

void CShop::Set_ShopItemTexturePos(_vec3 * pPos)
{
	memcpy(pPos, m_vItemTexturePos, sizeof(m_vItemTexturePos));
}

void CShop::Set_ShopItemNamePos(_vec3 * pPos)
{
	memcpy(pPos, m_vItemNamePos, sizeof(m_vItemNamePos));
}

void CShop::Set_ShopItemPricePos(_vec3 * pPos)
{
	memcpy(pPos, m_vItemPricePos, sizeof(m_vItemPricePos));
}

HRESULT CShop::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CShop::Setup_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	for (_uint i = 0; i < SHOP_END; ++i)
		m_vScale[i] = { 80.f, 90.f, 1.f };

	m_vPos[SHOP_WND] = D3DXVECTOR3(400.f, 300.f, 0.f);
	m_vScale[SHOP_WND] = D3DXVECTOR3(600.f, 550.f, 1.f);

	_uint iIndex = 0;

	for (_uint j = 0; j < 3; ++j)
	{
		for (_uint i = 0; i < 4; ++i)
		{
			m_vItemTexturePos[j][i].x = (i * 115.f) + 270.f;
			m_vItemTexturePos[j][i].y = (j * 172.f) + 105.f;
			m_vItemTexturePos[j][i].z = 0.f;

			m_vItemNamePos[j][i].x = (i * 115.f) + 270.f;
			m_vItemNamePos[j][i].y = (j * 167.f) + 150.f;
			m_vItemNamePos[j][i].z = 0.f;

			m_vItemPricePos[j][i].x = (i * 115.f) + 280.f;
			m_vItemPricePos[j][i].y = (j * 167.f) + 180.f;
			m_vItemPricePos[j][i].z = 0.f;
		}
	}

	for (_uint j = 0; j < 3; ++j)
	{
		for (_uint i = 0; i < 4; ++i)
		{
			//m_vBuyPos[i][j].x
		}
	}

	return S_OK;
}

int CShop::Update_GameObject(float DeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CItem* pItem = (CItem*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Item");
	if (nullptr == pItem)
		return E_FAIL;

	pItem->Set_RenderShopItem(m_bRender);

	return GAMEOBJECT::NOEVENT;
}

int CShop::LateUpdate_GameObject(float DeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_UI, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CShop::Render_UI()
{
	if (m_bRender)
	{
		// Z ¹öÆÛ ²ô±â
		m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return E_FAIL;

		CUICamera* pCamera = (CUICamera*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_UICamera");
		if (nullptr == pCamera)
			return E_FAIL;

		if (FAILED(dynamic_cast<CVIBuffer_RectTexture*>(m_pVIBufferCom)->Set_UITransform(
			&m_pTransformCom->Get_Desc().matWorld, pCamera
			, m_vPos[SHOP_WND], m_vScale[SHOP_WND])))
			return E_FAIL;

		if (FAILED(m_pTextureCom->SetTexture(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
			return E_FAIL;

		m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	}

	return S_OK;
}

HRESULT CShop::Add_Component()
{
	// 1. VIBuffer
	if (FAILED(CGameObject::Add_Component(
		SCENE_STATIC, L"Component_VIBuffer_RectTexture"
		, L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// 2. Transform
	if (FAILED(CGameObject::Add_Component(
		SCENE_STATIC, L"Component_Transform"
		, L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// 3. Texture
	if (FAILED(CGameObject::Add_Component(
		SCENE_STATIC, L"Component_Texture_Shop_ShopWnd",
		L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

CShop * CShop::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CShop* pInstance = new CShop(pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CShop", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CShop::Clone_GameObject(void * pArg)
{
	CShop* pInstance = new CShop(*this);
	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CShop", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShop::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);

	CGameObject::Free();
}
