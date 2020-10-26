#include "stdafx.h"
#include "Item.h"
#include "ItemManager.h"
#include "UICamera.h"
#include "KeyManager.h"
#include "Equip.h"
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

HRESULT CInventory::Get_ShopItem(const wstring & strItemName)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CItem* pItem = (CItem*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Item");
	if (nullptr == pItem)
		return E_FAIL;

	for (auto& pItem : m_pInvenList)
	{
		// 만약 이미 존재하는 아이템이고
		if (pItem->szItemTag == strItemName)
		{
			// 골드가 있다면 개수만 증가시킨다
			if (m_iGold >= pItem->iPrice)
			{
				++pItem->iCnt;
				m_iGold -= pItem->iPrice;
				PRINT_LOG(L"상점 아이템 구매 완료", LOG::CLIENT);
			}
			else
				PRINT_LOG(L"못사", LOG::CLIENT);
			return S_OK;
		}
	}

	_int iPrice = pItem->Get_ItemInfo_Price(strItemName);

	if ((iPrice == -1) || m_iGold < iPrice)
	{
		PRINT_LOG(L"못사", LOG::CLIENT);
	}
	else if (m_iGold >= iPrice)
	{
		m_iGold -= iPrice;

		// 아이템 클래스에서 strItemName(아이템의 이름)으로 구매한 아이템의 텍스처를 가져와서 저장
		if (m_pTextureItem[m_iInsertOrder])
			Safe_Release(m_pTextureItem[m_iInsertOrder]);
		m_pTextureItem[m_iInsertOrder] = pItem->Get_ItemInfo_Texture(strItemName);

		INVEN_ITEM* pInvenItem = new INVEN_ITEM;
		pItem->Get_ItemInfo(strItemName, *pInvenItem);
		pInvenItem->iInvneInsertOrder = m_iInsertOrder;

		m_pInvenList.emplace_back(pInvenItem);

		// 이 칸에는 아이템이 있다고 변경  
		m_bIsItemHere[m_iInsertOrder] = true;
		++m_pInvenList[m_iInsertOrder]->iCnt;

		PRINT_LOG(L"상점 아이템 구매 완료", LOG::CLIENT);
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

	//_vec3 vWndPos = { 500.f, 200.f, 0.f };
	m_pTransformCom[INVEN_WND]->Set_Position(vWndPos);
	//m_pTransformCom[INVEN_SELL_BUTTON]->Set_Position(_vec3(610.f, 380.f, 0.f));
	m_pTransformCom[INVEN_SELL_BUTTON]->Set_Position(_vec3(vWndPos.x + 110.f, vWndPos.y + 180.f, 0.f));
	//m_pTransformCom[INVEN_GOLD]->Set_Position(_vec3(525.f, 335.f, 0.f));
	m_pTransformCom[INVEN_GOLD]->Set_Position(_vec3(vWndPos.x + 25.f, vWndPos.y + 115.f, 0.f));

	_int iIndex = 0;
	for (_uint i = 0; i < 6; ++i)
	{
		for (_uint j = 0; j < 6; ++j)
		{
			iIndex = i * 6 + j;
			//m_vItemPos[i][j].x = (j * 45.f) + 380.f;
			//m_vItemPos[i][j].y = (i * 30.f) + 85.f;
			m_vItemPos[i][j].x = (j * 45.f) + (vWndPos.x - 120.f);
			m_vItemPos[i][j].y = (i * 30.f) + (vWndPos.y - 115.f);
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


	// 인벤 on/off
	if ((CKeyManager::Get_Instance()->Key_Down('I')))
		m_bRender = !m_bRender;

	if (m_bRender)
	{
		// 판매 버튼을 눌렀는지 확인(m_bSelect_SellItem)
		if (FAILED(Check_SellButton()))
			return GAMEOBJECT::WARN;
		// 판매 버튼을 눌렀을 경우
		if (m_bSelect_SellItem)
			// 판매할 아이템 선택
			if (FAILED(Select_SellItem()))
				return GAMEOBJECT::WARN;

		// 자동 정렬 버튼을 눌렀는지 확인
		if (FAILED(Check_AutoSortButton()))
			return GAMEOBJECT::WARN;

		// 아이템 장착
		if (FAILED(Check_EquipItem()))
			return GAMEOBJECT::WARN;

		// 인벤 창 이동
		if (!m_bSelect_SellItem)
		{
			if (FAILED(Move_InventoryWnd()))
				return GAMEOBJECT::WARN;
		}
	}

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
			if (i != INVEN_GOLD)
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
			else if (i == INVEN_GOLD)
			{
				TCHAR		szBuff[MAX_PATH] = L"";
				D3DXMATRIX	matScale, matTrans, matWorld;
				StringCchPrintf(szBuff, sizeof(TCHAR) * MAX_PATH, L"%d", m_iGold);
				
				D3DXMatrixScaling(&matScale, 1.5f, 1.5f, 0.f);
				D3DXMatrixTranslation(&matTrans, vWndPos.x + 50.f, vWndPos.y + 135.f, 0.f);
				matWorld = matScale * matTrans;

				//m_pSprite->SetTransform(&(matScale * m_pTransformCom[i]->Get_Desc().matWorld));
				m_pSprite->SetTransform(&matWorld);
				m_pFont->DrawTextW(m_pSprite, szBuff, lstrlen(szBuff),
					nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
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

		if (PtInRect(&m_tInvenWndCollRt[INVEN_SELL_BUTTON], ptMouse))
		{
			if (!m_bSelect_SellItem)
			{
				m_bSelect_SellItem = true;
				PRINT_LOG(L"판매 모드로 변경", LOG::CLIENT);
			}
			else if (m_bSelect_SellItem)
			{
				// 판매할 아이템 선택 끝
				m_bSelect_SellItem = false;
				// 이제 선택된 아이템들은 빈 공간으로 그려준다
				m_bRenderEmptySell = true;

				m_iGold += m_iSellGold;
				m_iSellGold = 0;

				m_iInsertOrder -= m_iDeleteCnt;
				m_iDeleteCnt = 0;

				PRINT_LOG(L"아이템 판매 시작.", LOG::CLIENT);
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
					_int k = 0;
					// 아이템이 있는 칸들만 선택 할 수 있음
					if (m_bIsItemHere[iIndex] && !m_bSelectedSell[iIndex])
					{
						m_bSelectedSell[iIndex] = true;
						++m_iDeleteCnt;
						m_iSellGold += (m_pInvenList[iIndex]->iPrice * m_pInvenList[iIndex]->iCnt);
					}

				}
			}
		}
	}
	return S_OK;
}

HRESULT CInventory::Check_AutoSortButton()
{
	POINT ptMouse = {};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	int iItemListSize = m_pInvenList.size();

	// 지금은 키입력으로 하는데 나중에 버튼 추가해야 함
	if (CKeyManager::Get_Instance()->Key_Pressing('M'))
	{
		auto& iter = m_pInvenList.begin();
		_uint iSize = m_pInvenList.size();
		int iItemIndex = 0;
		m_bAutoSort = true;

		while (iter != m_pInvenList.end())
		{
			if (m_bSelectedSell[iItemIndex])
			{
				//m_bIsItemHere[iItemIndex] = false;
				Safe_Delete(*iter);
				iter = m_pInvenList.erase(iter);
			}
			else
				++iter;

			++iItemIndex;
		}

		int iTextureItemIdx = 0;
		int iDeleteCnt = 0;
		for (auto& iter = m_pTextureItem.begin(); iter != m_pTextureItem.end(); )
		{
			if (m_bSelectedSell[iTextureItemIdx])
			{
				++iDeleteCnt;
				Safe_Release(*iter);
				iter = m_pTextureItem.erase(iter);
				m_bSelectedSell[iTextureItemIdx] = false;
			}
			else
				++iter;
			++iTextureItemIdx;
		}
		_int m = 0;
		m_pTextureItem.resize(36);
		// 삭제한만큼 새로 만들어서 넣어준다
		for (_int i = 36 - iDeleteCnt, j = 0; j < iDeleteCnt; i++, ++j)
		{
			// Texture-------------------------------------------------------------------
			TCHAR szItemTexture[MAX_STR] = L"";
			TCHAR szItemTextureName[MAX_STR] = L"";
			//ITEM* pItem = new ITEM;
			//pItem->wstrItemName = L"Empty";
			wsprintf(szItemTextureName, L"Component_Texture_Item_Empty");
			wsprintf(szItemTexture, L"Com_NewItemTexture%d", m_iNewInsertOrder);

			if (FAILED(CGameObject::Add_Component(SCENE_STATIC, 
				szItemTextureName, szItemTexture, (CComponent**)&m_pTextureItem[i])))
				return E_FAIL;
			++m_iNewInsertOrder;
		}

		for (_int i = iDeleteCnt + 1; i < 36; ++i)
		{
			m_bIsItemHere[i] = false;
		}

		// 정렬 끝
		//m_bAutoSort = false;
		m_bRenderEmptySell = false;
	}

	return S_OK;
}

HRESULT CInventory::Check_EquipItem()
{
	int iIndex = 0;
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	CEquip* pEquip = (CEquip*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_MainUI", 1);

	for (_uint i = 0; i < 6; i++)
	{
		for (_uint j = 0; j < 6; j++)
		{
			if (CKeyManager::Get_Instance()->Key_Pressing(VK_RBUTTON))
			{
				iIndex = i * 6 + j;
				POINT ptMouse = {};
				GetCursorPos(&ptMouse);
				ScreenToClient(g_hWnd, &ptMouse);
				if (PtInRect(&m_tItemCollRt[i][j], ptMouse))
				{
					_int k = 0;
					// 아이템이 있는 칸들만 선택 할 수 있음
					if (m_bIsItemHere[iIndex])
					{
						// 장비창에게 아이템 정보를 넘겨준다
						//pEquip->Equip_Item(*m_pInvenList[iIndex]);
						pEquip->Equip_Item(m_pInvenList[iIndex]->eSort, m_pInvenList[iIndex]->szItemTag);
						return S_OK;
					}

				}
			}
		}
	}
	return S_OK;
}

HRESULT CInventory::Render_Item()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	size_t iSize = m_pInvenList.size();
	for (_uint i = 0; i < 6; ++i)
	{
		for (_uint j = 0; j < 6; ++j)
		{
			_bool bRenderCnt = true;
			_int iIndex = i * 6 + j;

			
			// 정렬할 때 삭제한 벡터만큼 새로 생성해줘야 함
			// -> 근데 아직 구현안해서 그냥 막아놓음
			if (iIndex >= (_int)iSize)
				return S_OK;

			const D3DXIMAGE_INFO* pTexInfo = m_pTextureItem[iIndex]->Get_TexInfo(0);
			_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };

			_vec3 vPos = m_pTransformItem[iIndex]->Get_Desc().vPosition;

			m_tItemCollRt[i][j].left = (LONG)(vPos.x - vCenter.x);
			m_tItemCollRt[i][j].right = (LONG)(vPos.x + vCenter.x);
			m_tItemCollRt[i][j].top = (LONG)(vPos.y - vCenter.y);
			m_tItemCollRt[i][j].bottom = (LONG)(vPos.y + vCenter.y);

			// 판매 아이템 선택중일 때===================================================================================================
			if (!m_bRenderEmptySell)
			{
				// 판매 아이템으로 선택되었다면
				if (m_bSelectedSell[iIndex])
				{
					bRenderCnt = true;
					D3DXMATRIX matScale, matTrans, matWorld;
					// 선택되었다는 표시를 그리고
					const D3DXIMAGE_INFO* pTexInfo = m_pTextureSell->Get_TexInfo(0);
					_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };
					_vec3 vPos = m_pTransformItem[iIndex]->Get_Desc().vPosition;

					D3DXMatrixScaling(&matScale, 2.7f, 2.7f, 0.f);
					D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, 0.f);
					matWorld = matScale * matTrans;

					m_pSprite->SetTransform(&matWorld);
					m_pSprite->Draw(
						(LPDIRECT3DTEXTURE9)m_pTextureSell->GetTexture(0),
						nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
				}
				// 아이템을 그린다
				const D3DXIMAGE_INFO* pTexInfo = m_pTextureItem[iIndex]->Get_TexInfo(0);
				_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };

				m_pSprite->SetTransform(&m_pTransformItem[iIndex]->Get_Desc().matWorld);
				m_pSprite->Draw(
					(LPDIRECT3DTEXTURE9)m_pTextureItem[iIndex]->GetTexture(0),
					nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
			//===========================================================================================================================


			// 판매 아이템들을 선택한 후, 판매 버튼을 눌렀다면=================================================================================
			else if (m_bRenderEmptySell)
			{
				// 판매 아이템으로 선택된 아이템들은 빈 텍스처로 그린다
				if (m_bSelectedSell[iIndex])
				{
					bRenderCnt = false;
					const D3DXIMAGE_INFO* pTexInfo = m_pTextureEmpty->Get_TexInfo(0);
					_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };

					m_pSprite->SetTransform(&m_pTransformItem[iIndex]->Get_Desc().matWorld);
					m_pSprite->Draw(
						(LPDIRECT3DTEXTURE9)m_pTextureEmpty->GetTexture(0),
						nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
				}
				// 판매 아이템으로 선택되지 않은 아이템들은 그대로 그린다
				else
				{
					m_pSprite->SetTransform(&m_pTransformItem[iIndex]->Get_Desc().matWorld);
					m_pSprite->Draw(
						(LPDIRECT3DTEXTURE9)m_pTextureItem[iIndex]->GetTexture(0),
						nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

				}
			//===========================================================================================================================
			}

			// 아이템 개수
			if (/*!m_bSelectedSell[iIndex] && */bRenderCnt)
			{
				TCHAR		szBuff[MAX_PATH] = L"";
				D3DXMATRIX	matScale, matTrans, matWorld;
				D3DXMatrixIdentity(&matWorld);
				StringCchPrintf(szBuff, sizeof(TCHAR) * MAX_PATH, L"%d", m_pInvenList[iIndex]->iCnt);

				D3DXMatrixTranslation(&matTrans, vPos.x + 10.f, vPos.y + 10.f, 0.f);
				D3DXMatrixScaling(&matScale, 1.5f, 1.5f, 0.f);
				matWorld = matScale * matTrans;

				m_pSprite->SetTransform(&matWorld);
				m_pFont->DrawTextW(m_pSprite, szBuff, lstrlen(szBuff),
					nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
		}
	}

	return S_OK;
}

HRESULT CInventory::Move_InventoryWnd()
{
	POINT ptMouse = {};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (CKeyManager::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		if (PtInRect(&m_tInvenWndCollRt[INVEN_WND], ptMouse))
		{
			vWndPos.x = (_float)ptMouse.x;
			vWndPos.y = (_float)ptMouse.y;
			m_pTransformCom[INVEN_WND]->Set_Position(vWndPos);

			if (Change_AllPos())
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CInventory::Change_AllPos()
{
	D3DXMATRIX matWorld;
	matWorld = m_pTransformCom[INVEN_WND]->Get_Desc().matWorld;

	//for (_uint i = 1; i < INVEN_END; ++i)
	//{
	//	m_pTransformCom[i]->Set_WorldMatrix(
	//		m_pTransformCom[i]->Get_Desc().matWorld * matWorld);
	//}

	m_pTransformCom[INVEN_SELL_BUTTON]->Set_Position(_vec3(vWndPos.x + 110.f, vWndPos.y + 180.f, 0.f));
	//m_pTransformCom[INVEN_GOLD]->Set_Position(_vec3(525.f, 335.f, 0.f));
	m_pTransformCom[INVEN_GOLD]->Set_Position(_vec3(vWndPos.x + 25.f, vWndPos.y + 115.f, 0.f));

	_int iIndex = 0;
	for (_uint i = 0; i < 6; ++i)
	{
		for (_uint j = 0; j < 6; ++j)
		{
			iIndex = i * 6 + j;
			//m_vItemPos[i][j].x = (j * 45.f) + 380.f;
			//m_vItemPos[i][j].y = (i * 30.f) + 85.f;
			m_vItemPos[i][j].x = (j * 45.f) + (vWndPos.x - 120.f);
			m_vItemPos[i][j].y = (i * 30.f) + (vWndPos.y - 115.f);
			m_vItemPos[i][j].z = 0.f;
			m_pTransformItem[iIndex]->Set_Position(m_vItemPos[i][j]);
		}
	}

	return S_OK;
}

HRESULT CInventory::Add_Component()
{
	for (_uint i = 0; i < INVEN_END; ++i)
	{
		// 2. Transform
		TCHAR szTransform[MAX_PATH] = L"";
		StringCchPrintf(szTransform, sizeof(TCHAR) * MAX_PATH,
			L"Com_Transform%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_Transform"
			, szTransform, (CComponent**)&m_pTransformCom[i])))
			return E_FAIL;

		if (i == INVEN_GOLD)
			return S_OK;
		// 3. Texture
		TCHAR szTexture[MAX_PATH] = L"";
		TCHAR szTextureName[MAX_PATH] = L"";
		if (i == INVEN_WND)
			StringCchPrintf(szTextureName, sizeof(TCHAR) * MAX_PATH,
				L"Component_Texture_Inven_InvenWnd");
		if (i == INVEN_SELL_BUTTON)
			StringCchPrintf(szTextureName, sizeof(TCHAR) * MAX_PATH,
				L"Component_Texture_Inven_SellButton");
		StringCchPrintf(szTexture, sizeof(TCHAR) * MAX_PATH,
				L"Com_Texturem%d", i);
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
		TCHAR szItemTexture[MAX_PATH] = L"";
		TCHAR szItemTextureName[MAX_PATH] = L"";
		StringCchPrintf(szItemTexture, sizeof(TCHAR) * MAX_PATH,
			L"Com_ItemTexture%d", i);
		StringCchPrintf(szItemTextureName, sizeof(TCHAR) * MAX_PATH,
			L"Component_Texture_Item_Empty");

		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, szItemTextureName,
			szItemTexture, (CComponent**)&m_pTextureItem[i])))
			return E_FAIL;

		// Transform-------------------------------------------------------------------
		TCHAR szTransform[MAX_PATH] = L"";
		StringCchPrintf(szTransform, sizeof(TCHAR) * MAX_PATH,
			L"Com_ItemTransform%d", i);
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

	return S_OK;
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
		if (i == INVEN_GOLD)
			break;
		Safe_Release(m_pTextureCom[i]);
	}

	for (auto& pItem : m_pTextureItem)
		Safe_Release(pItem);
	m_pTextureItem.clear();
	
	for (auto& pItem : m_pTransformItem)
		Safe_Release(pItem);
	m_pTransformItem.clear();
	//m_pTransformItem.swap(m_pTransformItem);


	for (auto& pItem : m_pInvenList)
		Safe_Delete(pItem);
	m_pInvenList.clear();
	//m_pInvenList.swap(m_pInvenList);

	CUIObject::Free();
}
