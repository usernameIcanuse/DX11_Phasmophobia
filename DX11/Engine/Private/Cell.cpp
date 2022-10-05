#include "..\Public\Cell.h"
#include "VIBuffer_Cell.h"

CCell::CCell(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CCell::Initialize(const _float3 * pPoints, _int iIndex)
{
	m_iIndex = iIndex;

	memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);

	XMStoreFloat3(&m_vLine[LINE_AB], XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A]));
	XMStoreFloat3(&m_vLine[LINE_BC], XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B]));
	XMStoreFloat3(&m_vLine[LINE_CA], XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C]));

	m_vNormal[LINE_AB] = _float3(m_vLine[LINE_AB].z * -1.f, 0.f, m_vLine[LINE_AB].x);
	m_vNormal[LINE_BC] = _float3(m_vLine[LINE_BC].z * -1.f, 0.f, m_vLine[LINE_BC].x);
	m_vNormal[LINE_CA] = _float3(m_vLine[LINE_CA].z * -1.f, 0.f, m_vLine[LINE_CA].x);

	
	XMStoreFloat4(&m_vCellPlane, 
		XMPlaneFromPoints(XMLoadFloat3(&m_vPoints[POINT_A]),
			XMLoadFloat3(&m_vPoints[POINT_B
			]), XMLoadFloat3(&m_vPoints[POINT_C])));
		
	

#ifdef _DEBUG
	m_pVIBuffer = CVIBuffer_Cell::Create(m_pDevice, m_pContext, m_vPoints);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;
#endif // _DEBUG
	
	return S_OK;
}


/* 전달받은 두개의 점과 내 세갱릐 점중 같은 점이 두개냐?! */
_bool CCell::Compare_Points(_fvector vSourPoint, _fvector vDestPoint)
{
	
	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
			return true;

		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))	
			return true;	
	}


	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
			return true;

		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
			return true;
	}

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
			return true;

		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
			return true;
	}

	return false;	
}
#ifdef _DEBUG
_bool CCell::Compare_Points(_fvector vSourPoint, _fvector vDestPoint, _int iIndex)
{
	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
		{
			Set_Neighbor(LINE_AB, iIndex);
			return true;
		}
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
		{
			Set_Neighbor(LINE_CA, iIndex);
			return true;
		}
	}


	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
		{
			Set_Neighbor(LINE_AB, iIndex);
			return true;
		}
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
		{
			Set_Neighbor(LINE_BC, iIndex);
			return true;
		}
	}

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
		{
			Set_Neighbor(LINE_CA, iIndex);
			return true;
		}
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
		{
			Set_Neighbor(LINE_BC, iIndex);

			return true;
		}
	}

	return false;
}

#endif


_bool CCell::isIn(_fvector vPosition, _int* pNeighborIndex, _float& fPositionY, _int& iPrevIndex)
{
	/* 이 쎌의 세변에 대해서 나갔는지 안낙ㅆ다ㅣ;ㅓ 판단한다. */
	for (_uint i = 0; i < LINE_END; ++i)
	{
		_float3 vPlayerPos;
		_float3 vPointPos= m_vPoints[i];

		XMStoreFloat3(&vPlayerPos, vPosition);
		
		vPlayerPos.y = 0.f;
		vPointPos.y = 0.f;

		_vector	vDir = XMLoadFloat3(&vPlayerPos) - XMLoadFloat3(&vPointPos);
		if (DBL_EPSILON < XMVectorGetX(XMVector3Length(vDir)))
		{/* 바깥으로 낙ㅆ아어 */
			if (DBL_EPSILON < XMVectorGetX(XMVector3Dot(XMVector3Normalize(vDir), XMVector3Normalize(XMLoadFloat3(&m_vNormal[i])))))
			{

				if (-1 == m_iNeighbor[i] || iPrevIndex != m_iNeighbor[i])
				{
					*pNeighborIndex = m_iNeighbor[i];
					iPrevIndex = m_iIndex;

					return false;
				}
			}
		}
		else
			break;
	}

	
	if(DBL_EPSILON < fabs( m_vCellPlane.y))
	{
		_float4		vPos;
		XMStoreFloat4(&vPos, vPosition);
		fPositionY = (-m_vCellPlane.x * vPos.x - m_vCellPlane.z * vPos.z
		-m_vCellPlane.w) / m_vCellPlane.y;
	};
	

	return true;	
}

_bool CCell::isSamePoints(_float3 vPointA, _float3 vPointB, _float3 vPointC)
{
	_int	iCnt = 0;
	for (_int i = 0; i < 3; ++i)
	{
		_int iIndex = i;
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[iIndex]), XMLoadFloat3(&vPointA)))
		{
			++iCnt;
		}
		iIndex = (iIndex + 1) % 3;
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[iIndex]), XMLoadFloat3(&vPointB)))
		{
			++iCnt;
		}
		iIndex = (iIndex + 1) % 3;
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[iIndex]), XMLoadFloat3(&vPointC)))
		{
			++iCnt;
		}

		if (3 == iCnt)
		{
			return true;
		}
		else
			iCnt = 0;
	}
	return false;
}




#ifdef _DEBUG
HRESULT CCell::Render()
{
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pVIBuffer->Render();

	return S_OK;
}
#endif // _DEBUG

CCell * CCell::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _float3 * pPoints, _int iIndex)
{
	CCell*		pInstance = new CCell(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pPoints, iIndex)))
	{
		MSG_BOX("Failed to Created : CCell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCell::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);	

#ifdef _DEBUG
	Safe_Release(m_pVIBuffer);
#endif // _DEBUG

}
