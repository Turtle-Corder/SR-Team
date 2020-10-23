#include "stdafx.h"
#include "..\Headers\Fire.h"

USING(Client)

CFire::CFire(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CFire::CFire(const CFire& _rOther)
	:CGameObject(_rOther)
{
}

HRESULT CFire::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CFire::Setup_GameObject(void * _pArg)
{
	if (_pArg)
	m_vPosition = *(_vec3*)_pArg;

	return S_OK;
}

_int CFire::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CFire::LateUpdate_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

HRESULT CFire::Render_NoneAlpha()
{
	return S_OK;
}

HRESULT CFire::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	tTransformDesc.vPosition = { m_vPosition.x , m_vPosition.y , m_vPosition.z };
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);
	tTransformDesc.vScale = { 1.f , 5.f , 1.f };

	//--------------------------------------------------
	// VIBuffer Component
	//--------------------------------------------------
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	//--------------------------------------------------
	// Transform Component
	//--------------------------------------------------
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;
	//--------------------------------------------------
	// Texture Component
	//--------------------------------------------------
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE0, L"Component_Texture_Bomb", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFire::Movement(_float _fDeltaTime)
{
	return S_OK;
}

CGameObject * CFire::Clone_GameObject(void * _pArg)
{
	CFire* pInstance = new CFire(*this);
	if (FAILED(Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CFire", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFire::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}

CFire * CFire::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if(nullptr == _pDevice)
		return nullptr;

	CFire* pInstance = new CFire(_pDevice);
	
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CFire", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}
