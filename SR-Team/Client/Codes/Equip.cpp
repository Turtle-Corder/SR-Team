#include "stdafx.h"
#include "..\Headers\Equip.h"


USING(Client)

CEquip::CEquip(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
}

CEquip::CEquip(const CEquip & other)
	: CUIObject(other)
{
}

HRESULT CEquip::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CEquip::Setup_GameObject(void * _pArg)
{
	return S_OK;
}

_int CEquip::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CEquip::LateUpdate_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

CEquip * CEquip::Create(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
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
}
