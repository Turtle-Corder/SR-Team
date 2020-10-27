#include "stdafx.h"
#include "UICamera.h"
#include "KeyManager.h"
#include "Item.h"
#include "Inventory.h"
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

	for (_uint i = 0; i < 8; i++)
	{
		m_pTransformLeftSlot[i] = nullptr;
		m_pTransformRightSlot[i] = nullptr;

		m_pTextureLeftQuickSlot[i] = nullptr;
		m_pTextureRightQuickSlot[i] = nullptr;

		m_pLeftSlotItem[i] = nullptr;
		m_pRightSlotItem[i] = nullptr;
	}
}

CMainUI::CMainUI(const CMainUI & _rOther)
	: CUIObject(_rOther)
{
}

HRESULT CMainUI::Get_QuickSlotItem(INVEN_ITEM * pItem)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return E_FAIL;
	CItem* pItems = (CItem*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Item");

	m_pMovingItem = pItem;

	m_bRender_GoingItem = true;

	_int p = 0;

	if (m_pTexture_GoingItem)
		m_pTexture_GoingItem = nullptr;
	m_pTexture_GoingItem = pItems->Get_ItemInfo_Texture(m_pMovingItem->szItemTag);

	return S_OK;
}

HRESULT CMainUI::Delete_Potion(INVEN_ITEM * pItem)
{
	for (_uint i = 0; i < 8; i++)
	{
		if (m_pRightSlotItem[i] != nullptr)
		{
			if (!wcscmp(pItem->szItemTag, m_pRightSlotItem[i]->szItemTag))
			{
				m_pRightSlotItem[i]->iCnt = 0;
				return S_OK;
			}
		}
	}

	return S_OK;
}

HRESULT CMainUI::Get_QuickSlotSkill(_int iSkillID)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return E_FAIL;
	CItem* pItems = (CItem*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Item");

	// 받아온 스킬 아이디로 아이템 클래스에서 스킬 아이콘 객체를 받아온다
	m_pMovingItem = pItems->Get_ActiveSkillIcon(iSkillID);
	if (m_pMovingItem == nullptr)
		return E_FAIL;

	m_bRender_GoingItem = true;

	if (m_pTexture_GoingItem)
		m_pTexture_GoingItem = nullptr;
	m_pTexture_GoingItem = pItems->Get_ItemInfo_Texture(m_pMovingItem->szItemTag);
	if (m_pTexture_GoingItem == nullptr)
		return E_FAIL;

	return S_OK;
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

	for (_uint i = 0; i < 8; i++)
	{
		m_pTextureLeftQuickSlot[i] = nullptr;
		m_pTextureRightQuickSlot[i] = nullptr;

		m_pLeftSlotItem[i] = nullptr;
		m_pRightSlotItem[i] = nullptr;
	}

	m_vPos[MAINUI_MAIN] = _vec3(WINCX * 0.5f, WINCY - 100.f, 0.f);
	m_pTransformCom[MAINUI_MAIN]->Set_Position(m_vPos[MAINUI_MAIN]);

	m_vPos[MAINUI_HP] = _vec3(380.f, 500.f, 0.f);
	m_pTransformCom[MAINUI_HP]->Set_Position(m_vPos[MAINUI_HP]);

	m_vPos[MAINUI_MP] = _vec3(420.f, 500.f, 0.f);
	m_pTransformCom[MAINUI_MP]->Set_Position(m_vPos[MAINUI_MP]);

	//m_vPos[MAINUI_QUICKSLOT_LFFT] = _vec3(170.f, 500.f, 0.f);
	m_pTransformCom[MAINUI_QUICKSLOT_LFFT]->Set_Position(m_vLeftSlotPos);

	//m_vPos[MAINUI_QUICKSLOT_RIGHT] = _vec3(660.f, 500.f, 0.f);
	m_pTransformCom[MAINUI_QUICKSLOT_RIGHT]->Set_Position(m_vRightSlotPos);

	
	// 왼쪽 퀵슬롯 충돌 렉트
	for (_uint i = 0; i < 4; ++i)
	{
		_vec3 vPos = {};
		//vPos.x = (i * 39.f) + 97.f;
		vPos.x = (i * 39.f) + (m_vLeftSlotPos.x - 73.f);
		vPos.y = 472.f;
		vPos.z = 0.f;

		m_pTransformLeftSlot[i]->Set_Position(vPos);

		m_tLeftSlotCollRt[i].left = (LONG)(vPos.x - 16.f);
		m_tLeftSlotCollRt[i].right = (LONG)(vPos.x + 16.f);
		m_tLeftSlotCollRt[i].top = (LONG)(vPos.y - 16.f);
		m_tLeftSlotCollRt[i].bottom = (LONG)(vPos.y + 16.f);
	}
	for (_uint i = 4, j = 0; i < 8; ++i, ++j)
	{
		_vec3 vPos = {};
		//vPos.x = (j * 39.f) + 110.f;
		vPos.x = (j * 39.f) + (m_vLeftSlotPos.x - 60.f);
		vPos.y = 510.f;
		vPos.z = 0.f;

		m_pTransformLeftSlot[i]->Set_Position(vPos);

		m_tLeftSlotCollRt[i].left = (LONG)(vPos.x - 16.f);
		m_tLeftSlotCollRt[i].right = (LONG)(vPos.x + 16.f);
		m_tLeftSlotCollRt[i].top = (LONG)(vPos.y - 16.f);
		m_tLeftSlotCollRt[i].bottom = (LONG)(vPos.y + 16.f);
	}

	// 오른쪽 퀵슬롯 위치
	for (_uint i = 0; i < 4; ++i)
	{
		_vec3 vPos = {};
		//vPos.x = (i * 39.f) + 97.f;
		vPos.x = (i * 39.f) + (m_vRightSlotPos.x - 75.f);
		vPos.y = 472.f;
		vPos.z = 0.f;

		m_pTransformRightSlot[i]->Set_Position(vPos);

		m_tRightSlotCollRt[i].left = (LONG)(vPos.x - 16.f);
		m_tRightSlotCollRt[i].right = (LONG)(vPos.x + 16.f);
		m_tRightSlotCollRt[i].top = (LONG)(vPos.y - 16.f);
		m_tRightSlotCollRt[i].bottom = (LONG)(vPos.y + 16.f);
	}
	for (_uint i = 4, j = 0; i < 8; ++i, ++j)
	{
		_vec3 vPos = {};
		//vPos.x = (j * 39.f) + 110.f;
		vPos.x = (j * 39.f) + (m_vRightSlotPos.x - 48.f);
		vPos.y = 510.f;
		vPos.z = 0.f;

		m_pTransformRightSlot[i]->Set_Position(vPos);

		m_tRightSlotCollRt[i].left = (LONG)(vPos.x - 16.f);
		m_tRightSlotCollRt[i].right = (LONG)(vPos.x + 16.f);
		m_tRightSlotCollRt[i].top = (LONG)(vPos.y - 16.f);
		m_tRightSlotCollRt[i].bottom = (LONG)(vPos.y + 16.f);
	}

	return S_OK;
}

int CMainUI::Update_GameObject(float DeltaTime)
{
	if (CKeyManager::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		POINT ptMouse = {};
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		TCHAR szBuff[MAX_PATH] = L"";
		StringCchPrintf(szBuff, sizeof(TCHAR) * MAX_PATH,
			L"X : %d, Y : %d", ptMouse.x, ptMouse.y);
		//PRINT_LOG(szBuff, LOG::CLIENT);
	}

	if (m_bRender_GoingItem)
	{
		if (FAILED(Move_To_QuickSlot()))
			return GAMEOBJECT::WARN;
		if (FAILED(Check_LeftQuickSlot_Item()))
			return GAMEOBJECT::WARN;
		if (FAILED(Check_RightQuickSlot_Item()))
			return GAMEOBJECT::WARN;
	}

	if (FAILED(Set_SlotItem_Count()))
		return GAMEOBJECT::WARN;

#pragma region Move_MainUI
	if (GetAsyncKeyState('P') & 0x8000)
	{
		for (_uint i = 0; i < MAINUI_END; ++i)
			m_pTransformCom[i]->Set_Position(m_pTransformCom[i]->Get_Desc().vPosition - _vec3(0.f, 10.f, 0.f));
		for (_uint i = 0; i < 8; ++i)
		{
			m_pTransformLeftSlot[i]->Set_Position(m_pTransformLeftSlot[i]->Get_Desc().vPosition - _vec3(0.f, 10.f, 0.f));
			m_pTransformRightSlot[i]->Set_Position(m_pTransformRightSlot[i]->Get_Desc().vPosition - _vec3(0.f, 10.f, 0.f));
		}
	}

	if (GetAsyncKeyState('L') & 0x8000)
	{
		for (_uint i = 0; i < MAINUI_END; ++i)
			m_pTransformCom[i]->Set_Position(m_pTransformCom[i]->Get_Desc().vPosition + _vec3(0.f, 10.f, 0.f));
		for (_uint i = 0; i < 8; ++i)
		{
			m_pTransformLeftSlot[i]->Set_Position(m_pTransformLeftSlot[i]->Get_Desc().vPosition + _vec3(0.f, 10.f, 0.f));
			m_pTransformRightSlot[i]->Set_Position(m_pTransformRightSlot[i]->Get_Desc().vPosition + _vec3(0.f, 10.f, 0.f));
		}
	}

	for (_uint i = 0; i < MAINUI_END; ++i)
		m_pTransformCom[i]->Update_Transform();
	for (_uint i = 0; i < 8; i++)
	{
		m_pTransformLeftSlot[i]->Update_Transform();
		m_pTransformRightSlot[i]->Update_Transform();
	}
#pragma endregion

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
	_int iHP = 50;
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	for (_uint i = 0; i < MAINUI_END; ++i)
	{
		const D3DXIMAGE_INFO* pTexInfo = m_pTextureCom[i]->Get_TexInfo(0);
		_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };

		// - HP/MP 지연 감소
		// HP/MP 감소에 따른 RECT 변경 나중에 하기
		if (i == MAINUI_HP || i == MAINUI_MP)
		{
		}

		m_tCollRt[i].left = 0;
		m_tCollRt[i].right = (LONG)(pTexInfo->Width + (iHP - 50));
		m_tCollRt[i].top = 0;
		m_tCollRt[i].bottom = (LONG)(pTexInfo->Height);

		m_pSprite->SetTransform(&m_pTransformCom[i]->Get_Desc().matWorld);
		m_pSprite->Draw(
			(LPDIRECT3DTEXTURE9)m_pTextureCom[i]->GetTexture(0),
			&m_tCollRt[i], &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (m_bRender_GoingItem)
		if (FAILED(Render_Item_GoingToQuickSlot()))
			return E_FAIL;
	if (FAILED(Render_QuickSlot_Item()))
		return E_FAIL;

	//// 왼쪽
	//for (_uint i = 0; i < 8; ++i)
	//{
	//	const D3DXIMAGE_INFO* pTexInfo = m_pEmptyTexture->Get_TexInfo(0);
	//	_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };
	//	_vec3 vPos = m_pTransformLeftSlot[i]->Get_Desc().vPosition;

	//	m_tLeftSlotCollRt[i].left = (LONG)(vPos.x - 16.f);
	//	m_tLeftSlotCollRt[i].right = (LONG)(vPos.x + 16.f);
	//	m_tLeftSlotCollRt[i].top = (LONG)(vPos.y - 16.f);
	//	m_tLeftSlotCollRt[i].bottom = (LONG)(vPos.y + 16.f);

	//	m_pSprite->SetTransform(&m_pTransformLeftSlot[i]->Get_Desc().matWorld);
	//	m_pSprite->Draw(
	//		(LPDIRECT3DTEXTURE9)m_pEmptyTexture->GetTexture(0),
	//		&m_tLeftSlotCollRt[i], &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	//}

	//// 오른쪽
	//for (_uint i = 0; i < 8; ++i)
	//{
	//	const D3DXIMAGE_INFO* pTexInfo = m_pEmptyTexture->Get_TexInfo(0);
	//	_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };
	//	_vec3 vPos = m_pTransformRightSlot[i]->Get_Desc().vPosition;

	//	m_tRightSlotCollRt[i].left = (LONG)(vPos.x - 16.f);
	//	m_tRightSlotCollRt[i].right = (LONG)(vPos.x + 16.f);
	//	m_tRightSlotCollRt[i].top = (LONG)(vPos.y - 16.f);
	//	m_tRightSlotCollRt[i].bottom = (LONG)(vPos.y + 16.f);

	//	m_pSprite->SetTransform(&m_pTransformRightSlot[i]->Get_Desc().matWorld);
	//	m_pSprite->Draw(
	//		(LPDIRECT3DTEXTURE9)m_pEmptyTexture->GetTexture(0),
	//		&m_tRightSlotCollRt[i], &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	//} 

	return S_OK;
}

HRESULT CMainUI::Move_To_QuickSlot()
{
	POINT ptMouse = {};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	m_vGoingItem_Pos.x = (_float)ptMouse.x;
	m_vGoingItem_Pos.y = (_float)ptMouse.y;
	m_vGoingItem_Pos.z = 0.f;

	m_tGoingItem_CollRt.left = (LONG)(m_vGoingItem_Pos.x - 20.f);
	m_tGoingItem_CollRt.right = (LONG)(m_vGoingItem_Pos.x + 20.f);
	m_tGoingItem_CollRt.top = (LONG)(m_vGoingItem_Pos.y - 20.f);
	m_tGoingItem_CollRt.bottom = (LONG)(m_vGoingItem_Pos.y + 20.f);


	return S_OK;
}

HRESULT CMainUI::Check_LeftQuickSlot_Item()
{
	RECT rc = {};

	for (_uint i = 0; i < 8; i++)
	{
		// 왼쪽
		if (IntersectRect(&rc, &m_tLeftSlotCollRt[i], &m_tGoingItem_CollRt))
		{
			if (CKeyManager::Get_Instance()->Key_Up(VK_LBUTTON))
			{
				// 스킬 전용 슬롯
				if (m_pMovingItem->eSort == SKILL_ICON)
				{
					if (Check_Item_In_Slot())
					{
						// 초기화
						m_bRender_GoingItem = false;

						if (m_pTextureLeftQuickSlot[i])
							m_pTextureLeftQuickSlot[i] = nullptr;
						m_pTextureLeftQuickSlot[i] = m_pTexture_GoingItem;
						m_pTexture_GoingItem = nullptr;

						if (m_pLeftSlotItem[i])
							m_pLeftSlotItem[i] = nullptr;
						m_pLeftSlotItem[i] = m_pMovingItem;
						m_pMovingItem = nullptr;
					}
					else
					{
						m_bRender_GoingItem = false;
						m_pMovingItem = nullptr;
						m_pTexture_GoingItem = nullptr;
					}
				}
				else
				{
					m_bRender_GoingItem = false;
					m_pMovingItem = nullptr;
					m_pTexture_GoingItem = nullptr;
				}
			}
		}
	}

	return S_OK;
}

HRESULT CMainUI::Check_RightQuickSlot_Item()
{
	RECT rc = {};

	for (_uint i = 0; i < 8; i++)
	{
		// 왼쪽
		if (IntersectRect(&rc, &m_tRightSlotCollRt[i], &m_tGoingItem_CollRt))
		{
			if (CKeyManager::Get_Instance()->Key_Up(VK_LBUTTON))
			{
				// 스킬 전용 슬롯
				if (m_pMovingItem->eSort == POTION)
				{
					if (Check_Item_In_Slot())
					{
						// 초기화
						m_bRender_GoingItem = false;

						if (m_pTextureRightQuickSlot[i])
							m_pTextureRightQuickSlot[i] = nullptr;
						m_pTextureRightQuickSlot[i] = m_pTexture_GoingItem;
						m_pTexture_GoingItem = nullptr;

						if (m_pRightSlotItem[i])
							m_pRightSlotItem[i] = nullptr;
						m_pRightSlotItem[i] = m_pMovingItem;
						m_pMovingItem = nullptr;
					}
					else
					{
						m_bRender_GoingItem = false;
						m_pMovingItem = nullptr;
						m_pTexture_GoingItem = nullptr;
					}
				}
				else
				{
					m_bRender_GoingItem = false;
					m_pMovingItem = nullptr;
					m_pTexture_GoingItem = nullptr;
				}
			}
		}
	}

	return S_OK;
}

_bool CMainUI::Check_Item_In_Slot()
{
	// 현재 퀵슬롯에 해당 아이템이 있는지 없는지 검사
	for (_uint i = 0; i < 8; ++i)
	{
		if (m_pLeftSlotItem[i] != nullptr)
		{
			// 현재 퀵슬롯에 같은 이름의 아이템이 있으면
			if (!wcscmp(m_pMovingItem->szItemTag, m_pLeftSlotItem[i]->szItemTag))
			{
				PRINT_LOG(L"이미 장착하고 있는 아이템", LOG::CLIENT);
				return false;
			}
		}
		if (m_pRightSlotItem[i] != nullptr)
		{
			// 현재 퀵슬롯에 같은 이름의 아이템이 있으면
			if (!wcscmp(m_pMovingItem->szItemTag, m_pRightSlotItem[i]->szItemTag))
			{
				PRINT_LOG(L"이미 장착하고 있는 아이템", LOG::CLIENT);
				return false;
			}
		}
	}

	return true;
}

HRESULT CMainUI::Set_SlotItem_Count()
{
	CManagement* pManagement = CManagement::Get_Instance();
	CInventory* pInven = (CInventory*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Inventory");

	for (_uint i = 0; i < 8; ++i)
	{
		if (m_pRightSlotItem[i] != nullptr)
		{
			m_pRightSlotItem[i]->iCnt = pInven->Get_ItemCount(m_pRightSlotItem[i]->szItemTag);
			if (m_pRightSlotItem[i]->iCnt == 0 || m_pRightSlotItem[i]->iCnt == -1)
			{
				m_pTextureRightQuickSlot[i] = nullptr;
				m_pRightSlotItem[i] = nullptr;
			}
		}
	}

	return S_OK;
}

HRESULT CMainUI::Render_Item_GoingToQuickSlot()
{
	const D3DXIMAGE_INFO* pTexInfo = m_pTexture_GoingItem->Get_TexInfo(0);
	_vec3 vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };
	_vec3 vPos = m_vGoingItem_Pos;
	D3DXMATRIX matTrans, matWorld;

	m_tGoingItem_CollRt.left = (LONG)(vPos.x - vCenter.x);
	m_tGoingItem_CollRt.right = (LONG)(vPos.x + vCenter.x);
	m_tGoingItem_CollRt.top = (LONG)(vPos.y - vCenter.y);
	m_tGoingItem_CollRt.bottom = (LONG)(vPos.y + vCenter.y);

	D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, 0.f);
	matWorld = matTrans;

	m_pSprite->SetTransform(&matWorld);
	m_pSprite->Draw(
		(LPDIRECT3DTEXTURE9)m_pTexture_GoingItem->GetTexture(0),
		nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	return S_OK;
}

HRESULT CMainUI::Render_QuickSlot_Item()
{
	_int k = 0;
	_vec3 vCenter = { 0.f, 0.f, 0.f };
	_vec3 vPos = { 0.f, 0.f, 0.f };
	D3DXMATRIX matTrans, matWorld;

	for (_uint i = 0; i < 8; ++i)
	{
		if (m_pTextureLeftQuickSlot[i] != nullptr)
		{
			const D3DXIMAGE_INFO* pTexInfo = m_pTextureLeftQuickSlot[i]->Get_TexInfo(0);
			vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };
			vPos = m_pTransformLeftSlot[i]->Get_Desc().vPosition;

			m_tGoingItem_CollRt.left = (LONG)(vPos.x - vCenter.x);
			m_tGoingItem_CollRt.right = (LONG)(vPos.x + vCenter.x);
			m_tGoingItem_CollRt.top = (LONG)(vPos.y - vCenter.y);
			m_tGoingItem_CollRt.bottom = (LONG)(vPos.y + vCenter.y);

			D3DXMatrixTranslation(&matTrans, vPos.x + 7.f, vPos.y + 10.f, 0.f);
			matWorld = matTrans;

			m_pSprite->SetTransform(&matWorld);
			m_pSprite->Draw(
				(LPDIRECT3DTEXTURE9)m_pTextureLeftQuickSlot[i]->GetTexture(0),
				nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		if (m_pTextureRightQuickSlot[i] != nullptr)
		{
			const D3DXIMAGE_INFO* pTexInfo = m_pTextureRightQuickSlot[i]->Get_TexInfo(0);
			vCenter = { pTexInfo->Width * 0.5f, pTexInfo->Height * 0.5f, 0.f };
			vPos = m_pTransformRightSlot[i]->Get_Desc().vPosition;

			m_tGoingItem_CollRt.left = (LONG)(vPos.x - vCenter.x);
			m_tGoingItem_CollRt.right = (LONG)(vPos.x + vCenter.x);
			m_tGoingItem_CollRt.top = (LONG)(vPos.y - vCenter.y);
			m_tGoingItem_CollRt.bottom = (LONG)(vPos.y + vCenter.y);

			D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, 0.f);
			matWorld = matTrans;

			m_pSprite->SetTransform(&matWorld);
			m_pSprite->Draw(
				(LPDIRECT3DTEXTURE9)m_pTextureRightQuickSlot[i]->GetTexture(0),
				nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			// 개수
			TCHAR		szBuff[MAX_PATH] = L"";
			D3DXMATRIX	matScale2, matTrans2, matWorld2;
			D3DXMatrixIdentity(&matWorld);
			StringCchPrintf(szBuff, sizeof(TCHAR) * MAX_PATH, L"%d", m_pRightSlotItem[i]->iCnt);

			D3DXMatrixTranslation(&matTrans2, vPos.x + 10.f, vPos.y + 10.f, 0.f);
			D3DXMatrixScaling(&matScale2, 1.5f, 1.5f, 0.f);
			matWorld2 = matScale2 * matTrans2;

			m_pSprite->SetTransform(&matWorld2);
			m_pFont->DrawTextW(m_pSprite, szBuff, lstrlen(szBuff),
				nullptr, 0, D3DCOLOR_ARGB(255, 0, 0, 0));
		}
	}

	return S_OK;
}

HRESULT CMainUI::Add_Component()
{
	for (_uint i = 0; i < MAINUI_END; ++i)
	{
		// 1. VIBuffer
		TCHAR szVIBuffer[MAX_PATH] = L"";
		StringCchPrintf(szVIBuffer, sizeof(TCHAR) * MAX_PATH,
			L"Com_VIBuffer%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_VIBuffer_RectTexture"
			, szVIBuffer, (CComponent**)&m_pVIBufferCom[i])))
			return E_FAIL;

		// 2. Transform
		TCHAR szTransform[MAX_PATH] = L"";
		StringCchPrintf(szTransform, sizeof(TCHAR) * MAX_PATH,
			L"Com_Transform%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_Transform"
			, szTransform, (CComponent**)&m_pTransformCom[i])))
			return E_FAIL;

		// 3. Texture
		TCHAR szTexture[MAX_PATH] = L"";
		TCHAR szTextureName[MAX_PATH] = L"";
		wstring strTexture = L"";
		wstring strTextureName = L"";

		StringCchPrintf(szTexture, sizeof(TCHAR) * MAX_PATH,
			L"Com_Texture%d", i);
		strTexture = szTexture;
		if (i == MAINUI_MAIN)
			StringCchPrintf(szTextureName, sizeof(TCHAR) * MAX_PATH,
				L"Component_Texture_MainUI_Main");
		else if (i == MAINUI_HP)
			StringCchPrintf(szTextureName, sizeof(TCHAR) * MAX_PATH,
				L"Component_Texture_MainUI_Hp");
		else if (i == MAINUI_MP)
			StringCchPrintf(szTextureName, sizeof(TCHAR) * MAX_PATH,
				L"Component_Texture_MainUI_Mp");
		else if (i == MAINUI_QUICKSLOT_LFFT)
			StringCchPrintf(szTextureName, sizeof(TCHAR) * MAX_PATH,
				L"Component_Texture_MainUI_QuickSlot_Left");
		else if (i == MAINUI_QUICKSLOT_RIGHT)
			StringCchPrintf(szTextureName, sizeof(TCHAR) * MAX_PATH,
				L"Component_Texture_MainUI_QuickSlot_Right");
		strTextureName = szTextureName;

		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, strTextureName
			, strTexture, (CComponent**)&m_pTextureCom[i])))
			return E_FAIL;
	}

	for (_uint i = 0; i < 8; i++)
	{
		// 2. Transform
		TCHAR szTransform[MAX_PATH] = L"";
		StringCchPrintf(szTransform, sizeof(TCHAR) * MAX_PATH,
			L"Com_LeftTransform%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_Transform"
			, szTransform, (CComponent**)&m_pTransformLeftSlot[i])))
			return E_FAIL;

		TCHAR szRightTransform[MAX_PATH] = L"";
		StringCchPrintf(szRightTransform, sizeof(TCHAR) * MAX_PATH,
			L"Com_RightTransform%d", i);
		if (FAILED(CGameObject::Add_Component(
			SCENE_STATIC, L"Component_Transform"
			, szRightTransform, (CComponent**)&m_pTransformRightSlot[i])))
			return E_FAIL;
	}

	if (FAILED(CGameObject::Add_Component(
		SCENE_STATIC, L"Component_Texture_Item_Empty",
		L"Com_EmptyItemTexture", (CComponent**)&m_pEmptyTexture)))
		return E_FAIL;

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
	Safe_Release(m_pEmptyTexture);
	Safe_Release(m_pTexture_GoingItem);

	for (_uint i = 0; i < MAINUI_END; ++i)
	{
		Safe_Release(m_pTextureCom[i]);
		Safe_Release(m_pTransformCom[i]);
		Safe_Release(m_pVIBufferCom[i]);
	}

	for (_uint i = 0; i < 8; i++)
	{
		Safe_Release(m_pTransformLeftSlot[i]);
		Safe_Release(m_pTransformRightSlot[i]);
		//Safe_Release(m_pTextureLeftQuickSlot[i]);
	}

	CUIObject::Free();
}
