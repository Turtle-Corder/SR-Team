#pragma once

#ifndef __SKILLINVENTORY_H__
#define __SKILLINVENTORY_H__

#include "GameObject.h"
class CPlayerSkill;
BEGIN(Client)
USING(Engine)

// �÷��̾��� ��ų���� �����ϴ� Ŭ����
class CSkillInventory : public CGameObject
{
public:
	explicit CSkillInventory(LPDIRECT3DDEVICE9 pDevice);
	explicit CSkillInventory(const CSkillInventory& _rOther);
	virtual ~CSkillInventory() = default;

public:
	virtual void Free() override;

public:
	CPlayerSkill*	m_pPlayerSkill[8] = { nullptr, };
};

END
#endif