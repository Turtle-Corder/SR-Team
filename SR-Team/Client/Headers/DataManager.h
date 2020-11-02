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
	// CGameObject을(를) 통해 상속됨
	HRESULT Setup_DataManager();

private:
	HRESULT Setup_Component_Item();
	HRESULT Setup_Component_SkillIcon();

public:
	// ------------------------------------------------------------------
	// 필요한 아이템의 텍스처를 반환하는 함수
	// const wstring& strItemTag : 필요한 텍스처의 Tag
	// ------------------------------------------------------------------
	CTexture* Get_ItemInfo_Texture(const wstring& strItemTag);

	// ------------------------------------------------------------------
	// 필요한 아이템의 가격을 반환하는 함수
	// const wstring& strItemTag : 필요한 텍스처의 Tag
	// ------------------------------------------------------------------
	const _int Get_ItemInfo_Price(const wstring& strItemTag);
	HRESULT Get_ItemInfo(const wstring& strItemTag, INVEN_ITEM& tItem);

	// ------------------------------------------------------------------
	// 필요한 아이템의 스탯을 반환하는 함수
	// const wstring& strItemTag : 필요한 아이템의 Tag
	// ------------------------------------------------------------------
	CStatus* Get_ItemStat(const wstring& strItemTag);

	virtual void Free() override;


public:
	// ------------------------------------------------------------------
	// 필요한 액티브 스킬아이콘의 객체를 반환하는 함수
	// _int iSkillID : 반환받을 객체의 액티브 스킬 아이디
	// ------------------------------------------------------------------
	INVEN_ITEM* Get_ActiveSkillIcon(_int iSkillID);



// 아이템
private:
	CTexture*				m_pTextureCom[17];
	CStatus*				m_pStatCom[17];
	vector<INVEN_ITEM*>		m_vItemList;

// 스킬 아이콘
private:
	CTexture*				m_pTextureSkillIcon[9];
	vector<INVEN_ITEM*>		m_vSkillIconList;

};	

END

#endif // !__ITEM_H__
