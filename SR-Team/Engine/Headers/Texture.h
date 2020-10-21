#pragma once
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
public:
	enum TEXTURE_TYPE
	{
		TEXTURE_NORMAL,
		TEXTURE_CUBE,
		TEXTURE_SPRITE,
	};

private:
	explicit CTexture(LPDIRECT3DDEVICE9 _pDevice, TEXTURE_TYPE _eTextureType, const wstring& _strFilePath, _uint _iCount);
	explicit CTexture(const CTexture& _rOther);
	virtual ~CTexture() = default;

public:
	virtual HRESULT Setup_Component_Prototype() override;
	virtual HRESULT Setup_Component(void* _pArg) override;

	HRESULT SetTexture(_uint _iIndex);
	const IDirect3DBaseTexture9* GetTexture(_uint _iIndex) const;
	const D3DXIMAGE_INFO* Get_TexInfo(_uint _iIndex) const;

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 _pDevice, TEXTURE_TYPE _eTextureType, const wstring& _strFilePath, _uint _iCount = 1);
	virtual CComponent* Clone_Component(void* _pArg) override;
	virtual void Free() override;

private:
	typedef vector<IDirect3DBaseTexture9*> TEXTURES;
	TEXTURES m_Textures;

	vector<D3DXIMAGE_INFO*> m_TexInfo;

	TEXTURE_TYPE	m_eTextureType	= TEXTURE_NORMAL;
	wstring			m_strFilePath	= L"";
	_uint			m_iCount		= 0;
};

END

#endif
