#include "stdafx.h"
#include "Item.h"
#include "ItemManager.h"
#include "UICamera.h"
#include "KeyManager.h"
#include "..\Headers\Inventory.h"

USING(Client)

CInventory::CInventory(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CInventory::CInventory(const CInventory & _rOther)
	: CGameObject(_rOther)
{
}

void CInventory::Set_Render(bool bRender)
{
	m_bRender = bRender;
}

void CInventory::Set_InvenItemList(list<INVEN_ITEM*>* pItemList)
{
	m_pInvenList = *pItemList;
}

HRESULT CInventory::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CInventory::Setup_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;
	
	for (_uint i = 0; i < INVEN_END; ++i)
		m_vScale[i] = { 80.f, 90.f, 1.f };

	m_vPos[INVEN_WND] = _vec3(500.f, 200.f, 0.f);
	m_vScale[INVEN_WND] = _vec3(300.f, 300.f, 1.f);

	return S_OK;
}

_int CInventory::Update_GameObject(float DeltaTime)
{
	if (CKeyManager::Get_Instance()->Key_Pressing('G'))
	{
		// 일단은 그냥 아이템 먹는거로.. -> 나중에 수정
		m_pInvenList = m_pItemMgrCom->Get_DropItem(L"금 검", m_iInsertOrder, 200);
		//PRINT_LOG(L"검1 획득!");
		++m_iInsertOrder;
	}
	if (CKeyManager::Get_Instance()->Key_Pressing('H'))
	{
		m_pInvenList = m_pItemMgrCom->Get_DropItem(L"철 검", m_iInsertOrder, 100);
		//PRINT_LOG(L"검2 획득!");
		++m_iInsertOrder;
	}

	m_pInvenList = m_pItemMgrCom->Get_InvenList();

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CItem* pItem = (CItem*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Item");
	if (nullptr == pItem)
		return E_FAIL;

	// 인벤에 있는 아이템 그리게 하기
	dynamic_cast<CItem*>(pItem)->Set_RenderInvenItem(m_bRender);

	return 0;
}

_int CInventory::LateUpdate_GameObject(float DeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_UI, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CInventory::Render_UI()
{
	if (m_bRender)
	{
		// Z 버퍼 끄기
		m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

		CManagement* pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return E_FAIL;

		CUICamera* pCamera = (CUICamera*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_UICamera");
		if (nullptr == pCamera)
			return E_FAIL;

		if (FAILED(((CVIBuffer_RectTexture*)m_pVIBufferCom)->Set_UITransform(&m_pTransformCom->Get_Desc().matWorld, pCamera, m_vPos[INVEN_WND], m_vScale[INVEN_WND])))
			return E_FAIL;

		if (FAILED(m_pTextureCom->SetTexture(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
			return E_FAIL;

		m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	}

	return S_OK;
}

HRESULT CInventory::Add_Component()
{
	// 1. VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_RectTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// 2. Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// 3. Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Inven_InvenWnd", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// 4. ItemMgr
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_ItemManagement", L"Com_ItemMgr", (CComponent**)&m_pItemMgrCom)))
		return E_FAIL;

	return S_OK;
}

CInventory * CInventory::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CInventory* pInstance = new CInventory(pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CInven", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CInventory::Clone_GameObject(void * pArg)
{
	CInventory* pInstance = new CInventory(*this);
	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CInven", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInventory::Free()
{
	Safe_Release(m_pItemMgrCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);

	m_pInvenList.clear();

	CGameObject::Free();
}
