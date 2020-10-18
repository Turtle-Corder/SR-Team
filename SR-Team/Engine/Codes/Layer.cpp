#include "GameObject.h"
#include "..\Headers\Layer.h"

USING(Engine)

CLayer::CLayer()
{
}

CGameObject * CLayer::Get_GameObject(_uint _iIndex)
{
	if (0 > _iIndex || m_GameObjects.size() <= _iIndex)
		return nullptr;

	GAMEOBJECTS::iterator iter_begin = m_GameObjects.begin();

	for (_uint iCnt = 0; iCnt < _iIndex; ++iCnt)
		++iter_begin;

	return (*iter_begin);
}

CComponent * CLayer::Get_Component(const wstring & _strComponentTag, _uint _iIndex)
{
	if (0 > _iIndex || m_GameObjects.size() <= _iIndex)
		return nullptr;

	GAMEOBJECTS::iterator iter_begin = m_GameObjects.begin();

	for (_uint i = 0; i < _iIndex; ++i)
		++iter_begin;

	return (*iter_begin)->Get_Component(_strComponentTag);
}

HRESULT CLayer::Add_GameObject_InLayer(CGameObject * _pObject)
{
	if (nullptr == _pObject)
		return E_FAIL;

	auto iter_find = find(m_GameObjects.begin(), m_GameObjects.end(), _pObject);
	if (m_GameObjects.end() != iter_find)
		return E_FAIL;

	m_GameObjects.emplace_back(_pObject);

	return S_OK;
}

_int CLayer::Update_Layer(_float _fDeltaTime)
{
	_int iBehaviour = 0;

	auto iter = m_GameObjects.begin();
	auto iter_end = m_GameObjects.end();

	for (;iter != iter_end;)
	{
		_int iResult = (*iter)->Update_GameObject(_fDeltaTime);
		switch (iResult)
		{
		case GAMEOBJECT::NOEVENT:
			++iter;
			break;

		case GAMEOBJECT::DEAD:
			if (!Safe_Release(*iter))
				iter = m_GameObjects.erase(iter);
			break;

		// 老窜 default肺 贸府..

		//case GAMEOBJECT::WARN:
		//	break;

		//case GAMEOBJECT::ERR:
		//	break;

		default:
		{
			// UNDONE : 老窜 crash
			iBehaviour = -1;
			int* ptr = nullptr;
			*ptr = 0;
		}
			break;
		}
	}

	return iBehaviour;
}

_int CLayer::LateUpdate_Layer(_float _fDeltaTime)
{
	_int iBehaviour = 0;

	for (auto& pObject : m_GameObjects)
	{
		if (iBehaviour = pObject->LateUpdate_GameObject(_fDeltaTime))
			break;
	}

	return iBehaviour;
}

CLayer * CLayer::Create()
{
	return new CLayer();
}

void CLayer::Free()
{
	for (auto& pObject : m_GameObjects)
		Safe_Release(pObject);

	m_GameObjects.clear();
}