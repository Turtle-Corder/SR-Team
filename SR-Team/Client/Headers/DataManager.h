#pragma once
#ifndef __ITEM_H__
#define __ITEM_H__

#include "Base.h"

USING(Engine)
BEGIN(Client)

class CStatus;
class CDataManager final : public CBase
{
	DECLARE_SINGLETON(CDataManager)

private:
	CDataManager();
	virtual ~CDataManager() = default;

public:
	// CGameObject��(��) ���� ��ӵ�
	HRESULT Setup_DataManager();

private:
	HRESULT Setup_Component_Item();
	HRESULT Setup_Component_SkillIcon();

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

	virtual void Free() override;


public:
	// ------------------------------------------------------------------
	// �ʿ��� ��Ƽ�� ��ų�������� ��ü�� ��ȯ�ϴ� �Լ�
	// _int iSkillID : ��ȯ���� ��ü�� ��Ƽ�� ��ų ���̵�
	// ------------------------------------------------------------------
	INVEN_ITEM* Get_ActiveSkillIcon(_int iSkillID);



// ������
private:
	CTexture*				m_pTextureCom[17];
	CStatus*				m_pStatCom[17];
	vector<INVEN_ITEM*>		m_vItemList;

// ��ų ������
private:
	CTexture*				m_pTextureSkillIcon[9];
	vector<INVEN_ITEM*>		m_vSkillIconList;

};	

END

#endif // !__ITEM_H__
