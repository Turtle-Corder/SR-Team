#include "stdafx.h"
#include "..\Headers\NormalAtt.h"


USING(Client)


CNormalAtt::CNormalAtt(LPDIRECT3DDEVICE9 _pDevice)
	: CPlayerSkill(_pDevice)
{
}

CNormalAtt::CNormalAtt(const CNormalAtt & _rOther)
	: CPlayerSkill(_rOther)
{
}


HRESULT CNormalAtt::Use_Skill(_float fDeltaTime, CTransform& rPlayerRight)
{
	if (m_bRightAtt)
	{
		m_fAttTime += fDeltaTime;
		if (m_fAttTime >= 0.2f)
		{
			m_fAttTime = 0.f;
			//m_bIsNormalAtt = false;
			m_bRightAtt = false;
			m_bLeftAtt = true;
		}

		(&rPlayerRight)->Turn(CTransform::AXIS_Y, fDeltaTime * 5.f);
	}

	else if (m_bLeftAtt)
	{
		m_fAttTime += fDeltaTime;

		(&rPlayerRight)->Turn(CTransform::AXIS_Y, -fDeltaTime * 5.f);

		if (m_fAttTime >= 0.3f)
		{
			m_fAttTime = 0.f;
			m_bIsNormalAtt = false;
			m_bRightAtt = false;
			m_bLeftAtt = false;
		}
	}

	else
	{
		m_fAttTime += fDeltaTime;

		(&rPlayerRight)->Turn(CTransform::AXIS_X, fDeltaTime * 5.f);

		if (m_fAttTime >= 0.2f)
		{
			m_fAttTime = 0.f;
			m_bRightAtt = true;
			m_bIsNormalAtt = true;
		}
	}

	return S_OK;
}
