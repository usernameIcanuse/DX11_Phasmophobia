#include "stdafx.h"
#include "Camera_Fixed.h"
#include "GameInstance.h"
CCamera_Fixed::CCamera_Fixed(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCamera(pDevice,pContext)
{
}

CCamera_Fixed::CCamera_Fixed(const CCamera_Fixed& rhs)
	:CCamera(rhs)
{
}

HRESULT CCamera_Fixed::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Fixed::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Fixed::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	_matrix WorldMat = m_pTarget->Get_WorldMatrix();

	_vector vLookPos = WorldMat.r[3];

	WorldMat.r[0] = XMVector3Normalize(WorldMat.r[0]);
	WorldMat.r[1] = XMVector3Normalize(WorldMat.r[1]);
	WorldMat.r[2] = XMVector3Normalize(WorldMat.r[2]);
	WorldMat.r[3] = WorldMat.r[3] + WorldMat.r[2]*10.f;

	m_pTransformCom->Set_WorldMatrix(WorldMat);
	m_pTransformCom->LookAt(vLookPos);
}

void CCamera_Fixed::Set_Target(CTransform* _pTarget)
{
	m_pTarget = _pTarget;

	_matrix WorldMat = m_pTarget->Get_WorldMatrix();

	_vector vLookPos = WorldMat.r[3];

	WorldMat.r[0] = XMVector3Normalize(WorldMat.r[0]);
	WorldMat.r[1] = XMVector3Normalize(WorldMat.r[1]);
	WorldMat.r[2] = XMVector3Normalize(WorldMat.r[2]);
	WorldMat.r[3] = WorldMat.r[3] + WorldMat.r[2]*10.f;

	m_pTransformCom->Set_WorldMatrix(WorldMat);
	m_pTransformCom->LookAt(vLookPos);


}



CCamera_Fixed* CCamera_Fixed::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Fixed* pInstance = new CCamera_Fixed(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Fixed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Fixed::Clone(void* pArg)
{
	CCamera_Fixed* pInstance = new CCamera_Fixed(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Fixed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Fixed::Free()
{
	__super::Free();
}
