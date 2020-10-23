#include "GameObject.h"
#include "Collider.h"
#include "BoxCollider.h"
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

		// 일단 default로 처리..

		//case GAMEOBJECT::WARN:
		//	break;

		//case GAMEOBJECT::ERR:
		//	break;

		default:
		{
			// UNDONE : 일단 crash
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

HRESULT CLayer::Collision_Detection_Layers(CLayer* _pSrcLayer, const wstring& _strColliderTag, const wstring& _strDmgInfoTag)
{
	if (nullptr == _pSrcLayer)
		return E_FAIL;

	for (auto& pSrcObject : _pSrcLayer->m_GameObjects)
	{
		CCollider* pSrcCol = (CCollider*)pSrcObject->Get_Component(_strColliderTag);
		if (nullptr == pSrcCol)
			continue;

		for (auto& pDstObject : m_GameObjects)
		{
			CCollider* pDstCol = (CCollider*)pDstObject->Get_Component(_strColliderTag);
			if (nullptr == pDstCol)
				continue;

			_vec3 vDist = pSrcCol->Get_Desc().vPosition - pDstCol->Get_Desc().vPosition;
			float fDistance = D3DXVec3Length(&vDist);
			float fMaxDistance = pSrcCol->Get_Desc().fRadius + pDstCol->Get_Desc().fRadius;

			if (fDistance < fMaxDistance)
			{
				CComponent* pDmgInfoComp = pSrcObject->Get_Component(_strDmgInfoTag);
				pDstObject->Take_Damage(pDmgInfoComp);
			}
		}
	}

	return S_OK;
}


HRESULT CLayer::CollisionBox_Detection_Layers(CLayer* _pSrcLayer, const wstring& _strColliderTag, const wstring& _strDmgInfoTag)
{
	if (nullptr == _pSrcLayer)
		return E_FAIL;

	for (auto& pSrcObject : _pSrcLayer->m_GameObjects)
	{
		CBoxCollider* pSrcCol = (CBoxCollider*)pSrcObject->Get_Component(_strColliderTag);
		if (nullptr == pSrcCol)
			continue;

		for (auto& pDstObject : m_GameObjects)
		{
			CBoxCollider* pDstCol = (CBoxCollider*)pDstObject->Get_Component(_strColliderTag);
			if (nullptr == pDstCol)
				continue;

			_float fX, fY, fZ;

			//나와 상대 사이의 벡터를 구함
			_vec3 vDist = pSrcCol->Get_Desc().vPosition - pDstCol->Get_Desc().vPosition;

			//해당 벡터를 축분리하여 기록
			fX = vDist.x;
			fY = vDist.y;
			fZ = vDist.z;

			/*
			해당 축분리된 X, Y, Z에 대해, 미리 입력해둔 오브젝트의 사이즈(축과 평행함)와 비교하는 식
			상대와 나와의 중점의 거리의 차이의 두배가 두 오브젝트의 같은 축의 사이즈보다 같거나 작다면 두 오브젝트의 BOX는 만난 상태이다.
			*/
			if (abs(fX) * 2 <= pSrcCol->Get_Desc().vObjectSize.x + pDstCol->Get_Desc().vObjectSize.x &&
				abs(fY) * 2 <= pSrcCol->Get_Desc().vObjectSize.y + pDstCol->Get_Desc().vObjectSize.y &&
				abs(fZ) * 2 <= pSrcCol->Get_Desc().vObjectSize.z + pDstCol->Get_Desc().vObjectSize.z
				)
			{
				CComponent* pDmgInfoComp = pSrcObject->Get_Component(_strDmgInfoTag);
				pDstObject->Take_Damage(pDmgInfoComp);
			}
		}
	}

	return S_OK;
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