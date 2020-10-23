#include "stdafx.h"
#include "..\Headers\Golem.h"

USING(Client)

CGolem::CGolem(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{

}

CGolem::CGolem(const CGolem& _rOther)
	: CGameObject(_rOther)
{
}

HRESULT CGolem::Setup_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CGolem::Setup_GameObject(void * _pArg)
{
	if (_pArg)
		m_vStartPos = *(_vec3*)_pArg;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CGolem::Update_GameObject(_float _fDeltaTime)
{
	if (m_bDead)
		return GAMEOBJECT::DEAD;

	if (FAILED(Movement(_fDeltaTime)))
		return GAMEOBJECT::WARN;

	m_pTransformCom[GOLEM_BASE]->Update_Transform();

	Create_Bomb(_fDeltaTime);
	Create_MonSub(_fDeltaTime);

	if (FAILED(Setting_Center()))
		return GAMEOBJECT::WARN;

	if (FAILED(Setting_Part()))
		return GAMEOBJECT::WARN;


	return GAMEOBJECT::NOEVENT;
}

_int CGolem::LateUpdate_GameObject(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return GAMEOBJECT::ERR;

	if (FAILED(pManagement->Add_RendererList(CRenderer::RENDER_NONEALPHA, this)))
		return GAMEOBJECT::WARN;

	return GAMEOBJECT::NOEVENT;
}

HRESULT CGolem::Render_NoneAlpha()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CCamera* pCamera = (CCamera*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	for (int iCnt = GOLEM_BODY; iCnt < GOLEM_END; ++iCnt)
	{
		int iTemp = iCnt - 2;

		if (FAILED(m_pVIBufferCom[iCnt]->Set_Transform(&m_pTransformCom[iCnt]->Get_Desc().matWorld, pCamera)))
			return E_FAIL;

		if (FAILED(m_pTextureCom[iCnt]->SetTexture(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom[iCnt]->Render_VIBuffer()))
			return E_FAIL;
	}

	return S_OK;
}

CGameObject* CGolem::Clone_GameObject(void * _pArg)
{
	CGolem* pInstance = new CGolem(*this);

	if (FAILED(pInstance->Setup_GameObject(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone Golem", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGolem::Free()
{
	for (_uint iCnt = GOLEM_BASE; iCnt < GOLEM_END; ++iCnt)
	{
		Safe_Release(m_pTransformCom[iCnt]);
		Safe_Release(m_pVIBufferCom[iCnt]);
		Safe_Release(m_pTextureCom[iCnt]);
	}

	CGameObject::Free();
}

CGolem * CGolem::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CGolem* pInstance = new CGolem(_pDevice);

	if (FAILED(pInstance->Setup_GameObject_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CGolem", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

//HRESULT CGolem::Late_Setup()
//{
//	CManagement* pManagement = CManagement::Get_Instance();
//	if (nullptr == pManagement)
//		return 0;
//
//	if (FAILED(Setup_Layer_MonSub(L"Layer_MonSub")))
//		return E_FAIL;
//
//	for (_uint iCnt = 0; iCnt < 10; ++iCnt)
//	{
//		CMonSub* pMonsterSub = (CMonSub*)pManagement->Get_GameObject(SCENE_STAGE0, L"Layer_MonSub");
//		m_vecSub.emplace_back(pMonsterSub);
//	}
//
//	return S_OK;
//}

HRESULT CGolem::Attack(_float _fDeltaTime)
{
	if (m_bAttackStart)
	{
		m_bAttack = true;
	}

	//	if (Attack)
	//{
	//		m_iRandAttack = rand() % 3 + 1;
	//		m_fAttackTime += _fDeltaTime;
	//	}

	return S_OK;
}



HRESULT CGolem::Add_Component()
{
	TCHAR szName[MAX_PATH] = L"";
	TCHAR szPartName[MAX_PATH] = L"";

	// For.Com_Texture
	for (int iCnt = GOLEM_BASE; iCnt < GOLEM_END; ++iCnt)
	{
		StringCchPrintf(szName, sizeof(TCHAR) * MAX_PATH, L"Com_VIBuffer%d", iCnt);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_CubeTexture", szName, (CComponent**)&m_pVIBufferCom[iCnt]))) //생성 갯수
			return E_FAIL;

		//경우마다 그거에 맞게 복사해서 최종적으로 문자열 들어가게만들기
		if (iCnt == GOLEM_BASE)
			StringCchPrintf(szPartName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_YetiBody");
		else if (iCnt == GOLEM_CENTER)
			StringCchPrintf(szPartName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_YetiBody");
		else if (iCnt == GOLEM_BODY)
			StringCchPrintf(szPartName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_YetiBody");
		else if (iCnt == GOLEM_HEAD)
			StringCchPrintf(szPartName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_YetiHead");
		else if (iCnt == GOLEM_LEFT_ARM)
			StringCchPrintf(szPartName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_YetiLeft");
		else if (iCnt == GOLEM_RIGHT_ARM)
			StringCchPrintf(szPartName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_YetiRight");
		else if (iCnt == GOLEM_LEFT_LEG)
			StringCchPrintf(szPartName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_YetiLeftLeg");
		else if (iCnt == GOLEM_RIGHT_LEG)
			StringCchPrintf(szPartName, sizeof(TCHAR) * MAX_PATH, L"Component_Texture_YetiRightLeg");


		StringCchPrintf(szName, sizeof(TCHAR) * MAX_PATH, L"Com_Texture%d", iCnt);

		if (FAILED(CGameObject::Add_Component(SCENE_STAGE0, szPartName, szName, (CComponent**)&m_pTextureCom[iCnt]))) ////생성 갯수
			return E_FAIL;
	}

	for (_uint iCnt = GOLEM_BASE; iCnt < GOLEM_END; ++iCnt)
	{
		CTransform::TRANSFORM_DESC tTransformDesc[GOLEM_END];

		if (iCnt == GOLEM_BASE)
		{
			tTransformDesc[GOLEM_BASE].vPosition = { m_vStartPos.x , 0.f, m_vStartPos.z };
			tTransformDesc[GOLEM_BASE].fSpeedPerSecond = 10.f;
			tTransformDesc[GOLEM_BASE].fRotatePerSecond = D3DXToRadian(90.f);
		}
		else if (iCnt == GOLEM_CENTER)
		{
			tTransformDesc[GOLEM_CENTER].vPosition = { 0.f , 1.f, 0.f };
			tTransformDesc[GOLEM_CENTER].fSpeedPerSecond = 10.f;
			tTransformDesc[GOLEM_CENTER].fRotatePerSecond = D3DXToRadian(90.f);
		}
		else if (iCnt == GOLEM_BODY)
		{
			tTransformDesc[GOLEM_BODY].vPosition = { 0.f , 0.f, 0.f };
			tTransformDesc[GOLEM_BODY].fSpeedPerSecond = 10.f;
			tTransformDesc[GOLEM_BODY].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[GOLEM_BODY].vScale = { 3.0f , 1.7f , 1.f };
		}
		else if (iCnt == GOLEM_HEAD)
		{
			tTransformDesc[GOLEM_HEAD].vPosition = { 0.f , 0.f , -0.6f };
			tTransformDesc[GOLEM_HEAD].fSpeedPerSecond = 10.f;
			tTransformDesc[GOLEM_HEAD].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[GOLEM_HEAD].vScale = { 1.f , 1.f , 0.5f };
		}
		else if (iCnt == GOLEM_LEFT_ARM)
		{
			tTransformDesc[GOLEM_LEFT_ARM].vPosition = { -1.6f , 0.1f , -0.2f };
			tTransformDesc[GOLEM_LEFT_ARM].fSpeedPerSecond = 10.f;
			tTransformDesc[GOLEM_LEFT_ARM].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[GOLEM_LEFT_ARM].vScale = { 0.3f , 1.4f , 0.7f };
		}
		else if (iCnt == GOLEM_RIGHT_ARM)
		{
			tTransformDesc[GOLEM_RIGHT_ARM].vPosition = { 1.6f , 0.1f , -0.2f };
			tTransformDesc[GOLEM_RIGHT_ARM].fSpeedPerSecond = 10.f;
			tTransformDesc[GOLEM_RIGHT_ARM].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[GOLEM_RIGHT_ARM].vScale = { 0.3f , 1.4f , 0.7f };
		}
		else if (iCnt == GOLEM_LEFT_LEG)
		{
			tTransformDesc[GOLEM_LEFT_LEG].vPosition = { -0.5f , -1.f , 0.f };
			tTransformDesc[GOLEM_LEFT_LEG].fSpeedPerSecond = 10.f;
			tTransformDesc[GOLEM_LEFT_LEG].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[GOLEM_LEFT_LEG].vScale = { 0.3f , 0.7f , 0.3f };
		}
		else if (iCnt == GOLEM_RIGHT_LEG)
		{
			tTransformDesc[GOLEM_RIGHT_LEG].vPosition = { 0.5f , -1.f , 0.f };
			tTransformDesc[GOLEM_RIGHT_LEG].fSpeedPerSecond = 10.f;
			tTransformDesc[GOLEM_RIGHT_LEG].fRotatePerSecond = D3DXToRadian(90.f);
			tTransformDesc[GOLEM_RIGHT_LEG].vScale = { 0.3f , 0.7f , 0.3f };
		}

		StringCchPrintf(szName, sizeof(TCHAR) * MAX_PATH, L"Com_Transform%d", iCnt);

		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", szName, (CComponent**)&m_pTransformCom[iCnt], &tTransformDesc[iCnt]))) ////생성 갯수
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CGolem::Movement(float _fDeltaTime)
{
	if (FAILED(IsOnTerrain()))
		return E_FAIL;

	//--------------------------------------
	// 골렘테스트용
	//--------------------------------------
	if (GetAsyncKeyState('I') & 0x8000)
	{
		m_pTransformCom[GOLEM_BASE]->Move_Vertical(_fDeltaTime);
		m_pTransformCom[GOLEM_LEFT_LEG]->Turn(CTransform::AXIS_X, -_fDeltaTime + -_fDeltaTime);
		m_pTransformCom[GOLEM_RIGHT_LEG]->Turn(CTransform::AXIS_X, _fDeltaTime + _fDeltaTime);
	}

	if (GetAsyncKeyState('O') & 0x8000)
	{
		m_pTransformCom[GOLEM_BASE]->Move_Vertical(-_fDeltaTime);
		m_pTransformCom[GOLEM_LEFT_LEG]->Turn(CTransform::AXIS_X, _fDeltaTime + _fDeltaTime);
		m_pTransformCom[GOLEM_RIGHT_LEG]->Turn(CTransform::AXIS_X, -_fDeltaTime + -_fDeltaTime);
	}

	if (GetAsyncKeyState('V') & 0x8000)
	{
		m_pTransformCom[GOLEM_LEFT_ARM]->Turn(CTransform::AXIS_X, -_fDeltaTime + -_fDeltaTime);
		m_pTransformCom[GOLEM_RIGHT_ARM]->Turn(CTransform::AXIS_X, -_fDeltaTime + -_fDeltaTime);
	}

	if (GetAsyncKeyState('B') & 0x8000)
	{
		m_pTransformCom[GOLEM_LEFT_ARM]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
		m_pTransformCom[GOLEM_RIGHT_ARM]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
		//m_pTransformCom[GOLEM_LEFT_ARM]->Turn(CTransform::AXIS_X, 0);
		//m_pTransformCom[GOLEM_RIGHT_ARM]->Turn(CTransform::AXIS_X, 0);
	}

	if (GetAsyncKeyState('N') & 0x8000)
	{
		m_pTransformCom[GOLEM_BODY]->Turn(CTransform::AXIS_X, -_fDeltaTime);
	}

	if (GetAsyncKeyState('M') & 0x8000)
	{
		m_pTransformCom[GOLEM_BODY]->Set_Rotation(_vec3(0.f, 0.f, 0.f));
	}

	if (GetAsyncKeyState(VK_NUMPAD3) & 0x8000)
	{
		m_bCreate_Bomb = true;
	}

	//--------------------------------------


	return S_OK;
}

HRESULT CGolem::IsOnTerrain()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CVIBuffer_TerrainTexture* pTerrainBuffer = (CVIBuffer_TerrainTexture*)pManagement->Get_Component(SCENE_STAGE0, L"Layer_Terrain", L"Com_VIBuffer");
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	D3DXVECTOR3 vLeftLegPos = m_pTransformCom[GOLEM_BASE]->Get_Desc().vPosition;

	if (true == pTerrainBuffer->IsOnTerrain(&vLeftLegPos))
	{
		m_pTransformCom[GOLEM_BASE]->Set_Position(vLeftLegPos);
	}

	return S_OK;
}

HRESULT CGolem::Setting_Part()
{
	for (_uint iCnt = GOLEM_BODY; iCnt < GOLEM_END; ++iCnt)
	{
		m_pTransformCom[iCnt]->Update_Transform();
		m_pTransformCom[iCnt]->Set_WorldMatrix(m_pTransformCom[iCnt]->Get_Desc().matWorld * m_pTransformCom[GOLEM_CENTER]->Get_Desc().matWorld);
	}

	return S_OK;
}

HRESULT CGolem::Setting_Center()
{
	m_pTransformCom[GOLEM_CENTER]->Update_Transform();
	m_pTransformCom[GOLEM_CENTER]->Set_WorldMatrix(m_pTransformCom[GOLEM_CENTER]->Get_Desc().matWorld * m_pTransformCom[GOLEM_BASE]->Get_Desc().matWorld);

	return S_OK;
}

HRESULT CGolem::Create_Bomb(_float _fDeltaTime)
{

	for (_uint iCnt = 0; iCnt < 5; ++iCnt)
	{
		_float fRandX = _float(rand() % 20 - 10);
		_float fRandZ = _float(rand() % 20 - 10);
		m_vRand[iCnt] = _vec3(fRandX, 0.f, fRandZ);
	}

	if (m_bCreate_Bomb)
	{
		m_fAttackTime += _fDeltaTime;

		if (m_fAttackTime > 1.f)
		{
			//최소 최대 구해서 넣기

			/*	if (fRandX == 0.f && fRandZ == 0.f)
			{
			fRandX = 1.f;
			fRandZ = -1.f;
			}*/

			_vec3 vPos = m_pTransformCom[GOLEM_BASE]->Get_Desc().vPosition;

			m_vTemp = { vPos.x + m_vRand[m_iCreateCnt].x , vPos.y , vPos.z + m_vRand[m_iCreateCnt].z };

			if (FAILED(Setup_LayerBomb(L"Layer_Bomb", m_vTemp)))
				return E_FAIL;

			m_iCreateCnt += 1;
			m_fAttackTime = 0.f;
		}
	}

	if (m_iCreateCnt >= 5)
	{
		m_bCreate_Bomb = false;
		m_iCreateCnt = 0;
	}

	return S_OK;
}

HRESULT CGolem::Create_MonSub(_float _fDeltaTime)
{
	return S_OK;
}

HRESULT CGolem::Setup_LayerBomb(const wstring& LayerTag, _vec3 _vPos)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE0, L"GameObject_Bomb",
		SCENE_STAGE0, LayerTag, &_vPos)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGolem::Setup_Layer_MonSub(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE0, L"GameObject_MonSub", SCENE_STAGE0, LayerTag, &_vec3(5.f, 1.f, 30.f))))
		return E_FAIL;

	return S_OK;
}
