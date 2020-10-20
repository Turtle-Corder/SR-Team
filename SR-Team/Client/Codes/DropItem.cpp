#include "stdafx.h"
#include "..\Headers\DropItem.h"

USING(Client)

CDropItem::CDropItem(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
}

CDropItem::CDropItem(const CDropItem & other)
	: CGameObject(other)
{
}

HRESULT CDropItem::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CDropItem::Setup_GameObject(void * pArg)
{
	if(pArg)
		m_vPos = *(_vec3*)pArg;

	m_iRand = rand() % 10 + 1;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

int CDropItem::Update_GameObject(float _fDeltaTime)
{
	if (m_bDead)
	{
		//-------------------------------------------------
		// 1 => 2 => 4 => Release;
		//-------------------------------------------------
		return GAMEOBJECT::DEAD;
	}

	if (FAILED(Movement(_fDeltaTime)))
		return 0;

	if (FAILED(m_pTransformCom->Update_Transform()))
		return 0;

	return 0;
}

int CDropItem::LateUpdate_GameObject(float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return 0;

	return 0;
}

HRESULT CDropItem::Render_NoneAlpha()
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

HRESULT CDropItem::Add_Component()
{
	TCHAR szName[MAX_PATH] = L"";

	// For.Com_VIBuffer
	switch (m_iRand)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		StringCchPrintf(szName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_DropDiamond");
		break;
	case 6:
	case 7:
	case 8:
	case 9:
		StringCchPrintf(szName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_DropRuby");
		break;
	case 10:
		StringCchPrintf(szName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_Dropiron_sword");

		break;
	}


	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_RectTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE0, szName, L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// For.Com_Transform
	CTransform::TRANSFORM_DESC tTransformDesc;

	tTransformDesc.vPosition = { m_vPos.x , m_vPos.y , m_vPos.z };
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDropItem::Movement(float _fDeltaTime)
{
	if (FAILED(IsOnTerrain()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDropItem::IsOnTerrain()
{
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

	return S_OK;
}

HRESULT CDropItem::Move(float _fDeltaTime)
{
	return E_NOTIMPL;
}

CDropItem * CDropItem::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CDropItem* pInstance = new CDropItem(pDevice);

	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CTranslucentCube", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDropItem::Clone_GameObject(void * pArg)
{
	CDropItem* pInstance = new CDropItem(*this);

	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CTranslucentCube", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDropItem::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}
