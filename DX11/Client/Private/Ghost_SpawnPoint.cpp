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

	m_iAreaDefaultTemperature = rand() % 7+3;

	return S_OK;
}

void CGhost_SpawnPoint::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	if (nullptr != m_pGhost)
	{
		CTransform* matGhostTransform = (CTransform*)m_pGhost->Get_Component(CGameObject::m_pTransformTag);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, matGhostTransform->Get_State(CTransform::STATE_TRANSLATION));
	}

	_matrix matWorld = m_pTransformCom->Get_WorldMatrix();
	m_pAreaCom->Update(matWorld);
	m_pSpawnPointCom->Update(matWorld);

	m_iAreaTemperature = m_iAreaDefaultTemperature - rand() % 4;


}

void CGhost_SpawnPoint::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef _DEBUG
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
#endif
}

HRESULT CGhost_SpawnPoint::Render()
{
#ifdef _DEBUG

	m_pAreaCom->Render();
	m_pSpawnPointCom->Render();

#endif // _DEBUG

	return S_OK;
}

HRESULT CGhost_SpawnPoint::Setup_Component()
{
	/* For.Com_Sphere*/
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(40.f, 40.f, 40.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.pOwner = this;
	ColliderDesc.m_eObjID = COLLISION_TYPE::GHOST_AREA;

	if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_Area"), (CComponent**)&m_pAreaCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Sphere*/
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(15.f, 15.f, 15.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);
	ColliderDesc.pOwner = this;
	ColliderDesc.m_eObjID = COLLISION_TYPE::GHOST_SPAWNPOINT;

	if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SpawnPoint"), (CComponent**)&m_pSpawnPointCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Renderer*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

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

	Safe_Release(m_pSpawnPointCom);
	Safe_Release(m_pAreaCom);
	Safe_Release(m_pRendererCom);

}
