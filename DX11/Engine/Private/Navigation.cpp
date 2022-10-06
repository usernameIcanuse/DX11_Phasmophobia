#include "..\Public\Navigation.h"
#include "Cell.h"
#include "Shader.h"
#include "PipeLine.h"

CNavigation::CNavigation(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CNavigation::CNavigation(const CNavigation & rhs)
	: CComponent(rhs)
	, m_Cells(rhs.m_Cells)
#ifdef _DEBUG
	, m_pShader(rhs.m_pShader)
#endif // _DEBUG

{
	for (auto& pCell : m_Cells)
		Safe_AddRef(pCell);

#ifdef _DEBUG
	Safe_AddRef(m_pShader);
#endif // _DEBUG
}

HRESULT CNavigation::Initialize_Prototype(const char * pNavigationData)
{
	_ulong		dwByte = 0;
	
	HANDLE		hFile = CreateFileA(pNavigationData, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	while (true)
	{
		_float3		vPoints[3];

		ReadFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

		if (0 == dwByte)
			break;

		_int	iLength = m_Cells.size();
		_bool	bFlag = false;
		for (_int i = 0; i < iLength; ++i)
		{
			if (m_Cells[i]->isSamePoints(vPoints[0], vPoints[1], vPoints[2]))
			{
				bFlag = true;
				break;
			}
		}
		if (bFlag)
			continue;

		CCell*		pCell = CCell::Create(m_pDevice, m_pContext, vPoints, m_Cells.size());
		if (nullptr == pCell)
			return E_FAIL;

		m_Cells.push_back(pCell);
	}

	CloseHandle(hFile);

	if (FAILED(Make_Neighbor()))
		return E_FAIL;


#ifdef _DEBUG
	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"), VTXPOS_DECLARATION::Element, VTXPOS_DECLARATION::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;
#endif // _DEBUG

	return S_OK;
}

HRESULT CNavigation::Initialize(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_NaviDesc, pArg, sizeof(NAVIDESC));

	return S_OK;
}

/* 객체가 움직이는데 있어 네비게이션 상에서 움직일 수 있는가? 아닌가? : return  */
/* vPosition : 객체의 현재위치?!(x), 객체가 움직이고 난 이후의 위치. */
_bool CNavigation::isMove(_fvector vPosition, _float fPositionY, _vector vDirection, _vector& vMovedPosition)
{
	/* m_NaviDesc.m_iCurrentIndex : 현재 객체가 존재하는 쎌의 인덱스. */
	_int		iNeighborIndex = -1;
	_int		iPrevIndex = -1;
	_vector		vSlideDirection;


	if(m_Cells.empty())
		return true;

	/*1. 현재 존재하는 셀 안에서만 움직였을때  */
	if (true == m_Cells[m_NaviDesc.m_iCurrentIndex]->isIn(vPosition + vDirection, &iNeighborIndex, fPositionY, iPrevIndex, vSlideDirection))
	{
		vMovedPosition = vPosition + vDirection;
		return true;
	}
	/* 현재 존재하고 있는 쎌 바깥으로 나갔다. */
	else
	{		
		/*2. 나간쪽 쎌에 이웃이 존재할때 */
		if(0 <= iNeighborIndex)
		{
			
			while (1)
			{
				
				if (true == m_Cells[iNeighborIndex]->isIn(vPosition+vDirection, &iNeighborIndex, fPositionY, iPrevIndex, vSlideDirection))
					break;

				if (0 > iNeighborIndex)
					return false;

			}

			m_NaviDesc.m_iCurrentIndex = iNeighborIndex;
			vMovedPosition = vPosition + vDirection;

			return true;
		}

		/*1. 나간쪽에 이웃이 없다면.  슬라이딩 현재 셀 안에서만 타야함*/
		else
		{
			vSlideDirection = XMVector3Normalize(vSlideDirection);
			_vector vSlidePos = vPosition + vSlideDirection * XMVector3Dot(vSlideDirection, vDirection);
			if (true == m_Cells[m_NaviDesc.m_iCurrentIndex]->isIn(vSlidePos, &iNeighborIndex, fPositionY, iPrevIndex, vSlideDirection))
			{//내 셀 안에서 슬라이드 탈 때
				vMovedPosition = vSlidePos;
				return true;
			}
			else
			{
				if (0 <= iNeighborIndex)
				{

					while (1)
					{

						if (true == m_Cells[iNeighborIndex]->isIn(vSlidePos, &iNeighborIndex, fPositionY, iPrevIndex, vSlideDirection))
							break;

						if (0 > iNeighborIndex)
							return false;

					}

					m_NaviDesc.m_iCurrentIndex = iNeighborIndex;
					vMovedPosition = vSlidePos;

					return true;
				}
			}
			return false;
		}
	}
}

#ifdef _DEBUG

HRESULT CNavigation::Add_Cell(_float3 vPointA, _float3 vPointB, _float3 vPointC)
{
	_float3 vPoints[3] = { vPointA,vPointB,vPointC };

	CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, m_Cells.size());
	if (nullptr == pCell)
		return E_FAIL;

	m_Cells.push_back(pCell);

	for (auto& elem : m_Cells)
	{
		if (elem == pCell)
			continue;

		if (true == elem->Compare_Points(
			pCell->Get_Point(CCell::POINT_A),
			pCell->Get_Point(CCell::POINT_B),pCell->Get_Index()))
		{
			pCell->Set_Neighbor(CCell::LINE_AB, elem->Get_Index());
			continue;
		}

		if (true == elem->Compare_Points(
			pCell->Get_Point(CCell::POINT_B),
			pCell->Get_Point(CCell::POINT_C),pCell->Get_Index()))
		{
			pCell->Set_Neighbor(CCell::LINE_BC, elem->Get_Index());
			continue;
		}

		if (true == elem->Compare_Points(
			pCell->Get_Point(CCell::POINT_C),
			pCell->Get_Point(CCell::POINT_A), pCell->Get_Index()))
		{
			pCell->Set_Neighbor(CCell::LINE_CA, elem->Get_Index());
			continue;
		}
	}
	
}

HRESULT CNavigation::Render()
{
	if (nullptr == m_pShader)
		return E_FAIL;

	_float4x4		WorldMatrix;


	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);


	m_pShader->Set_RawValue("g_ViewMatrix", pPipeLine->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrix", pPipeLine->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));

	RELEASE_INSTANCE(CPipeLine);

	if (m_Cells.empty())
		return S_OK;

	if (0 <= m_NaviDesc.m_iCurrentIndex)
	{

		XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(XMMatrixTranslation(0.f, 0.1f, 0.f)));		
		m_pShader->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4));
		m_pShader->Set_RawValue("g_vColor", &_float4(1.f, 0.f ,0.f, 1.f), sizeof(_float4));
		m_pShader->Begin(0);

		m_Cells[m_NaviDesc.m_iCurrentIndex]->Render();
	}
	
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());
	//XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(XMMatrixTranslation(0.f, 0.1f, 0.f)));

	m_pShader->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_vColor", &_float4(0.f, 1.f, 0.f, 1.f), sizeof(_float4));
	m_pShader->Begin(0);

	for (auto& pCell : m_Cells)
	{
		if (nullptr != pCell)
			pCell->Render();
	}



	

	return S_OK;
}
#endif // _DEBUG

HRESULT CNavigation::Make_Neighbor()
{
	for (auto& pSourCell : m_Cells)
	{
		for (auto& pDestCell : m_Cells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->Compare_Points(
				pSourCell->Get_Point(CCell::POINT_A), 
				pSourCell->Get_Point(CCell::POINT_B)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_AB, pDestCell->Get_Index());
				continue;
			}

			if (true == pDestCell->Compare_Points(
				pSourCell->Get_Point(CCell::POINT_B),
				pSourCell->Get_Point(CCell::POINT_C)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_BC, pDestCell->Get_Index());
				continue;
			}

			if (true == pDestCell->Compare_Points(
				pSourCell->Get_Point(CCell::POINT_C),
				pSourCell->Get_Point(CCell::POINT_A)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_CA, pDestCell->Get_Index());
				continue;
			}
		}
	}


	return S_OK;
}

CNavigation * CNavigation::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const char* pNavigationData)
{
	CNavigation*		pInstance = new CNavigation(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pNavigationData)))
	{
		MSG_BOX("Failed to Created : CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CNavigation::Clone(void * pArg)
{
	CNavigation*		pInstance = new CNavigation(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);

	m_Cells.clear();

#ifdef _DEBUG
	Safe_Release(m_pShader);
#endif // _DEBUG

}
