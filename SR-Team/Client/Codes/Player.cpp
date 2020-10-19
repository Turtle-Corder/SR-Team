#include "stdafx.h"
#include "..\Headers\Player.h"

USING(Client)

CPlayer::CPlayer(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CPlayer::CPlayer(const CPlayer & _rOther)
	:CGameObject(_rOther)
{
}

HRESULT CPlayer::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Setup_GameObject(void * _pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CPlayer::Update_GameObject(_float _fDeltaTime)
{
	if (FAILED(Movement(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	if (FAILED(m_pTransformCom->Update_Transform()))
		return GAMEOBJECT::WARN;

	if (FAILED(m_pColliderCom->Update_Collider(m_pTransformCom->Get_Desc().vPosition)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

_int CPlayer::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagemnet = CManagement::Get_Instance();
	if (nullptr == pManagemnet)
		return GAMEOBJECT::ERR;


	if (FAILED(pManagemnet->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CPlayer::Render_NoneAlpha()
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

HRESULT CPlayer::Take_Damage()
{
	return S_OK;
}

HRESULT CPlayer::Add_Component()
{
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_RectTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Player", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.vPosition = { 5.f, 0.f, 5.f };
	tTransformDesc.fSpeedPerSecond = 10.f;
	tTransformDesc.fRotatePerSecond = D3DXToRadian(90.f);

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Raycast", L"Com_Ray", (CComponent**)&m_pRaycastCom)))
		return E_FAIL;

	CCollider::COLLIDER_DESC tColDesc;
	tColDesc.vPosition = tTransformDesc.vPosition;
	tColDesc.fRadius = 0.7f;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider", L"Com_Collider", (CComponent**)&m_pColliderCom, &tColDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Movement(_float _fDeltaTime)
{
	Move_Vertical(_fDeltaTime);
	Move_Horizontal(_fDeltaTime);
	Turn(_fDeltaTime);
	Move_Target(_fDeltaTime);

	if (FAILED(IsOnTerrain()))
		return E_FAIL;

	if (FAILED(RaycastOnTerrain()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::IsOnTerrain()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CVIBuffer_TerrainTexture* pTerrainBuffer = (CVIBuffer_TerrainTexture*)pManagement->Get_Component(SCENE_STAGE0, L"Layer_Terrain", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	_vec3 vPosition = m_pTransformCom->Get_Desc().vPosition;
	if (true == pTerrainBuffer->IsOnTerrain(&vPosition))
		m_pTransformCom->Set_Position(vPosition);

	return S_OK;

}

HRESULT CPlayer::RaycastOnTerrain()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CVIBuffer_TerrainTexture* pTerrainBuffer = (CVIBuffer_TerrainTexture*)pManagement->Get_Component(SCENE_STAGE0, L"Layer_Terrain", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;


	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		_matrix mat;
		D3DXMatrixIdentity(&mat);

		if (true == m_pRaycastCom->IsSimulate<VTX_TEXTURE, INDEX16>(
			g_hWnd, WINCX, WINCY, pTerrainBuffer, &mat, pCamera, &m_vTargetPos))
			m_bIsTagetMove = true;
	}

	return S_OK;
}

void CPlayer::Move_Vertical(_float _fDeltaTime)
{
	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_bIsTagetMove = false;
		m_pTransformCom->Move_Vertical(_fDeltaTime);
	}

	else if (GetAsyncKeyState('S') & 0x8000)
	{
		m_bIsTagetMove = false;
		m_pTransformCom->Move_Vertical(-_fDeltaTime);
	}
}

void CPlayer::Move_Horizontal(_float _fDeltaTime)
{
	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_bIsTagetMove = false;
		m_pTransformCom->Move_Horizontal(-_fDeltaTime);
	}
	else if (GetAsyncKeyState('D') & 0x8000)
	{
		m_bIsTagetMove = false;
		m_pTransformCom->Move_Horizontal(_fDeltaTime);
	}
}

void CPlayer::Turn(_float _fDeltaTime)
{
	if (GetAsyncKeyState('Q') & 0x8000)
		m_pTransformCom->Turn(CTransform::AXIS_Y, -_fDeltaTime);

	else if (GetAsyncKeyState('E') & 0x8000)
		m_pTransformCom->Turn(CTransform::AXIS_Y, _fDeltaTime);
}

void CPlayer::Move_Target(_float _fDeltaTime)
{
	if (false == m_bIsTagetMove)
		return;

	_vec3 vCurPos = m_pTransformCom->Get_Desc().vPosition;
	_vec3 vMoveDir = m_vTargetPos - vCurPos;

	_float fSpeedPerSecond = m_pTransformCom->Get_Desc().fSpeedPerSecond;
	_float fDist = D3DXVec3Length(&vMoveDir);

	if (0.6f >= fDist)
	{
		m_bIsTagetMove = false;
		return;
	}

	D3DXVec3Normalize(&vMoveDir, &vMoveDir);

	vCurPos += vMoveDir * fSpeedPerSecond * _fDeltaTime;
	m_pTransformCom->Set_Position(vCurPos);
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CPlayer* pInstance = new CPlayer(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create Player", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayer::Clone_GameObject(void * _pArg)
{
	CPlayer* pInstance = new CPlayer(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone Player", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRaycastCom);
	Safe_Release(m_pColliderCom);

	CGameObject::Free();
}
