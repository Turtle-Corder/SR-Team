#include "stdafx.h"
#include "..\Headers\Flower.h"

USING(Client)

CFlower::CFlower(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
	for (int i = 0; i < FLOWER_END; i++)
	{
		m_pVIBufferCom[i] = nullptr;
		m_pTransformCom[i] = nullptr;
		m_pTextureCom[i] = nullptr;
	}
}

CFlower::CFlower(const CFlower & other)
	: CGameObject(other)
{
}

HRESULT CFlower::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CFlower::Setup_GameObject(void * _pArg)
{
	vector<void*> GetVector;
	_vec3 vPos;
	BYTE chOption;

	if (_pArg)
	{
		GetVector = (*(vector<void*>*)(_pArg));
		vPos = (*(_vec3*)GetVector[0]);
		chOption = (*(BYTE*)GetVector[1]);
	}

	if (FAILED(Add_Component(vPos)))
		return E_FAIL;

	return S_OK;
}

int CFlower::Update_GameObject(float DeltaTime)
{
	return 0;
}

int CFlower::LateUpdate_GameObject(float DeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return 0;

	return 0;
}

HRESULT CFlower::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	for (int i = 0; i < BODY_ID::FLOWER_END; i++)
	{
		if (FAILED(m_pVIBufferCom[i]->Set_Transform(&m_pTransformCom[i]->Get_Desc().matWorld, pCamera)))
			return E_FAIL;

		if (FAILED(m_pTextureCom[i]->SetTexture(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom[i]->Render_VIBuffer()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CFlower::Add_Component()
{ 
	CTransform::TRANSFORM_DESC tTransformDesc[FLOWER_END];

	return S_OK;
}

HRESULT CFlower::Add_Component(_vec3 _vPos)
{

	CTransform::TRANSFORM_DESC tTransformDesc[FLOWER_END];
	tTransformDesc[FLOWER_END].vPosition = _vPos;
	tTransformDesc[FLOWER_END].vPosition = _vPos;

	//For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_TreeHead", L"Com_VIBufferHead", (CComponent**)&m_pVIBufferCom[TREE_HEAD])))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", L"Com_VIBufferBody", (CComponent**)&m_pVIBufferCom[TREE_BODY])))
		return E_FAIL;


	//For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE0, L"Component_Texture_Monster", L"Com_TextureHead", (CComponent**)&m_pTextureCom[TREE_HEAD])))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STAGE0, L"Component_Texture_Monster", L"Com_TextureBody", (CComponent**)&m_pTextureCom[TREE_BODY])))
		return E_FAIL;

	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformHead", (CComponent**)&m_pTransformCom[TREE_HEAD], &tTransformDesc[TREE_HEAD])))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformBody", (CComponent**)&m_pTransformCom[TREE_BODY], &tTransformDesc[TREE_BODY])))
		return E_FAIL;

	return S_OK;
}

CFlower * CFlower::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CFlower* pInstance = new CFlower(pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(_T("Failed To Create CTree"), LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CFlower::Clone_GameObject(void * pArg)
{
	CFlower* pInstance = new CFlower(*this);
	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(_T("Failed To Clone CTree"), LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFlower::Free()
{
	for (_uint iCnt = 0; iCnt < FLOWER_END; iCnt++)
	{
		Safe_Release(m_pTransformCom[iCnt]);
		Safe_Release(m_pVIBufferCom[iCnt]);
		Safe_Release(m_pTextureCom[iCnt]);
	}

	CGameObject::Free();
}
