#include "Pipeline.h"
#include "..\Headers\VIBuffer_TerrainTexture.h"

USING(Engine)

CVIBuffer_TerrainTexture::CVIBuffer_TerrainTexture(LPDIRECT3DDEVICE9 _pDevice)
	: CVIBuffer(_pDevice)
{
}

CVIBuffer_TerrainTexture::CVIBuffer_TerrainTexture(const CVIBuffer_TerrainTexture & _rOther)
	: CVIBuffer(_rOther)
	, m_pVTXOrigin(_rOther.m_pVTXOrigin)
	, m_pVTXConvert(_rOther.m_pVTXConvert)
	, m_pIndices(_rOther.m_pIndices)
	, m_iVertexCountX(_rOther.m_iVertexCountX)
	, m_iVertexCountZ(_rOther.m_iVertexCountZ)
	, m_fVertexInterval(_rOther.m_fVertexInterval)
{
}

void * CVIBuffer_TerrainTexture::Get_Vertices() const
{
	return m_pVTXOrigin;
}

void * CVIBuffer_TerrainTexture::Get_Indices() const
{
	return m_pIndices;
}

HRESULT CVIBuffer_TerrainTexture::Setup_Component_Prototype()
{
	m_iVertexSize = sizeof(VTX_TEXTURE);
	m_iIndexSize = sizeof(INDEX16);
	m_IndexFmt = D3DFMT_INDEX16;
	m_iFVF = VTX_TEXTURE_FVF;
	m_iVertexCount = m_iVertexCountX * m_iVertexCountZ;
	m_iTriCount = (m_iVertexCountX - 1) * (m_iVertexCountZ - 1) * 2;

	if (FAILED(CVIBuffer::Setup_Component_Prototype()))
		return E_FAIL;


	_uint iIndex = 0;

	VTX_TEXTURE*	pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	for (_uint iCntZ = 0; iCntZ < m_iVertexCountZ; ++iCntZ)
	{
		for (_uint iCntX = 0; iCntX < m_iVertexCountX; ++iCntX)
		{
			iIndex = iCntZ * m_iVertexCountX + iCntX;

			pVertex[iIndex].vPosition = D3DXVECTOR3(iCntX * m_fVertexInterval, 0.f, iCntZ * m_fVertexInterval);
			pVertex[iIndex].vUV = D3DXVECTOR2((float)iCntX / m_iVertexCountX, 1.f - (float)iCntZ / m_iVertexCountZ);
		}
	}

	m_pVTXOrigin = new VTX_TEXTURE[m_iVertexCount];
	memcpy_s(m_pVTXOrigin, sizeof(VTX_TEXTURE) * m_iVertexCount, pVertex, sizeof(VTX_TEXTURE) * m_iVertexCount);

	m_pVTXConvert = new VTX_TEXTURE[m_iVertexCount];
	memcpy_s(m_pVTXConvert, sizeof(VTX_TEXTURE) * m_iVertexCount, pVertex, sizeof(VTX_TEXTURE) * m_iVertexCount);

	m_pVB->Unlock();


	_uint iTriIndex = 0;

	INDEX16* pIndex = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for (_uint iCntZ = 0; iCntZ < m_iVertexCountZ - 1; ++iCntZ)
	{
		for (_uint iCntX = 0; iCntX < m_iVertexCountX - 1; ++iCntX)
		{
			iIndex = iCntZ * m_iVertexCountX + iCntX;
			
			// 위
			pIndex[iTriIndex]._1 = iIndex + m_iVertexCountX;
			pIndex[iTriIndex]._2 = iIndex + m_iVertexCountX + 1;
			pIndex[iTriIndex++]._3 = iIndex + 1;

			// 아래 
			pIndex[iTriIndex]._1 = iIndex + m_iVertexCountX;
			pIndex[iTriIndex]._2 = iIndex + 1;
			pIndex[iTriIndex++]._3 = iIndex;
		}
	}

	m_pIndices = new INDEX16[m_iTriCount];
	memcpy_s(m_pIndices, sizeof(INDEX16) * m_iTriCount, pIndex, sizeof(INDEX16) * m_iTriCount);

	m_pIB->Unlock();
}

HRESULT CVIBuffer_TerrainTexture::Setup_Component_Prototype(const wstring & _strFilePath)
{
	// TODO : 데이터 저장되는 형식에 맞춰서 파싱해서 작업

	// .. 파싱 ..

	if (FAILED(CVIBuffer::Setup_Component_Prototype()))
		return E_FAIL;

	// .. 셋팅 ..

	return E_NOTIMPL;
}

HRESULT CVIBuffer_TerrainTexture::Setup_Component_Prototype(const wstring & _strFilePath, const wstring & _strHeightPath)
{
	// .. 파싱 ..

	if (FAILED(CVIBuffer::Setup_Component_Prototype()))
		return E_FAIL;

	// .. 셋팅 ..

	_pixel* pPixels = Load_HeightMap_FromFile(_strHeightPath);
	if (nullptr == pPixels)
		return E_FAIL;

	VTX_TEXTURE* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	// .. 셋팅 ..

	m_pVB->Unlock();
	Safe_Delete_Array(pPixels);

	return E_NOTIMPL;
}

HRESULT CVIBuffer_TerrainTexture::Setup_Component(void * _pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_TerrainTexture::Set_Transform(const _matrix * _pMatWorld, const _matrix * _pMatView, const _matrix * _pMatProj)
{
	for (_uint iCnt = 0; iCnt < m_iVertexCount; ++iCnt)
	{
		// World
		CPipeline::TransformCoord(&m_pVTXConvert[iCnt].vPosition, &m_pVTXOrigin[iCnt].vPosition, _pMatWorld);

		// View
		CPipeline::TransformCoord(&m_pVTXConvert[iCnt].vPosition, &m_pVTXConvert[iCnt].vPosition, _pMatView);

		if (1.f >= m_pVTXConvert[iCnt].vPosition.z)
			continue;

		// Projection
		CPipeline::TransformCoord(&m_pVTXConvert[iCnt].vPosition, &m_pVTXConvert[iCnt].vPosition, _pMatProj);
	}

	VTX_TEXTURE* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	memcpy_s(pVertex, sizeof(VTX_TEXTURE) * m_iVertexCount, m_pVTXConvert, sizeof(VTX_TEXTURE) * m_iVertexCount);
	m_pVB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_TerrainTexture::Set_Transform(const _matrix * _pMatWorld, const CCamera * _pCamera)
{
	_matrix matView;
	_matrix matProj;

	//matView = _pCamera->Get_ViewMatrix();
	//matProj = _pCamera->Get_ProjectionMatrix();

	for (_uint iCnt = 0; iCnt < m_iVertexCount; ++iCnt)
	{
		// World
		CPipeline::TransformCoord(&m_pVTXConvert[iCnt].vPosition, &m_pVTXOrigin[iCnt].vPosition, _pMatWorld);

		// View
		CPipeline::TransformCoord(&m_pVTXConvert[iCnt].vPosition, &m_pVTXConvert[iCnt].vPosition, &matView);

		if (1.f >= m_pVTXConvert[iCnt].vPosition.z)
			continue;

		// Projection
		CPipeline::TransformCoord(&m_pVTXConvert[iCnt].vPosition, &m_pVTXConvert[iCnt].vPosition, &matProj);
	}

	VTX_TEXTURE* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	memcpy_s(pVertex, sizeof(VTX_TEXTURE) * m_iVertexCount, m_pVTXConvert, sizeof(VTX_TEXTURE) * m_iVertexCount);
	m_pVB->Unlock();

	return S_OK;
}

_bool CVIBuffer_TerrainTexture::IsOnTerrain(_vec3 * _pInOutPos)
{
	_uint iX = _uint(_pInOutPos->x / m_fVertexInterval);
	_uint iZ = _uint(_pInOutPos->z / m_fVertexInterval);

	_uint iIndex = iZ * m_iVertexCountX + iX;
	if (m_iVertexCountX * m_iVertexCountZ <= iIndex)
		return false;

	_float fRatioX = _pInOutPos->x - m_pVTXOrigin[iIndex + m_iVertexCountX].vPosition.x;
	_float fRatioZ = m_pVTXOrigin[iIndex + m_iVertexCountX].vPosition.z - _pInOutPos->z;

	D3DXPLANE Plane;

	if (fRatioX > fRatioZ)		// 위
	{
		D3DXPlaneFromPoints(&Plane,
			);
	}
	else 						//아래
	{
		D3DXPlaneFromPoints(&Plane,
			);
	}

}

CVIBuffer_TerrainTexture * CVIBuffer_TerrainTexture::Create(LPDIRECT3DDEVICE9 _pDevice, _uint _iVertexCountX, _uint _iVertexCountZ, _float _fVertexInterval)
{
	if (nullptr == _pDevice)
		return nullptr;

	CVIBuffer_TerrainTexture* pInstance = new CVIBuffer_TerrainTexture(_pDevice);
	pInstance->m_iVertexCountX = _iVertexCountX;
	pInstance->m_iVertexCountZ = _iVertexCountZ;
	pInstance->m_fVertexInterval = _fVertexInterval;

	if (FAILED(pInstance->Setup_Component_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CVIBuffer_TerrainTexture", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CVIBuffer_TerrainTexture * CVIBuffer_TerrainTexture::Create(LPDIRECT3DDEVICE9 _pDevice, const wstring & _strFilePath)
{
	if (nullptr == _pDevice)
		return nullptr;

	CVIBuffer_TerrainTexture* pInstance = new CVIBuffer_TerrainTexture(_pDevice);
	if (FAILED(pInstance->Setup_Component_Prototype(_strFilePath)))
	{
		PRINT_LOG(L"Failed To Create CVIBuffer_TerrainTexture Form DataPath", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CVIBuffer_TerrainTexture * CVIBuffer_TerrainTexture::Create(LPDIRECT3DDEVICE9 _pDevice, const wstring & _strDataPath, const wstring & _strHeightPath)
{
	if (nullptr == _pDevice)
		return nullptr;

	CVIBuffer_TerrainTexture* pInstance = new CVIBuffer_TerrainTexture(_pDevice);
	if (FAILED(pInstance->Setup_Component_Prototype(_strDataPath, _strHeightPath)))
	{
		PRINT_LOG(L"Failed To Create CVIBuffer_TerrainTexture Form Data&Height", LOG::ENGINE);
		Safe_Release(pInstance);
	}

	return pInstance;
}
