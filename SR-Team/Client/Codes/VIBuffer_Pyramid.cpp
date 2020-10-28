#include "stdafx.h"
#include "Pipeline.h"
#include "..\Headers\VIBuffer_Pyramid.h"

USING(Client)

CVIBuffer_Pyramid::CVIBuffer_Pyramid(LPDIRECT3DDEVICE9 pDevice)
	: CVIBuffer(pDevice)
{
}

CVIBuffer_Pyramid::CVIBuffer_Pyramid(const CVIBuffer_Pyramid & other)
	: CVIBuffer(other)
	, m_pVertices(other.m_pVertices)
	, m_pConvertVertices(other.m_pConvertVertices)
{
}

HRESULT CVIBuffer_Pyramid::Setup_Component_Prototype()
{
	m_iVertexSize = sizeof(VTX_CUBETEXTURE);
	m_iIndexSize = sizeof(INDEX16);
	m_IndexFmt = D3DFMT_INDEX16;
	m_iFVF = VTX_CUBETEXTURE_FVF;
	m_iVertexCount = 5;
	m_iTriCount = 6;

	if (FAILED(CVIBuffer::Setup_Component_Prototype()))
		return E_FAIL;

	VTX_CUBETEXTURE*	pVertex = nullptr;

	// ���ؽ����۰� �����ϰ� �ִ� ���ؽ��� ������ �Լ�.
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = D3DXVECTOR3(0.0f, 1.f, 0.0f);
	pVertex[0].vUV = pVertex[0].vPosition;

	pVertex[1].vPosition = D3DXVECTOR3(0.5f, 0.f, -0.5f);
	pVertex[1].vUV = pVertex[1].vPosition;

	pVertex[2].vPosition = D3DXVECTOR3(-0.5f, 0.f, -0.5f);
	pVertex[2].vUV = pVertex[2].vPosition;

	pVertex[3].vPosition = D3DXVECTOR3(0.5f, 0.f, 0.5f);
	pVertex[3].vUV = pVertex[3].vPosition;

	pVertex[4].vPosition = D3DXVECTOR3(-0.5f, 0.f, 0.5f);
	pVertex[4].vUV = pVertex[4].vPosition;

	// ���� ���ؽ� ����
	m_pVertices = new VTX_CUBETEXTURE[m_iVertexCount];
	memcpy(m_pVertices, pVertex, sizeof(VTX_CUBETEXTURE) * m_iVertexCount);

	m_pConvertVertices = new VTX_CUBETEXTURE[m_iVertexCount];
	memcpy(m_pConvertVertices, pVertex, sizeof(VTX_CUBETEXTURE) * m_iVertexCount);

	m_pVB->Unlock();

	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// ��� 4��
	pIndex[0]._1 = 0;
	pIndex[0]._2 = 3;
	pIndex[0]._3 = 1;

	pIndex[1]._1 = 0;
	pIndex[1]._2 = 4;
	pIndex[1]._3 = 3;
		
	pIndex[2]._1 = 0;
	pIndex[2]._2 = 2;
	pIndex[2]._3 = 4;

	pIndex[3]._1 = 0;
	pIndex[3]._2 = 1;
	pIndex[3]._3 = 2;

	// �ظ�
	pIndex[4]._1 = 2;
	pIndex[4]._2 = 4;
	pIndex[4]._3 = 3;

	pIndex[5]._1 = 2;
	pIndex[5]._2 = 3;
	pIndex[5]._3 = 1;


	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Pyramid::Setup_Component(void * pArg)
{
	return S_OK;
}

void * CVIBuffer_Pyramid::Get_Vertices() const
{
	return m_pVertices;
}

void * CVIBuffer_Pyramid::Get_Indices() const
{
	return nullptr;
}

HRESULT CVIBuffer_Pyramid::Set_Transform(const _matrix * _pMatWorld, const _matrix * _pMatView, const _matrix * _pMatProj)
{
	for (_uint i = 0; i < m_iVertexCount; ++i)
	{
		// ���� ��ȯ
		CPipeline::TransformCoord(
			&m_pConvertVertices[i].vPosition,
			&m_pVertices[i].vPosition, _pMatWorld);

		// �� ��ȯ
		CPipeline::TransformCoord(
			&m_pConvertVertices[i].vPosition,
			&m_pConvertVertices[i].vPosition, _pMatView);

		if (1.f >= m_pConvertVertices[i].vPosition.z)
			continue;

		// ���� ��ȯ (Z��������� �ؾ� ������ȯ�� �Ϸ�)
		CPipeline::TransformCoord(
			&m_pConvertVertices[i].vPosition,
			&m_pConvertVertices[i].vPosition, _pMatProj);
	}

	VTX_CUBETEXTURE* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	memcpy(pVertex, m_pConvertVertices, sizeof(VTX_CUBETEXTURE) * m_iVertexCount);

	m_pVB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Pyramid::Set_Transform(const _matrix * _pMatWorld, const CCamera * _pCamera)
{
	for (_uint i = 0; i < m_iVertexCount; ++i)
	{
		// ���� ��ȯ
		CPipeline::TransformCoord(
			&m_pConvertVertices[i].vPosition,
			&m_pVertices[i].vPosition, _pMatWorld);

		// �� ��ȯ
		CPipeline::TransformCoord(
			&m_pConvertVertices[i].vPosition,
			&m_pConvertVertices[i].vPosition, _pCamera->Get_ViewMatrix());

		if (1.f >= m_pConvertVertices[i].vPosition.z)
			continue;

		// ���� ��ȯ (Z��������� �ؾ� ������ȯ�� �Ϸ�)
		CPipeline::TransformCoord(
			&m_pConvertVertices[i].vPosition,
			&m_pConvertVertices[i].vPosition, _pCamera->Get_ProjMatrix());
	}

	VTX_CUBETEXTURE* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	memcpy(pVertex, m_pConvertVertices, sizeof(VTX_CUBETEXTURE) * m_iVertexCount);

	m_pVB->Unlock();

	return S_OK;
}

CVIBuffer_Pyramid * CVIBuffer_Pyramid::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return nullptr;

	CVIBuffer_Pyramid* pInstance = new CVIBuffer_Pyramid(pDevice);

	if (FAILED(pInstance->Setup_Component_Prototype()))
	{
		PRINT_LOG(L"Failed To Create CVIBuffer_CubeTexture", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Pyramid::Clone_Component(void * pArg)
{
	CVIBuffer_Pyramid* pInstance = new CVIBuffer_Pyramid(*this);
	if (FAILED(pInstance->Setup_Component(pArg)))
	{
		PRINT_LOG(L"Failed To Clone CVIBuffer_CubeTexture", LOG::CLIENT);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Pyramid::Free()
{
	if (!m_bIsClone)
	{
		Safe_Delete_Array(m_pConvertVertices);
		Safe_Delete_Array(m_pVertices);
	}

	CVIBuffer::Free();
}