#include "stdafx.h"
#include "..\Headers\PlayerItem.h"


USING(Client)


CPlayerItem::CPlayerItem(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont)
	: CUIObject(_pDevice, _pSprite, _pFont)
{
}

CPlayerItem::CPlayerItem(const CPlayerItem & _rOther)
	: CUIObject(_rOther)
{
}

HRESULT CPlayerItem::Update_Delay(_float _fDeltaTime)
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

_bool CPlayerItem::Can_UseItem()
{
	if (0 >= m_iCanUseCnt)
		return false;

	return true;
}

_bool CPlayerItem::Actual_UseItem()
{
	return false;
}

void CPlayerItem::Free()
{
	CUIObject::Free();
}
