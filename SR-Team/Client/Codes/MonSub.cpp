#include "stdafx.h"
#include "..\Headers\MonSub.h"

USING(Client)

CMonSub::CMonSub(LPDIRECT3DDEVICE9 _pDevice)
	:CGameObject(_pDevice)
{
}

CMonSub::CMonSub(const CMonSub& _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CMonSub::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CMonSub::Setup_GameObject(void * _pArg)
{
	if (_pArg)
		m_vStartPos = *(_vec3*)_pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CMonSub::Update_GameObject(_float _fDeltaTime)
{
	if (Movement(_fDeltaTime))
		return 0;

	return 0;
}

_int CMonSub::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(m_pTransformCom->Update_Transform()))
		return GAMEOBJECT::WARN;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return 0;

	return 0;
}

HRESULT CMonSub::Render_NoneAlpha()
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

HRESULT CMonSub::Add_Component()
{
	CTransform::TRANSFORM_DESC tTransformDesc;

	tTransformDesc.vPosition = { m_vStartPos.x , 1.f , m_vStartPos.z };
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);
	tTransformDesc.vScale = { 1.f , 1.f , 1.f };
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
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE0, L"Component_Texture_Translucent_Cube", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CMonSub::Movement(_float _fDeltaTime)
{
	if (FAILED(Setting_Dir()))
		return E_FAIL;

	if (FAILED(IsOnTerrain(_fDeltaTime)))
		return E_FAIL;


	return S_OK;
}

HRESULT CMonSub::IsOnTerrain(_float _fDeltaTime)
{
	D3DXVECTOR3 vPos = m_pTransformCom->Get_Desc().vPosition;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CVIBuffer_TerrainTexture* pTerrainBuffer = (CVIBuffer_TerrainTexture*)pManagement->Get_Component(SCENE_STAGE0, L"Layer_Terrain", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	D3DXVECTOR3 vPosition = m_pTransformCom->Get_Desc().vPosition;

	if (true == pTerrainBuffer->IsOnTerrain(&vPosition))
	{
		m_pTransformCom->Set_Position(vPosition);
	}

	if (m_bFallDown)
	{
		vPos.y += m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime;
		m_fJumpTime += _fDeltaTime;

		//vPos += _vec3(m_vDir.x, 0.f, m_vDir.z) * (_fDeltaTime * 10.f);
	}

	if (vPosition.y > vPos.y)
	{
		m_bFallDown = false;
	}

	m_pTransformCom->Set_Position(_vec3(vPos.x, vPos.y, vPos.z));

	return S_OK;
}
CGameObject* CMonSub::Clone_GameObject(void * _pArg)
{
	CMonSub* pInstance = new CMonSub(*this);

	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CMonSub", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonSub::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}

CMonSub * CMonSub::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CMonSub* pInstance = new CMonSub(_pDevice);

	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CMonSub", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CMonSub::Setting_Dir()
{
	_vec3 vPos = m_pTransformCom->Get_Desc().vPosition;

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(SCENE_STAGE0, L"Layer_Player", L"Com_Transform0");

	if (nullptr == pPlayerTransform)
		return E_FAIL;

	_vec3 vPlayerPos = pPlayerTransform->Get_Desc().vPosition;

	m_vDir = vPlayerPos - vPos;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_bFallDown = true;

	return S_OK;
}
