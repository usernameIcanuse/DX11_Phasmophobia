#include "stdafx.h"
#include"Phantom.h"
#include "Ghost_SpawnPoint.h"
#include "GameInstance.h"



CPhantom::CPhantom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGhost(pDevice, pContext)
{
}

CPhantom::CPhantom(const CPhantom& rhs)
	:CGhost(rhs)
{
}

HRESULT CPhantom::Initialize_Prototype()
{
	if (__super::Initialize_Prototype())
		return E_FAIL;

	return S_OK;
}

HRESULT CPhantom::Initialize(void* pArg)
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

	m_pSpawnPoint->Set_Evidence(true, true, false, true, false);


	GAMEINSTANCE->Set_GhostName(TEXT("ฦาลา"));

	return S_OK;
}





CPhantom* CPhantom::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPhantom* pInstance = new CPhantom(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPhantom");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPhantom::Clone(void* pArg)
{
	CPhantom* pInstance = new CPhantom(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPhantom");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPhantom::Free()
{
	__super::Free();

}
