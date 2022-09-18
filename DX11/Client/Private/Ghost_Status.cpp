#include "stdafx.h"
#include "Ghost_Status.h"
#include "GameInstance.h"

CGhost_Status::CGhost_Status(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice,pContext)
{
}

CGhost_Status::CGhost_Status(const CGhost_Status& rhs)
	:CGameObject(rhs)
{
}

HRESULT CGhost_Status::Initialize_Prototype()
{
	if (__super::Initialize_Prototype())
		return E_FAIL;

	return S_OK;
}

HRESULT CGhost_Status::Initialize(void* pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	
	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	return S_OK;
}

void CGhost_Status::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CGhost_Status::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

HRESULT CGhost_Status::Render()
{

	return S_OK;
}

CGhost_Status* CGhost_Status::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGhost_Status* pInstance = new CGhost_Status(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGhost_Status");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGhost_Status::Clone(void* pArg)
{
	CGhost_Status* pInstance = new CGhost_Status(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGhost_Status");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhost_Status::Free()
{
	__super::Free();

}
