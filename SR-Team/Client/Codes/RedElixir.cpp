#include "stdafx.h"
#include "..\Headers\RedElixir.h"
#include "Equip.h"
#include "Inventory.h"
USING(Client)


CRedElixir::CRedElixir(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CPlayerItem(_pDevice, _pSprite, _pFont)
{
}

CRedElixir::CRedElixir(const CRedElixir & _rOther)
	: CPlayerItem(_rOther)
{
}

HRESULT CRedElixir::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CRedElixir::Setup_GameObject(void * _pArg)
{
	return S_OK;
}

_int CRedElixir::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CRedElixir::LateUpdate_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

CRedElixir * CRedElixir::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == pDevice)
		return nullptr;

	CRedElixir* pInstance = new CRedElixir(pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CRedElixir", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRedElixir::Clone_GameObject(void * _pArg)
{
	CRedElixir* pInstance = new CRedElixir(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CRedElixir", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CRedElixir::Use_Item()
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
	pEquip->Set_PlayerHp(50);

	// 인벤에서 포션 아이템 개수 감소
	pInven->Use_Potion(RED_ELIXIR);

	return S_OK;
}
