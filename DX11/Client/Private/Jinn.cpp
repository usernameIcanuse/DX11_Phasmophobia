#include "stdafx.h"
#include"Jinn.h"
#include "Ghost_SpawnPoint.h"
#include "GameInstance.h"



CJinn::CJinn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGhost(pDevice,pContext)
{
}

CJinn::CJinn(const CJinn& rhs)
	:CGhost(rhs)
{
}

HRESULT CJinn::Initialize_Prototype()
{
	if (__super::Initialize_Prototype())
		return E_FAIL;

	return S_OK;
}

HRESULT CJinn::Initialize(void* pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 15.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (nullptr != pArg)
	{
		m_pSpawnPoint = (CGhost_SpawnPoint*)pArg;
	}

	
	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	m_pSpawnPoint->Set_Evidence(false, false, true, true,true);
	

	GAMEINSTANCE->Set_GhostName(TEXT("Áø"));

	return S_OK;
}





CJinn* CJinn::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CJinn* pInstance = new CJinn(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CJinn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CJinn::Clone(void* pArg)
{
	CJinn* pInstance = new CJinn(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CJinn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CJinn::Free()
{
	__super::Free();

}
