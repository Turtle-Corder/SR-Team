#include "GameObject.h"
#include "Layer.h"
#include "..\Headers\Object_Manager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{
}

CGameObject * CObject_Manager::Get_GameObject(_int _iSceneID, const wstring & _strLayerTag, _uint _iIndex)
{
	if (0 > _iSceneID || m_iSceneCount <= _iSceneID)
		return nullptr;

	auto iter_find = m_pLayers[_iSceneID].find(_strLayerTag);
	if (m_pLayers[_iSceneID].end() == iter_find)
		return nullptr;

	return iter_find->second->Get_GameObject(_iIndex);
}

CComponent * CObject_Manager::Get_Component(_int _iSceneID, const wstring & _strLayerTag, const wstring & _strComponentTag, _uint _iIndex)
{
	if (0 > _iSceneID || m_iSceneCount <= _iSceneID)
		return nullptr;

	auto iter_find = m_pLayers[_iSceneID].find(_strLayerTag);
	if (m_pLayers[_iSceneID].end() == iter_find)
		return nullptr;

	return iter_find->second->Get_Component(_strComponentTag, _iIndex);
}

HRESULT CObject_Manager::Setup_Object_Manager(_int _iSceneCount)
{
	if (1 > _iSceneCount)
		return E_FAIL;

	m_pGameObjects = new GAMEOBJECTS[_iSceneCount];
	m_pLayers = new LAYERS[_iSceneCount];

	m_iSceneCount = _iSceneCount;

	return S_OK;
}

HRESULT CObject_Manager::Add_GameObject_Prototype(_int _iSceneID, const wstring & _strProtoTypeTag, CGameObject * _pProtoType)
{
	if (0 > _iSceneID || m_iSceneCount <= _iSceneID)
		return E_FAIL;

	if (nullptr == m_pGameObjects || nullptr == _pProtoType)
		return E_FAIL;

	auto iter_find = m_pGameObjects[_iSceneID].find(_strProtoTypeTag);
	if (m_pGameObjects[_iSceneID].end() != iter_find)
	{
		Safe_Release(_pProtoType);
		return E_FAIL;
	}

	m_pGameObjects[_iSceneID].emplace(_strProtoTypeTag, _pProtoType);

	return S_OK;
}

HRESULT CObject_Manager::Add_GameObject_InLayer(_int _iFromSceneID, const wstring & _strProtoTypeTag, _int _iToSceneID, const wstring & _strLayerTag, void * _pArg)
{
	if (0 > _iFromSceneID || 0 > _iToSceneID ||
		m_iSceneCount <= _iFromSceneID || m_iSceneCount <= _iToSceneID)
		return E_FAIL;

	if (nullptr == m_pGameObjects || nullptr == m_pLayers)
		return E_FAIL;

	auto iter_find = m_pGameObjects[_iFromSceneID].find(_strProtoTypeTag);
	if (m_pGameObjects[_iFromSceneID].end() == iter_find)
	{
		PRINT_LOG(_strProtoTypeTag.c_str(), LOG::ENGINE);
		PRINT_LOG(L"Prototype Not Found", LOG::ENGINE);
		return E_FAIL;
	}

	CGameObject* pClone = iter_find->second->Clone_GameObject(_pArg);
	if (nullptr == pClone)
		return E_FAIL;

	auto iter_find_Layer = m_pLayers[_iToSceneID].find(_strLayerTag);
	if (m_pLayers[_iToSceneID].end() == iter_find_Layer)
	{
		CLayer* pLayer = CLayer::Create();
		m_pLayers[_iToSceneID].emplace(_strLayerTag, pLayer);
	}

	if (FAILED(m_pLayers[_iToSceneID][_strLayerTag]->Add_GameObject_InLayer(pClone)))
	{
		Safe_Release(pClone);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CObject_Manager::Clear_ForScene(_int _iSceneID)
{
	if (0 > _iSceneID || m_iSceneCount <= _iSceneID)
		return E_FAIL;

	if (nullptr == m_pGameObjects || nullptr == m_pLayers)
		return E_FAIL;

	for (auto& Pair : m_pGameObjects[_iSceneID])
		Safe_Release(Pair.second);

	for (auto& Pair : m_pLayers[_iSceneID])
		Safe_Release(Pair.second);

	m_pGameObjects[_iSceneID].clear();
	m_pLayers[_iSceneID].clear();

	return S_OK;
}

_int CObject_Manager::Update_Object_Manger(_float _fDeltaTime)
{
	_int iBehaviour = 0;

	for (_int iCnt = 0; iCnt < m_iSceneCount; ++iCnt)
	{
		for (auto& rPair : m_pLayers[iCnt])
		{
			// UNDONE : Behaviour convert enum
			if (iBehaviour = rPair.second->Update_Layer(_fDeltaTime))
				goto EXIT_UPDATE;
		}
	}
EXIT_UPDATE:
	return iBehaviour;
}

_int CObject_Manager::LateUpdate_Object_Manager(_float _fDeltaTime)
{
	_int iBehavior = 0;

	for (_int i = 0; i < m_iSceneCount; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			if (iBehavior = Pair.second->LateUpdate_Layer(_fDeltaTime))
				goto EXIT_LATEUPDATE;
		}
	}

EXIT_LATEUPDATE:
	return iBehavior;
}

void CObject_Manager::Free()
{
	for (_int iCnt = 0; iCnt < m_iSceneCount; ++iCnt)
	{
		for (auto& rPair : m_pGameObjects[iCnt])
			Safe_Release(rPair.second);

		for (auto& rPair : m_pLayers[iCnt])
			Safe_Release(rPair.second);

		m_pGameObjects[iCnt].clear();
		m_pLayers[iCnt].clear();
	}

	Safe_Delete_Array(m_pGameObjects);
	Safe_Delete_Array(m_pLayers);
}
