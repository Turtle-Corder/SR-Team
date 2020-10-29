#include "stdafx.h"
#include "..\Headers\BlueElixir.h"
#include "Equip.h"
#include "Inventory.h"

USING(Client)

CBlueElixir::CBlueElixir(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CPlayerItem(_pDevice, _pSprite, _pFont)
{
}

CBlueElixir::CBlueElixir(const CBlueElixir & _rOther)
	: CPlayerItem(_rOther)
{
}

HRESULT CBlueElixir::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CBlueElixir::Setup_GameObject(void * _pArg)
{
	return S_OK;
}

_int CBlueElixir::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CBlueElixir::LateUpdate_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

CBlueElixir * CBlueElixir::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == pDevice)
		return nullptr;

	CBlueElixir* pInstance = new CBlueElixir(pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CBlueElixir", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBlueElixir::Clone_GameObject(void * _pArg)
{
	CBlueElixir* pInstance = new CBlueElixir(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CBlueElixir", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CBlueElixir::Use_Item()
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

	// 장비창에서 플레이어 HP 증가
	pEquip->Set_PlayerMP(50);

	// 인벤에서 포션 아이템 개수 감소
	pInven->Use_Potion(BLUE_ELIXIR);

	return S_OK;
}
