#include "stdafx.h"
#include "..\Headers\PreLoader.h"

#pragma region GameObject_Headers
#include "Terrain.h"
#include "Skybox.h"
#include "Slime.h"
#include "Snail.h"
#include "Yeti.h"
#include "Snow.h"
#include "Meteor.h"
#include "CubeTerrain.h"
#include "DropItem.h"
#include "Tree.h"
#include "Golem.h"
#include "Bomb.h"
#include "MonSub.h"
#include "Flower.h"
#include "PlaneSkill.h"
#include "EnergyBolt.h"
#include "Crack.h"
#include "TerrainBundle.h"
#include "Wand.h"
#pragma endregion

USING(Client)



CPreLoader::CPreLoader(LPDIRECT3DDEVICE9 _pDevice)
	: m_pDevice(_pDevice)
{
	Safe_AddRef(_pDevice);
}

_bool CPreLoader::IsFinished()
{
	return m_bFinished;
}

HRESULT CPreLoader::Setup_PreLoader(eSCENE_ID _eNextSceneID)
{
	m_eNextSceneID = _eNextSceneID;

	InitializeCriticalSection(&m_CriticalSection);

	m_hLoadThread = (HANDLE)_beginthreadex(0, 0, ThreadMain, this, 0, 0);
	if (nullptr == m_hLoadThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CPreLoader::Load_Resources_Stage0()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	//----------------------------------------------------------------------------------------------------
	// GameObject
	//----------------------------------------------------------------------------------------------------
#pragma region GameObject_Terrain
	if (FAILED(pManagement->Add_GameObject_Prototype(m_eNextSceneID, L"GameObject_Terrain", CTerrain::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Skybox
	if (FAILED(pManagement->Add_GameObject_Prototype(m_eNextSceneID, L"GameObject_Skybox", CSkybox::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Translucent_Cube
	if (FAILED(pManagement->Add_GameObject_Prototype(m_eNextSceneID, L"GameObject_Translucent_Cube", CSlime::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Translucent_Cube
	if (FAILED(pManagement->Add_GameObject_Prototype(m_eNextSceneID, L"GameObject_Slime", CSlime::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Snail
	if (FAILED(pManagement->Add_GameObject_Prototype(m_eNextSceneID, L"GameObject_Snail", CSnail::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Yeti
	if (FAILED(pManagement->Add_GameObject_Prototype(m_eNextSceneID, L"GameObject_Yeti", CYeti::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Item
	if (FAILED(pManagement->Add_GameObject_Prototype(m_eNextSceneID, L"GameObject_DropItem", CDropItem::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Wand
	if (FAILED(pManagement->Add_GameObject_Prototype(m_eNextSceneID, L"GameObject_Wand", CWand::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Tree
	if (FAILED(pManagement->Add_GameObject_Prototype(m_eNextSceneID, L"GameObject_Tree", CTree::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Flower
	if (FAILED(pManagement->Add_GameObject_Prototype(m_eNextSceneID, L"GameObject_Flower", CFlower::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Golem
	if (FAILED(pManagement->Add_GameObject_Prototype(m_eNextSceneID, L"GameObject_Golem", CGolem::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Bomb
	if (FAILED(pManagement->Add_GameObject_Prototype(m_eNextSceneID, L"GameObject_Bomb", CBomb::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_MonSub
	if (FAILED(pManagement->Add_GameObject_Prototype(m_eNextSceneID, L"GameObject_MonSub", CMonSub::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_PlanSkill
	if (FAILED(pManagement->Add_GameObject_Prototype(m_eNextSceneID, L"GameObject_PlanSkill", CPlaneSkill::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_EnergyBolt
	if (FAILED(pManagement->Add_GameObject_Prototype(m_eNextSceneID, L"GameObject_EnergyBolt", CEnergyBolt::Create(m_pDevice))))
	return E_FAIL;
#pragma endregion

#pragma region GameObject_Crack
	if (FAILED(pManagement->Add_GameObject_Prototype(m_eNextSceneID, L"GameObject_Crack", CCrack::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion
	//----------------------------------------------------------------------------------------------------
	// Component
	//----------------------------------------------------------------------------------------------------

	// terrain
#pragma region Component_VIBuffer_TerrainTexture
	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_VIBuffer_TerrainTexture", CVIBuffer_TerrainTexture::Create(m_pDevice, 129, 129))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_CubeTerrain
	if (FAILED(pManagement->Add_GameObject_Prototype(m_eNextSceneID, L"GameObject_CubeTerrain", CCubeTerrain::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Terrain
	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_Texture_Terrain", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/Terrain/Terrain%d.png"))))
		return E_FAIL;
#pragma endregion


	// skybox
#pragma region Component_Texture_Skybox
	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_Texture_Skybox", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Monster%d.dds"))))
		return E_FAIL;
#pragma endregion


	// monster
#pragma region Component_Texture_Translucent_Cube
	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_Texture_Translucent_Cube", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/TranslucentCube%d.dds", 2))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_MonSub
	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_Texture_MonSub", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/MonSub%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_SnailBody
	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_Texture_SnailBody", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/SnailBody%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_SnailHead
	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_Texture_SnailHead", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/SnailHead%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_YetiBody
	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_Texture_YetiBody", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/YetiBody%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_YetiHead
	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_Texture_YetiHead", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/YetiHead%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_YetiLeft
	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_Texture_YetiLeft", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/YetiPart%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_YetiRight
	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_Texture_YetiRight", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/YetiPart%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_YetiLeftLeg
	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_Texture_YetiLeftLeg", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/YetiPart%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_YetiRightLeg
	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_Texture_YetiRightLeg", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/YetiPart%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_SemiBossBody
	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_Texture_SemiBossBody", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/SemiBossBody%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_SemiBossHead
	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_Texture_SemiBossHead", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/SemiBossHead%d.dds"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_SemiBossPart
	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_Texture_SemiBossPart", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/SemiBossPart%d.dds"))))
		return E_FAIL;

#pragma region Component_Texture_Bomb
	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_Texture_Bomb", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Bomb/Bomb%d.dds", 2))))
		return E_FAIL;
#pragma endregion



	//projectile
#pragma region GameObject_Snow
	if (FAILED(pManagement->Add_GameObject_Prototype(m_eNextSceneID, L"GameObject_Snow", CSnow::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region GameObject_Meteor
	if (FAILED(pManagement->Add_GameObject_Prototype(m_eNextSceneID, L"GameObject_Meteor", CMeteor::Create(m_pDevice))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_SpellJin
	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_Texture_SpellJin", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/SpellJin%d.png"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_Crack
	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_Texture_Crack", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/Crack%d.png"))))
		return E_FAIL;
#pragma endregion
	// player
#pragma region Component_Texture_Player
	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_Texture_Monster", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE, L"../Resources/Monster%d.dds"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_Texture_PlayerHead", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE,
			L"../Resources/Player/PlayerHead%d.dds"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_Texture_PlayerHand", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE,
			L"../Resources/Player/PlayerHand%d.dds"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_Texture_PlayerFoot", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE,
			L"../Resources/Player/PlayerFoot%d.dds"))))
		return E_FAIL;
#pragma endregion



	// item
#pragma region Component_Texture_Item
	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_Texture_DropDiamond", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/item/diamond%d.png"))))
		return E_FAIL;
#pragma endregion
	
#pragma region Component_Texture_Ruby
	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_Texture_DropRuby", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/item/ruby%d.png"))))
		return E_FAIL;
#pragma endregion

#pragma region Component_Texture_ironsword
	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_Texture_Dropiron_sword", CTexture::Create(m_pDevice, CTexture::TEXTURE_NORMAL, L"../Resources/item/iron_sword%d.png"))))
		return E_FAIL;
#pragma endregion

	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_Texture_Meteor", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE,
		L"../Resources/Monster%d.dds"))))
		return E_FAIL;

	if (FAILED(pManagement->Add_Component_Prototype(m_eNextSceneID, L"Component_Texture_Snow", CTexture::Create(m_pDevice, CTexture::TEXTURE_CUBE,
		L"../Resources/Snow%d.dds"))))
		return E_FAIL;

	//Ÿ�� ������ �ҷ�����



	Setup_Stage_CubeTerrain(_T("Layer_CubeTerrain"));


	return S_OK;
}

HRESULT CPreLoader::Load_Resources_Stage1()
{

	

	return S_OK;
}

HRESULT CPreLoader::Load_Resources_Stage2()
{
	return E_NOTIMPL;
}

HRESULT CPreLoader::Load_Resources_Stage3()
{
	return E_NOTIMPL;
}

HRESULT CPreLoader::Load_Resources_Stage4()
{
	return E_NOTIMPL;
}

HRESULT CPreLoader::Load_Resources_Stage5()
{
	return E_NOTIMPL;
}

_uint CPreLoader::ThreadMain(void * _pParam)
{
	CPreLoader* pInstance = (CPreLoader*)_pParam;
	if (nullptr == pInstance)
		return 1;

	HRESULT hr = 0;
	
	EnterCriticalSection(&pInstance->m_CriticalSection);
	switch (pInstance->m_eNextSceneID)
	{
	case eSCENE_ID::SCENE_STATIC:
		break;

	case eSCENE_ID::SCENE_LOGO:
		break;

	case eSCENE_ID::SCENE_STAGE0:
		hr = pInstance->Load_Resources_Stage0();
		break;

	case eSCENE_ID::SCENE_STAGE1:
		hr = pInstance->Load_Resources_Stage1();
		break;

	case eSCENE_ID::SCENE_STAGE2:
		hr = pInstance->Load_Resources_Stage2();
		break;

	case eSCENE_ID::SCENE_STAGE3:
		hr = pInstance->Load_Resources_Stage3();
		break;

	case eSCENE_ID::SCENE_STAGE4:
		hr = pInstance->Load_Resources_Stage4();
		break;

	case eSCENE_ID::SCENE_STAGE5:
		hr = pInstance->Load_Resources_Stage5();
		break;

	default:
		break;
	}

	LeaveCriticalSection(&pInstance->m_CriticalSection);
	if (FAILED(hr))
		return 1;

	pInstance->m_bFinished = true;
	return 0;
}

CPreLoader * CPreLoader::Create(LPDIRECT3DDEVICE9 _pDevice, eSCENE_ID _eNextSceneID)
{
	if (nullptr == _pDevice)
		return nullptr;

	CPreLoader* pInstance = new CPreLoader(_pDevice);
	if (FAILED(pInstance->Setup_PreLoader(_eNextSceneID)))
	{
		PRINT_LOG(L"Failed To Create CPreLoader", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPreLoader::Free()
{
	DeleteCriticalSection(&m_CriticalSection);
	CloseHandle(m_hLoadThread);

	Safe_Release(m_pDevice);
}





HRESULT CPreLoader::Setup_Stage_CubeTerrain(const wstring & LayerTag)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	//�ε�����

	wifstream fin;
	wstring wstrFilePath = _T("../DataPath/MapSource/Stage2.txt");
	fin.open(wstrFilePath.c_str());
	if (!fin.fail())
	{
		//����
		TCHAR szXVerCount[MAX_PATH] = L"";
		TCHAR szZVerCount[MAX_PATH] = L"";
		TCHAR szFloor[MAX_PATH] = L"";
		TCHAR szIndex[MAX_PATH] = L"";
		TCHAR szOnOff[MAX_PATH] = L"";
		TCHAR szTextureID[MAX_PATH] = L"";
		TCHAR szMovePossible[10] = L"";

		//X, Z �హ��
		fin.getline(szXVerCount, MAX_PATH, L'|');
		fin.getline(szZVerCount, MAX_PATH);

		_int XNumber = _ttoi(szXVerCount) - 1;
		_int ZNumber = _ttoi(szZVerCount) - 1;

		TILEINFO* tTileInfo = new TILEINFO[XNumber * ZNumber];

		int iFloorMax = XNumber*ZNumber;

		if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STATIC, L"GameObject_TerrainBundle", m_eNextSceneID, _T("Layer_TerrainBundle"), &iFloorMax)))
			return E_FAIL;

		while (true)
		{


			//Cube Info �ޱ�
			fin.getline(szFloor, MAX_PATH, L'|');
			fin.getline(szIndex, MAX_PATH, L'|');
			fin.getline(szOnOff, MAX_PATH, L'|');
			fin.getline(szTextureID, MAX_PATH, L'|');
			fin.getline(szMovePossible, MAX_PATH);
			_uint iFloor = _ttoi(szFloor);
			_uint iIndex = _ttoi(szIndex);
			_bool bOnOff = 1 && (_ttoi(szOnOff));
			_uint iTextureID = _ttoi(szTextureID);

			//Index ���ϱ�
			_uint iXIndex = iIndex % XNumber;
			_uint iZIndex = iIndex / XNumber;

			//�Ѱ��� TerrainInfo
			TERRAININFO Temp_Info;
			Temp_Info.iFloor = iFloor;
			Temp_Info.iIndex = iIndex;
			Temp_Info.iTextureID = iTextureID;
			Temp_Info.iX_Index = iXIndex;
			Temp_Info.iZ_Index = iZIndex;
			Temp_Info.iInterval = 2.f;
			Temp_Info.iMaxX = XNumber;
			Temp_Info.iMaxZ = ZNumber;


			tTileInfo[iIndex].iOpt = 0;
			tTileInfo[iIndex].iX = iXIndex;
			tTileInfo[iIndex].iZ = iZIndex;



			if (fin.eof())
				break;


			((CTerrainBundle*)pManagement->Get_GameObject(m_eNextSceneID, _T("Layer_TerrainBundle")))->Set_TerrainInfo(iIndex, iFloor, Temp_Info);


			if (true == bOnOff)
			{
				if (FAILED(pManagement->Add_GameObject_InLayer(SCENE_STAGE0, L"GameObject_CubeTerrain", m_eNextSceneID, LayerTag, &Temp_Info)))
					return E_FAIL;

			}

		}

		pManagement->Set_TileInfo(tTileInfo, XNumber, ZNumber);


	}

	else
		return E_FAIL;


	fin.close();
	return S_OK;
}
