#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Point_Instance final : public CVIBuffer
{
protected:
	CVIBuffer_Point_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Point_Instance(const CVIBuffer_Point_Instance& rhs);
	virtual ~CVIBuffer_Point_Instance() = default;

public:
	virtual HRESULT Initialize_Prototype(_uint iNumInstance);
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render();

	virtual _uint3 Get_Indices(_uint iIndex)const { return _uint3(0, 0, 0); }

public:
	void Update(_float fTimeDelta);
	void Update(vector<_float3>& vecPosition);

private:
	ID3D11Buffer*				m_pVBInstance = nullptr;
	_uint						m_iInstanceStride = 0;
	_uint						m_iNumInstance = 0;

	_float*						m_pInstanceSpeeds = nullptr;

public:
	static CVIBuffer_Point_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumInstance = 1);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END