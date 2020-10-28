#pragma once

#ifndef __LASERSKILL_H__
#define __LASERSKILL_H__

#include "PlayerSkill.h"
USING(Engine)
BEGIN(Client)

class CLaserSkill : public CPlayerSkill
{
public:
	explicit CLaserSkill(LPDIRECT3DDEVICE9 _pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	explicit CLaserSkill(const CLaserSkill& _rOther);
	virtual ~CLaserSkill() = default;

public:
	// CPlayerSkill��(��) ���� ��ӵ�
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * _pArg) override;
	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;

public:
	static CLaserSkill* Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE _pSprite, LPD3DXFONT _pFont);
	virtual CGameObject * Clone_GameObject(void * _pArg) override;

public:
	virtual HRESULT Use_Skill(float fDeltaTime) override;
};

END
#endif