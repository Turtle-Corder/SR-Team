#pragma once
#ifndef __MONSTER_H__
#define __MONSTER_H__

USING(Engine)
BEGIN(Client)

class CMonster : public CGameObject 
{
public:
	explicit CMonster(LPDIRECT3DDEVICE9 _pDevice);
	explicit CMonster(const CMonster& _rOther);
	virtual ~CMonster() = default;

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void* _pArg) override;

	virtual _int Update_GameObject(_float _fDeltaTime) override;
	virtual _int LateUpdate_GameObject(_float _fDeltaTime) override;
	virtual HRESULT Render_NoneAlpha() override;

	virtual HRESULT Take_Damage() override;

private:
	HRESULT Add_Component();


public:
	static CMonster* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject* Clone_GameObject(void* _pArg) override;
	virtual void Free() override;



private:
	CVIBuffer*	m_pVIBufferCom	= nullptr;
	CTransform* m_pTransformCom = nullptr;
	CTexture*	m_pTextureCom	= nullptr;
	CCollider*	m_pColliderCom	= nullptr;
};

END

#endif // !__MONSTER_H__
