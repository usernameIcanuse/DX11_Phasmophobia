#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
protected:
	CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
	virtual ~CVIBuffer_Terrain() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pHeightMapFilePath);
	virtual HRESULT Initialize(void* pArg) override;

public:
	virtual _uint3	Get_Indices(_uint iIndex)const
	{
 		return _uint3(((FACEINDICES32*)m_pIndices)[iIndex]._1, ((FACEINDICES32*)m_pIndices)[iIndex]._2,
			((FACEINDICES32*)m_pIndices)[iIndex]._3);
	}

private:
	_ulong					m_iNumVerticesX, m_iNumVerticesZ;

public:
	static CVIBuffer_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pHeightMapFilePath);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END