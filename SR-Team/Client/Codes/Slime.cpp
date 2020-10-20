#include "stdafx.h"
#include "KeyManager.h"
#include "..\Headers\Slime.h"

USING(Client)

CSlime::CSlime(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
}

CSlime::CSlime(const CSlime & other)
	: CGameObject(other)
{
	m_bIsClone = true;
}

HRESULT CSlime::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CSlime::Setup_GameObject(void * pArg)
{
	m_bDead = false;

	if (pArg == nullptr)
		m_iCurCount = 1;
	else
		m_iCurCount = *(int*)pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CSlime::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
	{
		//-------------------------------------------------
		// 1 => 2 => 4 => Release;
		//-------------------------------------------------
		if (m_bIsClone && m_iCurCount < m_iMaxCount)
		{
			m_iCurCount *= 2;

			for (int i = 0; i < m_iCurCount; ++i)
			{
				if (FAILED(Divide_Cube(L"Layer_Translucent_Cube")))
					return GAMEOBJECT::WARN;
			}
		}
		else
		{
			if (FAILED(Create_Item(L"Layer_DropItem")))
				return GAMEOBJECT::WARN;
		}

		return GAMEOBJECT::DEAD;
	}

	if (CKeyManager::Get_Instance()->Key_Down(VK_F4))
		m_bDead = true;

	if (FAILED(Movement(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	if (FAILED(LookAtPlayer(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	if (FAILED(m_pTransformCom->Update_Transform()))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::DEAD;
}

_int CSlime::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_BLNEDALPHA, this)))
		return 0;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CSlime::Render_BlendAlpha()
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

HRESULT CSlime::Add_Component()
{
	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE0, L"Component_Texture_Translucent_Cube", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// For.Com_Transform
	CTransform::TRANSFORM_DESC tTransformDesc;

	float x = float(rand() % 20);
	float z = float(rand() % 20);

	tTransformDesc.vPosition = { x , 1.f , z };
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);
	tTransformDesc.vScale = { 1.4f / (float)m_iCurCount , 1.4f , 1.4f / (float)m_iCurCount };

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSlime::Movement(float _fDeltaTime)
{
	if (FAILED(IsOnTerrain()))
		return E_FAIL;

	Jumping(_fDeltaTime);

	return S_OK;
}

HRESULT CSlime::IsOnTerrain()
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
		m_bJump = true;
	}

	return S_OK;
}

void CSlime::Jumping(float _fDeltaTime)
{
	D3DXVECTOR3 vPos = m_pTransformCom->Get_Desc().vPosition;

	if (m_bJump)
	{
		vPos.y += m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime * 0.5f;
		m_fJumpTime += _fDeltaTime;

		if (vPos.y < m_pTransformCom->Get_Desc().vPosition.y)
		{
			vPos.y = m_pTransformCom->Get_Desc().vPosition.y;
			m_bJump = false;
			m_fJumpTime = 0.f;
		}

		m_pTransformCom->Set_Position(vPos);
	}
}

HRESULT CSlime::LookAtPlayer(float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(SCENE_STAGE0, L"Layer_Player", L"Com_Transform0");

	if (nullptr == pPlayerTransform)
		return E_FAIL;
	//--------------------------------------------------
	// 플레이어와 this => Pos
	//--------------------------------------------------
	_vec3 vPlayerPos = pPlayerTransform->Get_Desc().vPosition;
	_vec3 vMonPos = m_pTransformCom->Get_Desc().vPosition;
	_vec3 vMonLook = {};

	////--------------------------------------------------
	//// Look 과 목적지 - 출발지를 내적
	////--------------------------------------------------

	memcpy_s(&vMonLook, sizeof(_vec3), &m_pTransformCom->Get_Desc().matWorld._31, sizeof(_vec3));

	_vec3 vMonToPlayer = vPlayerPos - vMonPos;

	D3DXVec3Normalize(&vMonLook, &vMonLook);
	D3DXVec3Normalize(&vMonToPlayer, &vMonToPlayer);

	float fDot = 0.f;
	float fRad = 0.f;

	fDot = D3DXVec3Dot(&vMonLook, &vMonToPlayer);
	fRad = (float)acos(fDot);

	_vec3 vMonRight = {};
	D3DXVec3Cross(&vMonRight, &vMonLook, &_vec3(0.f, 1.f, 0.f));

	D3DXVec3Dot(&vMonRight, &vMonToPlayer);

	float fLimit = D3DXVec3Dot(&vMonRight, &vMonToPlayer);


	if (fabsf(fLimit) < 0.2f)
		return S_OK;


	CTransform* pMonsterTransform = (CTransform*)pManagement->Get_Component(SCENE_STAGE0, L"Layer_Monster", L"Com_Transform");

	if (nullptr == pMonsterTransform)
		return E_FAIL;

	if (fLimit > 0)
	{
		m_pTransformCom->Turn(CTransform::AXIS_Y, -_fDeltaTime * fRad);
		pMonsterTransform->Turn(CTransform::AXIS_Y, -_fDeltaTime * fRad);
	}
	else
	{
		m_pTransformCom->Turn(CTransform::AXIS_Y, _fDeltaTime * fRad);
		pMonsterTransform->Turn(CTransform::AXIS_Y, _fDeltaTime * fRad);
	}
	//------------------------------------------
	// 공전 구현 순서생각하기 LateUpdate안에 월드구성하고 나서
	//------------------------------------------
	/*	D3DXMATRIX matTrans , matRevY , matParent;
	D3DXMatrixTranslation(&matTrans, 2.f, 0.f, 2.f);
	D3DXMatrixRotationY(&matRevY, D3DXToRadian(m_fAngle));
	D3DXMatrixTranslation(&matParent, vMonPos.x, vMonPos.y, vMonPos.z);
	m_pTransformCom->Set_WorldMatrix(matTrans * matRevY * matParent);*/
	//------------------------------------------
	return S_OK;
}

HRESULT CSlime::Divide_Cube(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE0, L"GameObject_Translucent_Cube", SCENE_STAGE0, LayerTag, &m_iCurCount)))
		return E_FAIL;

	return S_OK;
}

CSlime * CSlime::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CSlime* pInstance = new CSlime(pDevice);

	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CTranslucentCube", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSlime::Clone_GameObject(void * pArg)
{
	CSlime* pInstance = new CSlime(*this);

	if (FAILED(pInstance->Setup_GameObject(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CTranslucentCube", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSlime::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}

HRESULT CSlime::Create_Item(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	_vec3 vPos = m_pTransformCom->Get_Desc().vPosition;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE0, L"GameObject_DropItem", SCENE_STAGE0, LayerTag, &vPos)))
		return E_FAIL;

	return S_OK;
}
