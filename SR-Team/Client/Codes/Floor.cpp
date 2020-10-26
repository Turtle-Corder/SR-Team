#include "stdafx.h"
#include "..\Headers\Floor.h"

USING(Client)

CFloor::CFloor(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CFloor::CFloor(const CFloor & _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CFloor::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CFloor::Setup_GameObject(void * _pArg)
{
	m_vPos = *(_vec3*)_pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CFloor::Update_GameObject(_float _fDeltaTime)
{
	return GAMEOBJECT::NOEVENT;
}

_int CFloor::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(m_pTransformCom->Update_Transform()))
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return GAMEOBJECT::ERR;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CFloor::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Set_Transform(&m_pTransformCom->Get_Desc().matWorld, pCamera)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetTexture(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFloor::Add_Component()
{
	
	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.vPosition = { m_vPos.x , m_vPos.y + 3.f , m_vPos.z };
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);
	tTransformDesc.vScale = { 1.f , 1.f , 1.f };

	if (FAILED(CGameObject::Add_Component(SCENE_STAGE0, L"Component_VIBuffer_TerrainTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STAGE0, L"Component_Texture_Floor", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom , &tTransformDesc)))
		return E_FAIL;
	
	return S_OK;
}

CFloor* CFloor::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CFloor* pInstance = new CFloor(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CFloor", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CFloor::Clone_GameObject(void * _pArg)
{
	CFloor* pInstance = new CFloor(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CFloor", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFloor::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}
