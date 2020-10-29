#include "stdafx.h"
#include "DamageInfo.h"
#include "..\Headers\Wand.h"
#include "DamageInfo.h"

USING(Client)

CWand::CWand(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CWand::CWand(const CWand & _rOther)
	:CGameObject(_rOther)
{
}

HRESULT CWand::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CWand::Setup_GameObject(void * _pArg)
{
	return S_OK;
}

_int CWand::Update_GameObject(_float _fDeltaTime)
{
	

	return GAMEOBJECT::NOEVENT;
}

_int CWand::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;


	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CWand::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	for (_uint iCnt = WAND_HEAD; iCnt < WAND_END; ++iCnt)
	{
		if (FAILED(m_pVIBufferCom[iCnt]->Set_Transform(&m_pTransformCom[iCnt]->Get_Desc().matWorld, pCamera)))
			return E_FAIL;

		if (FAILED(m_pTextureCom[iCnt]->SetTexture(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom[iCnt]->Render_VIBuffer()))
			return E_FAIL;

	}

	return S_OK;
}

HRESULT CWand::Add_Component()
{
	return S_OK;
}

HRESULT CWand::Movement(_float _fDeltaTime)
{
	return S_OK;
}

CGameObject* CWand::Clone_GameObject(void * _pArg)
{
	CWand* pInstance = new CWand(*this);

	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CWand", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWand::Free()
{
	for (_uint iCnt = 0; iCnt < WAND_END; ++iCnt)
	{
		Safe_Release(m_pTransformCom[iCnt]);
		Safe_Release(m_pVIBufferCom[iCnt]);
		Safe_Release(m_pTextureCom[iCnt]);
	}

	Safe_Release(m_pStatusComp);
	Safe_Release(m_pDmgInfoCom);

	CGameObject::Free();
}

CWand * CWand::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CWand* pInstance = new CWand(pDevice);

	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CWand", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CWand::Take_Damage(const CComponent * _pDamageComp)
{

	return S_OK;
}

HRESULT CWand::Change_State(STATE _eState)
{
	return E_NOTIMPL;
}

