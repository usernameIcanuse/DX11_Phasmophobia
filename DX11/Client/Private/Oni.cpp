#include "stdafx.h"
#include"Oni.h"
#include "Ghost_SpawnPoint.h"
#include "GameInstance.h"



COni::COni(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGhost(pDevice, pContext)
{
}

COni::COni(const COni& rhs)
	:CGhost(rhs)
{
}

HRESULT COni::Initialize_Prototype()
{
	if (__super::Initialize_Prototype())
		return E_FAIL;

	return S_OK;
}

HRESULT COni::Initialize(void* pArg)
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

	m_pSpawnPoint->Set_Evidence(false, true, true, false, true);


	GAMEINSTANCE->Set_GhostName(TEXT("¿À´Ï"));

	return S_OK;
}





COni* COni::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	COni* pInstance = new COni(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : COni");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* COni::Clone(void* pArg)
{
	COni* pInstance = new COni(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : COni");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void COni::Free()
{
	__super::Free();

}
