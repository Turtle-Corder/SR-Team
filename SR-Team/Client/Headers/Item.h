#pragma once
#ifndef __ITEM_H__
#define __ITEM_H__

#include "GameObject.h"

USING(Engine)
BEGIN(Client)

class CStatus;
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

	// ------------------------------------------------------------------
	// �ʿ��� �������� ������ ��ȯ�ϴ� �Լ�
	// const wstring& strItemTag : �ʿ��� �������� Tag
	// ------------------------------------------------------------------
	CStatus* Get_ItemStat(const wstring& strItemTag);


public:
	// ------------------------------------------------------------------
	// �ʿ��� ��Ƽ�� ��ų�������� ��ü�� ��ȯ�ϴ� �Լ�
	// _int iSkillID : ��ȯ���� ��ü�� ��Ƽ�� ��ų ���̵�
	// ------------------------------------------------------------------
	INVEN_ITEM* Get_ActiveSkillIcon(_int iSkillID);


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
	HRESULT Add_Component_SkillIcon();

public:
	static CItem* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	bool			m_bRenderShopItem = false;
	bool			m_bRenderInvenItem = false;

// ������
private:
	CTexture*				m_pTextureCom[10];
	CStatus*				m_pStatCom[10];
	vector<INVEN_ITEM*>		m_vItemList;

// ��ų ������
private:
	CTexture*				m_pTextureSkillIcon[9];
	vector<INVEN_ITEM*>		m_vSkillIconList;

};	

END

#endif // !__ITEM_H__
