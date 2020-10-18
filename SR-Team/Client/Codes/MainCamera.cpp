#include "stdafx.h"
#include "..\Headers\MainCamera.h"

USING(Client)

CMainCamera::CMainCamera(LPDIRECT3DDEVICE9 _pDevice)
	: CCamera(_pDevice)
{
}

CMainCamera::CMainCamera(const CMainCamera & _rOther)
	: CCamera(_rOther)
	//, m_fDistanceToTarget(_rOther.m_fDistanceToTarget)
	//, m_fZoomInOutSpeedPerSecond(_rOther.m_fZoomInOutSpeedPerSecond)
	//, m_fCameraAngle(_rOther.m_fCameraAngle)
	//, m_fUpDownSpeedPerSecond(_rOther.m_fUpDownSpeedPerSecond)
{
}

HRESULT CMainCamera::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CMainCamera::Setup_GameObject(void * _pArg)
{
	if (nullptr == _pArg)
		return E_FAIL;

	memcpy_s(&m_tCameraDesc, sizeof(CCamera::CAMERA_DESC), _pArg, sizeof(CCamera::CAMERA_DESC));
	return S_OK;
}

_int CMainCamera::Update_GameObject(_float _fDeltaTime)
{
	if (FAILED(Movement(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	if (FAILED(ZoomInOut(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

_int CMainCamera::LateUpdate_GameObject(_float _fDeltaTime)
{
	if (CCamera::LateUpdate_GameObject(_fDeltaTime))
		return GAMEOBJECT::ERR;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CMainCamera::Movement(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(SCENE_STAGE0, L"Layer_Player", L"Com_Transform");
	if (nullptr == pPlayerTransform)
	{
		PRINT_LOG(L"pPlayerTransform is nullptr in CMainCamera::Movement", LOG::CLIENT);
		return E_FAIL;
	}

	m_tCameraDesc.vAt = pPlayerTransform->Get_Desc().vPosition;

	_vec3 vPlayerLook;
	memcpy_s(&vPlayerLook, sizeof(_vec3), &pPlayerTransform->Get_Desc().matWorld.m[2][0], sizeof(_vec3));
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);

	vPlayerLook *= -m_fDistanceToTarget;

	_vec3 vPlayerRight;
	memcpy_s(&vPlayerRight, sizeof(_vec3), &pPlayerTransform->Get_Desc().matWorld.m[0][0], sizeof(_vec3));
	D3DXVec3Normalize(&vPlayerRight, &vPlayerRight);

	_matrix matRotAxis;
	D3DXMatrixRotationAxis(&matRotAxis, &vPlayerRight, m_fCameraAngle);
	D3DXVec3TransformNormal(&vPlayerLook, &vPlayerLook, &matRotAxis);

	m_tCameraDesc.vEye = m_tCameraDesc.vAt + vPlayerLook;

	return S_OK;
}

HRESULT CMainCamera::ZoomInOut(_float _fDeltaTime)
{
	if (GetAsyncKeyState(VK_ADD) & 0x8000)
		m_fDistanceToTarget -= m_fZoomInOutSpeedPerSecond * _fDeltaTime;
	if (GetAsyncKeyState(VK_SUBTRACT) & 0x8000)
		m_fDistanceToTarget += m_fZoomInOutSpeedPerSecond * _fDeltaTime;

	return S_OK;
}

CMainCamera * CMainCamera::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CMainCamera* pInstance = new CMainCamera(_pDevice);
	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CMainCamera", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMainCamera::Clone_GameObject(void * _pArg)
{
	CMainCamera* pInstance = new CMainCamera(*this);
	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CMainCamera", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainCamera::Free()
{
	CCamera::Free();
}
