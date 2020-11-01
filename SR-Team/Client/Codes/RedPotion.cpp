#include "stdafx.h"
#include "..\Headers\RedPotion.h"
#include "Equip.h"
#include "Inventory.h"

USING(Client)


CRedPotion::CRedPotion(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CPlayerItem(_pDevice, _pSprite, _pFont)
{
}

CRedPotion::CRedPotion(const CRedPotion & _rOther)
	: CPlayerItem(_rOther)
{
}

HRESULT CRedPotion::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CRedPotion::Setup_GameObject(void * _pArg)
{
	m_iCanUseCnt = m_iMaxUseCnt = 1;

	return S_OK;
}

_int CRedPotion::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CRedPotion::LateUpdate_GameObject(_float _fDeltaTime)
{
	if (FAILED(Update_Delay(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}


CRedPotion * CRedPotion::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
{
	if (nullptr == pDevice)
		return nullptr;

	CRedPotion* pInstance = new CRedPotion(pDevice, _pSprite, _pFont);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CRedPotion", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRedPotion::Clone_GameObject(void * _pArg)
{
	CRedPotion* pInstance = new CRedPotion(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CRedPotion", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

_bool CRedPotion::Actual_UseItem()
{
	if (!Can_UseItem())
		return false;

	CManagement* pManagement = CManagement::Get_Instance();
	if (pManagement == nullptr)
		return false;
	CEquip* pEquip = (CEquip*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_MainUI", 1);
	if (pEquip == nullptr)
		return false;
	CInventory* pInven = (CInventory*)pManagement->Get_GameObject(pManagement->Get_CurrentSceneID(), L"Layer_Inventory", 0);
	if (pInven == nullptr)
		return false;

	// ���â���� �÷��̾� HP ����
	pEquip->Set_PlayerHp(30);

	// �κ����� ���� ������ ���� ����
	pInven->Use_Potion(RED_POTION);

	return true;
}
