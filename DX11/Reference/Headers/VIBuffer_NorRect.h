#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_NorRect final : public CVIBuffer
{
protected:
	CVIBuffer_NorRect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_NorRect(const CVIBuffer_NorRect& rhs);
	virtual ~CVIBuffer_NorRect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	virtual _uint3	Get_Indices(_uint iIndex)const
	{
		return _uint3(((FACEINDICES32*)m_pIndices)[iIndex]._1, ((FACEINDICES32*)m_pIndices)[iIndex]._2,
			((FACEINDICES32*)m_pIndices)[iIndex]._3);
	}


public:
	static CVIBuffer_NorRect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END