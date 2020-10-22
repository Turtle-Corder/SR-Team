#include "stdafx.h"
#include "Item.h"
#include "ItemManager.h"
#include "UICamera.h"
#include "KeyManager.h"
#include "..\Headers\Inventory.h"

USING(Client)

CInventory::CInventory(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
	for (_uint i = 0; i < INVEN_END; ++i)
	{
		m_pTransformCom[i] = nullptr;
		m_pTextureCom[i] = nullptr;
	}
}

CInventory::CInventory(const CInventory & _rOther)
	: CUIObject(_rOther)
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

HRESULT CInventory::Get_ShopItem(const wstring & strItemName)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CItem* pItem = (CItem*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Item");
	if (nullptr == pItem)
		return E_FAIL;

	// �κ��� ������ �߰�
	auto& iter_begin = m_pInvenList.begin();

	for (auto& pItem : m_pInvenList)
	{
		// ���� �̹� �����ϴ� �������̶�� ������ ������Ų��
		if (pItem->szItemTag == strItemName)
		{
			// ���߿� ��� �߰�
			++pItem->iCnt;
		}
	}

	// ������ Ŭ���� ���� �� �߰�
	// ������ Ŭ�������� strItemName(�������� �̸�)���� ������ �������� �ؽ�ó�� �����ͼ� ����
	m_pTextureItem[m_iInsertOrder] = pItem->Get_ItemInfo_Texture(strItemName);
	if (m_pTextureItem[m_iInsertOrder] == nullptr)
	{
		PRINT_LOG(L"����", LOG::CLIENT);
	}
	else
	{
		// �� ĭ���� �������� �ִٰ� ����
		m_bIsItemHere[m_iInsertOrder] = true;

		PRINT_LOG(L"���� ������ ���� �Ϸ�", LOG::CLIENT);
		++m_iInsertOrder;
	}

	return S_OK;
}

HRESULT CInventory::Setup_GameObject_Prototype()
{
	
	return S_OK;
}

HRESULT CInventory::Setup_GameObject(void * pArg)
{
	m_pTextureItem.resize(36);
	m_pTransformItem.resize(36);

	if (FAILED(Add_Component()))
		return E_FAIL;
	if (FAILED(Add_Component_Item()))
		return E_FAIL;

	m_pTransformCom[INVEN_WND]->Set_Position(_vec3(500.f, 200.f, 0.f));
	m_pTransformCom[INVEN_SELL_BUTTON]->Set_Position(_vec3(610.f, 325.f, 0.f));

	_int iIndex = 0;
	for (_uint i = 0; i < 6; ++i)
	{
		for (_uint j = 0; j < 6; ++j)
		{
			iIndex = i * 6 + j;
			m_vItemPos[i][j].x = (j * 45.f) + 380.f;
			m_vItemPos[i][j].y = (i * 30.f) + 117.f;
			m_vItemPos[i][j].z = 0.f;
			m_pTransformItem[iIndex]->Set_Position(m_vItemPos[i][j]);
		}
	}

	return S_OK;
}

_int CInventory::Update_GameObject(float DeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;


	// �κ� on/off
	if ((CKeyManager::Get_Instance()->Key_Pressing('I')) && !m_bRender)
		m_bRender = true;
	else if ((CKeyManager::Get_Instance()->Key_Pressing('I')) && m_bRender)
		m_bRender = false;

	// �Ǹ� ��ư�� �������� Ȯ��(m_bSelect_SellItem)
	if (FAILED(Check_SellButton()))
		return GAMEOBJECT::WARN;
	// �Ǹ� ��ư�� ������ ���
	if (m_bSelect_SellItem)
		// �Ǹ��� ������ ����
		if (FAILED(Select_SellItem()))
			return GAMEOBJECT::WARN;

	// �ڵ� ���� ��ư�� �������� Ȯ��
	if (FAILED(Check_AutoSortButton()))
		return GAMEOBJECT::WARN;

	for (_uint i = 0; i < INVEN_END; ++i)
		m_pTransformCom[i]->Update_Transform();

	_int iSize = m_pTransformItem.size();
	for (_int i = 0; i < iSize; ++i)
	{
		m_pTransformItem[i]->Update_Transform();
	}

	return GAMEOBJECT::NOEVENT;
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
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (m_bRender)
	{
		for (_uint i = 0; i < INVEN_END; ++i)
		{
			const D3DXIMAGE_INFO* pTexInfo = m_pTextureCom[i]->Get_TexInfo(0);
			_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };
			_vec3 vPos = m_pTransformCom[i]->Get_Desc().vPosition;

			m_tInvenWndCollRt[i].left = (LONG)(vPos.x - vCenter.x);
			m_tInvenWndCollRt[i].right = (LONG)(vPos.x + vCenter.x);
			m_tInvenWndCollRt[i].top = (LONG)(vPos.y - vCenter.y);
			m_tInvenWndCollRt[i].bottom = (LONG)(vPos.y + vCenter.y);

			m_pSprite->SetTransform(&m_pTransformCom[i]->Get_Desc().matWorld);
			m_pSprite->Draw(
				(LPDIRECT3DTEXTURE9)m_pTextureCom[i]->GetTexture(0),
				nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		if (FAILED(Render_Item()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CInventory::Check_SellButton()
{
	if (CKeyManager::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		POINT ptMouse = {};
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		if (PtInRect(&m_tSellButtonRt, ptMouse))
		{
			if (!m_bSelect_SellItem)
			{
				m_bSelect_SellItem = true;
				PRINT_LOG(L"�Ǹ� ���� ����", LOG::CLIENT);
			}
			else if (m_bSelect_SellItem)
			{
				// �Ǹ��� ������ ���� ��
				m_bSelect_SellItem = false;
				// ���� ���õ� �����۵��� �� �������� �׷��ش�
				m_bRenderEmptySell = true;
				PRINT_LOG(L"������ �Ǹ� ����.", LOG::CLIENT);
				//if (FAILED(Sell_Item()))
				//	return E_FAIL;
			}
		}
	}
	return S_OK;
}

HRESULT CInventory::Select_SellItem()
{
	int iIndex = 0;

	for (_uint i = 0; i < 6; i++)
	{
		for (_uint j = 0; j < 6; j++)
		{
			if (CKeyManager::Get_Instance()->Key_Pressing(VK_LBUTTON))
			{
				iIndex = i * 6 + j;
				POINT ptMouse = {};
				GetCursorPos(&ptMouse);
				ScreenToClient(g_hWnd, &ptMouse);
				if (PtInRect(&m_tItemCollRt[i][j], ptMouse))
				{
					// �������� �ִ� ĭ�鸸 ���� �� �� ����
					if (m_bIsItemHere[iIndex])
						m_bSelectedSell[iIndex] = true;

				}
			}
		}
	}
	return S_OK;
}

HRESULT CInventory::Sell_Item()
{
	auto& iter = m_pInvenList.begin();
	_uint iSize = m_pInvenList.size();
	int iIndex = 0;

	while (iter != m_pInvenList.end())
	{
		if (m_bSelectedSell[iIndex])
		{
			// �Ǹ� ������ ���� ����
			m_bSelectedSell[iIndex] = false;
			// ����Ʈ���� ����
			m_pInvenList.erase(iter++);
		}
		else
			iter++;
		++iIndex;
	}

	return S_OK;
}

HRESULT CInventory::Check_AutoSortButton()
{
	POINT ptMouse = {};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	int iItemListSize = m_pInvenList.size();

	// ������ Ű�Է����� �ϴµ� ���߿� ��ư �߰��ؾ� ��
	if (CKeyManager::Get_Instance()->Key_Pressing('M'))
	{
		auto& iter = m_pInvenList.begin();
		_uint iSize = m_pInvenList.size();
		int iItemIndex = 0;
		m_bAutoSort = true;

		while (iter != m_pInvenList.end())
		{
			if (m_bSelectedSell[iItemIndex])
				m_pInvenList.erase(iter++);
			else
				iter++;

			++iItemIndex;
		}

		int iTextureItemIdx = 0;
		int iDeleteCnt = 0;
		for (auto& iter = m_pTextureItem.begin(); iter != m_pTextureItem.end(); )
		{
			if (m_bSelectedSell[iTextureItemIdx])
			{
				++iDeleteCnt;
				iter = m_pTextureItem.erase(iter);
				m_bSelectedSell[iTextureItemIdx] = false;
			}
			else
				++iter;
			++iTextureItemIdx;
		}

		// �����Ѹ�ŭ ���� ���� �־��ش�
		for (_int i = iItemListSize - iDeleteCnt; i < iDeleteCnt; i++)
		{
			// Texture-------------------------------------------------------------------
			TCHAR szItemTexture[MAX_STR] = L"";
			TCHAR szItemTextureName[MAX_STR] = L"";
			//ITEM* pItem = new ITEM;
			//pItem->wstrItemName = L"Empty";
			wsprintf(szItemTextureName, L"Component_Texture_Item_Empty");
			wsprintf(szItemTexture, L"Com_ItemTexture%d", i);

			if (FAILED(CGameObject::Add_Component(SCENE_STATIC, 
				szItemTextureName, szItemTexture, (CComponent**)&m_pTextureItem[i])))
				return E_FAIL;
		}

		// ���� ��
		//m_bAutoSort = false;
		m_bRenderEmptySell = false;
	}

	return S_OK;
}

HRESULT CInventory::Render_Item()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	size_t iSize = m_pTextureItem.size();
	for (_uint i = 0; i < 6; ++i)
	{
		for (_uint j = 0; j < 6; ++j)
		{
			int iIndex = i * 6 + j;
			// ������ �� ������ ���͸�ŭ ���� ��������� ��
			// -> �ٵ� ���� �������ؼ� �׳� ���Ƴ���
			if (iIndex >= iSize)
				return S_OK;

			const D3DXIMAGE_INFO* pTexInfo = m_pTextureItem[iIndex]->Get_TexInfo(0);
			_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };

			_vec3 vPos = m_pTransformItem[iIndex]->Get_Desc().vPosition;

			m_tItemCollRt[i][j].left = (LONG)(vPos.x - vCenter.x);
			m_tItemCollRt[i][j].left = (LONG)(vPos.x + vCenter.x);
			m_tItemCollRt[i][j].left = (LONG)(vPos.y - vCenter.y);
			m_tItemCollRt[i][j].left = (LONG)(vPos.y + vCenter.y);

			m_pSprite->SetTransform(&m_pTransformItem[iIndex]->Get_Desc().matWorld);
			m_pSprite->Draw(
				(LPDIRECT3DTEXTURE9)m_pTextureItem[iIndex]->GetTexture(0),
				nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			// �Ǹ� ������ �������� ��
			if (!m_bRenderEmptySell)
			{
				//// �Ǹ� ���������� ���õ��� �ʾ����� �����۸� �׸���
				//if (!m_bSelectedSell[iIndex])
				//{
				//	const D3DXIMAGE_INFO* pTexInfo = m_pTextureItem[iIndex]->Get_TexInfo(0);
				//	_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };

				//	m_pSprite->SetTransform(&m_pTransformItem[iIndex]->Get_Desc().matWorld);
				//	m_pSprite->Draw(
				//		(LPDIRECT3DTEXTURE9)m_pTextureItem[iIndex]->GetTexture(0),
				//		nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
				//}

				// �Ǹ� ���������� ���õǾ��ٸ�
				if (m_bSelectedSell[iIndex])
				{
					const D3DXIMAGE_INFO* pTexInfo = m_pTextureSell->Get_TexInfo(0);
					_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };

					m_pSprite->SetTransform(&m_pTransformItem[iIndex]->Get_Desc().matWorld);
					m_pSprite->Draw(
						(LPDIRECT3DTEXTURE9)m_pTextureSell->GetTexture(0),
						nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					//// �� �ȿ� �������� �׸���
					//if (FAILED(m_pTextureItem[iIndex]->SetTexture(0)))
					//	return E_FAIL;
					//if (FAILED(m_pVIBufferItem[iIndex]->Render_VIBuffer()))
					//	return E_FAIL;
				}
			}

			// �Ǹ� �����۵��� ������ ��, �Ǹ� ��ư�� �����ٸ�
			else if (m_bRenderEmptySell)
			{
				// �Ǹ� ���������� ���õ� �����۵��� �� �ؽ�ó�� �׸���
				if (m_bSelectedSell[iIndex])
				{
					const D3DXIMAGE_INFO* pTexInfo = m_pTextureEmpty->Get_TexInfo(0);
					_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };

					m_pSprite->SetTransform(&m_pTransformItem[iIndex]->Get_Desc().matWorld);
					m_pSprite->Draw(
						(LPDIRECT3DTEXTURE9)m_pTextureEmpty->GetTexture(0),
						nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
				}
				//// �Ǹ� ���������� ���õ��� ���� �����۵��� �״�� �׸���
				//else
				//{
				//	if (FAILED(m_pTextureItem[iIndex]->SetTexture(0)))
				//		return E_FAIL;
				//	if (FAILED(m_pVIBufferItem[iIndex]->Render_VIBuffer()))
				//		return E_FAIL;

				//}
			}
		}
	}

	return S_OK;
}

HRESULT CInventory::Add_Component()
{
	for (_uint i = 0; i < INVEN_END; ++i)
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
		if (i == INVEN_WND)
			wsprintf(szTextureName, L"Component_Texture_Inven_InvenWnd");
		if (i == INVEN_SELL_BUTTON)
			wsprintf(szTextureName, L"Component_Texture_Inven_SellButton");
		wsprintf(szTexture, L"Com_Texturem%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, szTextureName,
			szTexture, (CComponent**)&m_pTextureCom[i])))
			return E_FAIL;
	}

	//// 4. ItemMgr
	//if (FAILED(CGameObject::Add_Component(SCENE_STATIC, 
	//	L"Component_ItemManagement", L"Com_ItemMgr", 
	//	(CComponent**)&m_pItemMgrCom)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CInventory::Add_Component_Item()
{
	for (_uint i = 0; i < 36; ++i)
	{
		// Texture-------------------------------------------------------------------
		TCHAR szItemTexture[MAX_STR] = L"";
		TCHAR szItemTextureName[MAX_STR] = L"";
		//ITEM* pItem = new ITEM;
		//pItem->wstrItemName = L"Empty";
		wsprintf(szItemTextureName, L"Component_Texture_Item_Empty");
		wsprintf(szItemTexture, L"Com_ItemTexture%d", i);

		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, szItemTextureName,
			szItemTexture, (CComponent**)&m_pTextureItem[i])))
			return E_FAIL;

		// Transform-------------------------------------------------------------------
		TCHAR szTransform[MAX_STR] = L"";
		wsprintf(szTransform, L"Com_ItemTransform%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_Transform"
			, szTransform, (CComponent**)&m_pTransformItem[i])))
			return E_FAIL;
	}

	if (FAILED(CGameObject::Add_Component(
		SCENE_STATIC, L"Component_Texture_Select_SellItem",
		L"Com_SellItemTexture", (CComponent**)&m_pTextureSell)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(
		SCENE_STATIC, L"Component_Texture_Item_Empty",
		L"Com_EmptyItemTexture", (CComponent**)&m_pTextureEmpty)))
		return E_FAIL;
}

CInventory * CInventory::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == pDevice)
		return nullptr;

	CInventory* pInstance = new CInventory(pDevice, _pSprite, _pFont);
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
	//Safe_Release(m_pItemMgrCom);
	Safe_Release(m_pTextureSell);
	Safe_Release(m_pTextureEmpty);

	for (_uint i = 0; i < INVEN_END; i++)
	{
		Safe_Release(m_pTransformCom[i]);
		Safe_Release(m_pTextureCom[i]);
	}

	for (auto& pItem : m_pTextureItem)
	{
		Safe_Release(pItem);
	}
	for (auto& pItem : m_pTransformItem)
	{
		Safe_Release(pItem);
	}

	m_pTextureItem.clear();
	m_pTransformItem.clear();

	m_pInvenList.clear();

	CUIObject::Free();
}
