#pragma once
#ifndef __ITEM_H__
#define __ITEM_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CStatus;
class CItemManager;
class CItem : public CGameObject
{
private:
	explicit CItem(LPDIRECT3DDEVICE9 _pDevice);
	explicit CItem(const CItem& _rOther);
	virtual ~CItem() = default;


public:
	void Set_RenderShopItem(bool bRender); 
	void Set_RenderInvenItem(bool bRender); 

public:
	CTexture* Get_ItemInfo_Texture(const wstring& strItemTag);

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Setup_GameObject_Prototype() override;
	virtual HRESULT Setup_GameObject(void * pArg) override;
	virtual int Update_GameObject(float DeltaTime) override;
	virtual int LateUpdate_GameObject(float DeltaTime) override;
	virtual HRESULT Render_UI() override;


private:
	HRESULT Add_Component();
	HRESULT Add_Component_Item();

public:
	static CItem* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	bool			m_bRenderShopItem = false;
	bool			m_bRenderInvenItem = false;

	// Component-----------------------------------
	//CItemManager*	m_pItmeMgrCom = nullptr;

	// ������ �̹���
	CTexture*				m_pTextureCom[5];
	vector<INVEN_ITEM*>		m_vItemList;
};

END

#endif // !__ITEM_H__
