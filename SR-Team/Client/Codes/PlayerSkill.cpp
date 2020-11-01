#include "stdafx.h"
#include "..\Headers\PlayerSkill.h"


USING(Client)


CPlayerSkill::CPlayerSkill(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
}

CPlayerSkill::CPlayerSkill(const CPlayerSkill & _rOther)
	: CUIObject(_rOther)
{
}

HRESULT CPlayerSkill::Update_Delay(_float _fDeltaTime)
{
	if (m_iCanUseCnt < m_iMaxUseCnt)
	{
		m_fDelayCnt += _fDeltaTime;
		if (m_fDelayCnt > m_fEachDelay)
		{
			++m_iCanUseCnt;
			m_fDelayCnt = 0.f;
		}
	}

	return S_OK;
}

_bool CPlayerSkill::Can_UseSkill()
{
	if (0 >= m_iCanUseCnt)
		return false;

	return true;
}

_bool CPlayerSkill::Actual_UseSkill()
{
	return false;
}

_int CPlayerSkill::Get_ConsumeMP()
{
	return m_iConsumeMP;
}

void CPlayerSkill::Free()
{
	CUIObject::Free();
}
