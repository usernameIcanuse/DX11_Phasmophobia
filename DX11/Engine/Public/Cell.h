#pragma once

#include "Base.h"

BEGIN(Engine)

class CCell final : public CBase
{
public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };
private:
	CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);	
	virtual ~CCell() = default;

public:
	_vector Get_Point(POINT ePoint) {
		return XMLoadFloat3(&m_vPoints[ePoint]);
	}

	_int Get_Index() const {
		return m_iIndex;
	}

	void Set_Neighbor(LINE eLine, _int iNeighborIndex) {
		m_iNeighbor[eLine] = iNeighborIndex;
	}

public:
	HRESULT Initialize(const _float3* pPoints, _int iIndex);
	_bool Compare_Points(_fvector vSourPoint, _fvector vDestPoint);
#ifdef _DEBUG
	_bool Compare_Points(_fvector vSourPoint, _fvector vDestPoint, _int iIndex);	
#endif
	_bool isIn(_fvector vPosition, _int* pNeighborIndex, _float& fPositionY, _int& iPrevIndex);
	_bool isSamePoints(_float3 vPointA, _float3 vPointB, _float3 vPointC);

#ifdef _DEBUG
public:
	HRESULT Render();
#endif // _DEBUG

private:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pContext = nullptr;

	_uint						m_iIndex = 0;
	_float3						m_vPoints[POINT_END]; 
	_float3						m_vLine[LINE_END];
	_float3						m_vNormal[LINE_END];
	_int						m_iNeighbor[LINE_END] = { -1, -1, -1 };

	_float4						m_vCellPlane;
#ifdef _DEBUG
	class CVIBuffer_Cell*		m_pVIBuffer = nullptr;
#endif

public:
	static CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _int iIndex);
	virtual void Free() override;
};

END