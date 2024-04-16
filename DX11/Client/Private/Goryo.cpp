#include "stdafx.h"
#include"Goryo.h"
#include "Ghost_SpawnPoint.h"
#include "GameInstance.h"



CGoryo::CGoryo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGhost(pDevice, pContext)
{
}

CGoryo::CGoryo(const CGoryo& rhs)
	:CGhost(rhs)
{
}

HRESULT CGoryo::Initialize_Prototype()
{
	if (__super::Initialize_Prototype())
		return E_FAIL;

	return S_OK;
}

HRESULT CGoryo::Initialize(void* pArg)
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

	m_pSpawnPoint->Set_Evidence(false, true, false, true, true);


	GAMEINSTANCE->Set_GhostName(TEXT("°í·á"));

	return S_OK;
}





CGoryo* CGoryo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGoryo* pInstance = new CGoryo(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGoryo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGoryo::Clone(void* pArg)
{
	CGoryo* pInstance = new CGoryo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGoryo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGoryo::Free()
{
	__super::Free();

}
