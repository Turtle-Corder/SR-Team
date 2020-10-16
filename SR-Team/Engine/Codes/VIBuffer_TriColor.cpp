#include "..\Headers\VIBuffer_TriColor.h"

USING(Engine)

CVIBuffer_TriColor::CVIBuffer_TriColor(LPDIRECT3DDEVICE9 _pDevice)
	: CVIBuffer(_pDevice)
{
}

CVIBuffer_TriColor::CVIBuffer_TriColor(const CVIBuffer_TriColor & _rOther)
	: CVIBuffer(_rOther)
{
}

HRESULT CVIBuffer_TriColor::Setup_Component_Prototype()
{
	m_iVertexCount = sizeof(VTX_COLOR);
	m_iFVF = VTX_CLOLOR_FVF;
	m_iVertexCount = 3;
	m_iTriCount = 1;

	if (FAILED(CVIBuffer::Setup_Component_Prototype()))
		return E_FAIL;

	VTX_COLOR* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec3(0.f, 1.f, 0.f);
	pVertex[0].iColor = D3DCOLOR_ARGB(255, 255, 0, 0);

	pVertex[1].vPosition = _vec3(1.f, -1.f, 0.f);
	pVertex[1].iColor = D3DCOLOR_ARGB(255, 255, 0, 0);;

	pVertex[2].vPosition = _vec3(-1.f, -1.f, 0.f);
	pVertex[2].iColor = D3DCOLOR_ARGB(255, 255, 0, 0);

	m_pVB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_TriColor::Setup_Component(void * _pArg)
{
	if (nullptr != _pArg)
	{
		// TODO this ...
	}

	return E_NOTIMPL;
}

CVIBuffer_TriColor * CVIBuffer_TriColor::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	return nullptr;
}

CComponent * CVIBuffer_TriColor::Clone_Component(void * _pArg)
{
	return nullptr;
}

void CVIBuffer_TriColor::Free()
{
}
