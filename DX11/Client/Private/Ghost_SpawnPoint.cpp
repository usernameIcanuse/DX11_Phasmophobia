#include "stdafx.h"
#include "Ghost_SpawnPoint.h"
#include "GameInstance.h"
#include "Ghost.h"

CGhost_SpawnPoint::CGhost_SpawnPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice,pContext)
{
}

CGhost_SpawnPoint::CGhost_SpawnPoint(const CGhost_SpawnPoint& rhs)
	:CGameObject(rhs)
{
}

HRESULT CGhost_SpawnPoint::Initialize_Prototype()
{
	if (__super::Initialize_Prototype())
		return E_FAIL;

	return S_OK;
}

HRESULT CGhost_SpawnPoint::Initialize(void* pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (pArg)
		m_pGhost = (CGhost*)pArg;

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(Setup_Component()))
		return E_FAIL;

	return S_OK;
}

void CGhost_SpawnPoint::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


}

void CGhost_SpawnPoint::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CGhost_SpawnPoint::Render()
{
	return S_OK;
}

HRESULT CGhost_SpawnPoint::Setup_Component()
{
	

	return S_OK;
}



void CGhost_SpawnPoint::On_Collision_Enter(CCollider* pCollider)
{
	
}

void CGhost_SpawnPoint::On_Collision_Stay(CCollider* pCollider)
{
	
}

void CGhost_SpawnPoint::On_Collision_Exit(CCollider* pCollider)
{
}

CGhost_SpawnPoint* CGhost_SpawnPoint::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGhost_SpawnPoint* pInstance = new CGhost_SpawnPoint(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGhost_SpawnPoint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGhost_SpawnPoint::Clone(void* pArg)
{
	CGhost_SpawnPoint* pInstance = new CGhost_SpawnPoint(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGhost_SpawnPoint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhost_SpawnPoint::Free()
{
	__super::Free();

}
