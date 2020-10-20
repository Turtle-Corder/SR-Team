#pragma once
#ifndef __INVENTORY_H__
#define __INVENTORY_H__

USING(Engine)
BEGIN(Client)

class CItemManager;
class CInventory : public CGameObject
{
private:
	explicit CInventory(LPDIRECT3DDEVICE9 _pDevice);
	explicit CInventory(const CInventory& _rOther);
	virtual ~CInventory() = default;

public:
	void Set_Render(bool bRender);
	void Set_InvenItemList(list<INVEN_ITEM*>* pItemList);

public:
	bool Get_Render() { return m_bRender; }
	const list<INVEN_ITEM*>& Get_InvenList() { return m_pInvenList; }

public:
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * pArg) override;
	virtual _int Update_GameObject(float DeltaTime) override;
	virtual _int LateUpdate_GameObject(float DeltaTime) override;
	virtual HRESULT Render_UI() override;


private:
	HRESULT Add_Component();

public:
	static CInventory* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	bool				m_bRender = false;
	list<INVEN_ITEM*>	m_pInvenList;

	CItemManager*		m_pItemMgrCom = nullptr;
	CVIBuffer*			m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;

	D3DXVECTOR3			m_vPos[INVEN_END];
	D3DXVECTOR3			m_vScale[INVEN_END];

	int					m_iInsertOrder = 1;
};

END

#endif // !__INVENTORY_H__
