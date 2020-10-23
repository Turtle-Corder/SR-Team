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
	// ------------------------------------------------------------------
	// �ʿ��� �������� �ؽ�ó�� ��ȯ�ϴ� �Լ�
	// const wstring& strItemTag : �ʿ��� �ؽ�ó�� Tag
	// ------------------------------------------------------------------
	CTexture* Get_ItemInfo_Texture(const wstring& strItemTag);
	// ------------------------------------------------------------------
	// �ʿ��� �������� ������ ��ȯ�ϴ� �Լ�
	// const wstring& strItemTag : �ʿ��� �ؽ�ó�� Tag
	// ------------------------------------------------------------------
	const _int Get_ItemInfo_Price(const wstring& strItemTag);
	HRESULT Get_ItemInfo(const wstring& strItemTag, INVEN_ITEM& tItem);

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
