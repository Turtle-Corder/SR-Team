#include "stdafx.h"
#include "..\Headers\BluePotion.h"
#include "Equip.h"
#include "Inventory.h"

USING(Client)

CBluePotion::CBluePotion(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CPlayerItem(_pDevice, _pSprite, _pFont)
{
}

CBluePotion::CBluePotion(const CBluePotion & _rOther)
	: CPlayerItem(_rOther)
{
}

HRESULT CBluePotion::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CBluePotion::Setup_GameObject(void * _pArg)
{
	return S_OK;
}

_int CBluePotion::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CBluePotion::LateUpdate_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

CBluePotion * CBluePotion::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == pDevice)
		return nullptr;

	CBluePotion* pInstance = new CBluePotion(pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CBluePotion", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBluePotion::Clone_GameObject(void * _pArg)
{
	CBluePotion* pInstance = new CBluePotion(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CBluePotion", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CBluePotion::Use_Item()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return E_FAIL;
	CEquip* pEquip = (CEquip*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_MainUI", 1);
	if (pEquip == nullptr)
		return E_FAIL;
	CInventory* pInven = (CInventory*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Inventory", 0);
	if (pInven == nullptr)
		return E_FAIL;

	// 장비창에서 플레이어 MP 증가
	pEquip->Set_PlayerMP(30);
	
	// 인벤에서 포션 아이템 개수 감소
	pInven->Use_Potion(BLUE_POTION);

	return S_OK;
}
