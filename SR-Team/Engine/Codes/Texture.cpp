#include "..\Headers\Texture.h"

USING(Engine)

CTexture::CTexture(LPDIRECT3DDEVICE9 _pDevice, TEXTURE_TYPE _eTextureType, const wstring & _strFilePath, _uint _iCount)
	: CComponent(_pDevice)
	, m_eTextureType(_eTextureType)
	, m_strFilePath(_strFilePath)
	, m_iCount(_iCount)
{
}

CTexture::CTexture(const CTexture & _rOther)
	: CComponent(_rOther)
	, m_Textures(_rOther.m_Textures)
	, m_eTextureType(_rOther.m_eTextureType)
	, m_strFilePath(_rOther.m_strFilePath)
	, m_iCount(_rOther.m_iCount)
{
	for (auto& pTexture : m_Textures)
		Safe_AddRef(pTexture);
}

HRESULT CTexture::Setup_Component_Prototype()
{
	IDirect3DBaseTexture9* pTexture = nullptr;
	TCHAR szFullPath[MAX_STR] = L"";
	HRESULT hr = 0;

	for (_uint iCnt = 0; iCnt < m_iCount; ++iCnt)
	{
		StringCchPrintf(szFullPath, _countof(szFullPath), m_strFilePath.c_str(), iCnt);

		switch (m_eTextureType)
		{
		case Engine::CTexture::TEXTURE_NORMAL:
			hr = D3DXCreateTextureFromFile(m_pDevice, szFullPath, (LPDIRECT3DTEXTURE9*)&pTexture);
			break;

		case Engine::CTexture::TEXTURE_CUBE:
			hr = D3DXCreateCubeTextureFromFile(m_pDevice, szFullPath, (LPDIRECT3DCUBETEXTURE9*)&pTexture);
			break;

		case Engine::CTexture::TEXTURE_SPRITE:
		{
			D3DXIMAGE_INFO tTexInfo;
			ZeroMemory(&tTexInfo, sizeof(D3DXIMAGE_INFO));

			hr = D3DXGetImageInfoFromFile(szFullPath, &tTexInfo);
			if (FAILED(hr))	break;

			hr = D3DXCreateTextureFromFileEx(m_pDevice,
				szFullPath,
				tTexInfo.Width,
				tTexInfo.Height,
				tTexInfo.MipLevels,
				0,
				tTexInfo.Format,
				D3DPOOL_MANAGED,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				0,
				nullptr,
				nullptr,
				(LPDIRECT3DTEXTURE9*)pTexture);
		}
		break;

		}

		if (FAILED(hr))
		{
			PRINT_LOG(szFullPath, LOG::SYSTEM);
			return E_FAIL;
		}

		m_Textures.emplace_back(pTexture);
	}

	return S_OK;
}

HRESULT CTexture::Setup_Component(void * _pArg)
{
	return S_OK;
}

HRESULT CTexture::SetTexture(_uint _iIndex)
{
	if (m_iCount <= _iIndex)
		return E_FAIL;

	return m_pDevice->SetTexture(0, m_Textures[_iIndex]);
}

const IDirect3DBaseTexture9 * CTexture::GetTexture(_uint _iIndex) const
{
	if (m_iCount <= _iIndex)
		return nullptr;

	return m_Textures[_iIndex];
}

CTexture * CTexture::Create(LPDIRECT3DDEVICE9 _pDevice, TEXTURE_TYPE _eTextureType, const wstring & _strFilePath, _uint _iCount)
{
	if (nullptr == _pDevice)
		return nullptr;

	CTexture* pInstance = new CTexture(_pDevice, _eTextureType, _strFilePath, _iCount);
	if (FAILED(pInstance->Setup_Component_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CTexture", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTexture::Clone_Component(void * _pArg)
{
	CTexture* pInstance = new CTexture(*this);
	if (FAILED(pInstance->Setup_Component(_pArg)))
	{
		PRINT_LOG(L"Failed To Clone_Component", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexture::Free()
{
	for (auto& pTexture : m_Textures)
		Safe_Release(pTexture);

	m_Textures.clear();

	CComponent::Free();
}
