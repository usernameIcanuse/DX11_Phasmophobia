#include "stdafx.h"
#include"Wraith.h"
#include "Ghost_SpawnPoint.h"
#include "GameInstance.h"



CWraith::CWraith(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGhost(pDevice, pContext)
{
}

CWraith::CWraith(const CWraith& rhs)
	:CGhost(rhs)
{
}

HRESULT CWraith::Initialize_Prototype()
{
	if (__super::Initialize_Prototype())
		return E_FAIL;

	return S_OK;
}

HRESULT CWraith::Initialize(void* pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (nullptr != pArg)
	{
		m_pSpawnPoint = (CGhost_SpawnPoint*)pArg;
	}


	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	m_pSpawnPoint->Set_Evidence(true, true, false, false, true);


	GAMEINSTANCE->Set_GhostName(TEXT("·¹ÀÌ½º"));

	return S_OK;
}





CWraith* CWraith::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWraith* pInstance = new CWraith(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWraith");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWraith::Clone(void* pArg)
{
	CWraith* pInstance = new CWraith(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWraith");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWraith::Free()
{
	__super::Free();

}
