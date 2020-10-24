#include "..\Headers\NavAgent.h"

USING(Engine)

CNavAgent::CNavAgent(LPDIRECT3DDEVICE9 _pDevice)
	: CComponent(_pDevice)
{
}

CNavAgent::CNavAgent(const CNavAgent & _rOther)
	: CComponent(_rOther)
{
}

const CNavAgent::NAV_DESC & CNavAgent::Get_NavDesc() const
{
	return m_tNavDesc;
}

bool CNavAgent::IsFound() const
{
	return m_bNavDone;
}

HRESULT CNavAgent::Setup_Component_Prototype()
{
	return S_OK;
}

HRESULT CNavAgent::Setup_Component(void * _pArg)
{
	if (_pArg)
		memcpy_s(&m_tNavDesc, sizeof(NAV_DESC), _pArg, sizeof(NAV_DESC));

	return S_OK;
}

void CNavAgent::Set_NavDone()
{
	m_bNavDone = true;
}

void CNavAgent::Set_NavDesc(const NAV_DESC & tNavDesc)
{
	m_bNavDone = false;
	m_tNavDesc = tNavDesc;
	m_BranchList.clear();
}

void CNavAgent::Set_NavDesc(_int _iStartX, _int _iStartY, _int _iGoalX, _int _iGoalY)
{
	m_bNavDone = false;
	m_tNavDesc.tStart = { _iStartX, _iStartY };
	m_tNavDesc.tGoal = { _iGoalX, _iGoalY };
}

void CNavAgent::Set_NavDesc(const _ivec2 & _rStart, const _ivec2 & _rGoal)
{
	m_bNavDone = false;
	m_tNavDesc.tStart = _rStart;
	m_tNavDesc.tGoal = _rGoal;
}

void CNavAgent::Add_Branch(const _ivec2 & _rBranch)
{
	m_BranchList.push_front(_rBranch);
}

bool CNavAgent::Get_NextGoal(_ivec2 * _pBranch)
{
	if (0 >= m_BranchList.size())
		return false;

	*_pBranch = m_BranchList.back();
	m_BranchList.pop_back();
	return true;
}

CNavAgent * CNavAgent::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
		return nullptr;

	CNavAgent* pInstance = new CNavAgent(_pDevice);
	if (FAILED(pInstance->Setup_Component_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CNavAgent", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CNavAgent::Clone_Component(void * _pArg)
{
	CNavAgent* pInstance = new CNavAgent(*this);
	if (FAILED(pInstance->Setup_Component(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone CNavAgent", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavAgent::Free()
{
	m_BranchList.clear();

	CComponent::Free();
}
