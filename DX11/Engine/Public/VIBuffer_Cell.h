#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Cell final : public CVIBuffer
{
protected:
	CVIBuffer_Cell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Cell(const CVIBuffer_Cell& rhs);
	virtual ~CVIBuffer_Cell() = default;

public:
	virtual HRESULT Initialize_Prototype(const _float3* pVerticesPos);
	virtual HRESULT Initialize(void* pArg) override;

public:
	virtual _uint3	Get_Indices(_uint iIndex)const
	{
		return _uint3(((FACEINDICES16*)m_pIndices)[iIndex]._1, ((FACEINDICES16*)m_pIndices)[iIndex]._2,
			((FACEINDICES16*)m_pIndices)[iIndex]._3);
	}
public:
	static CVIBuffer_Cell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pVerticesPos);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END