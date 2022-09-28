#include "stdafx.h"
#include "Ghost.h"
#include "GameInstance.h"
#include "Ghost_SpawnPoint.h"

CGhost::CGhost(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice,pContext)
{
}

CGhost::CGhost(const CGhost& rhs)
	:CGameObject(rhs)
{
}

HRESULT CGhost::Initialize_Prototype()
{
	if (__super::Initialize_Prototype())
		return E_FAIL;

	return S_OK;
}

HRESULT CGhost::Initialize(void* pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	
	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(Setup_Component()))
		return E_FAIL;

	if (FAILED(Setup_SpawnPoint()))
		return E_FAIL;

	GAMEINSTANCE->Add_EventObject(CGame_Manager::EVENT_GHOST, this);
	GAMEINSTANCE->Broadcast_Message(CGame_Manager::EVENT_GHOST, TEXT("Moving"));

	return S_OK;
}

void CGhost::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fTime += fTimeDelta;

	_matrix matWorld = m_pTransformCom->Get_WorldMatrix();

	m_pOBBCom->Update(matWorld);
	m_pSphereCom->Update(matWorld);


}

void CGhost::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

#ifdef _DEBUG
	m_pRendererCom->Add_DebugRenderGroup(m_pOBBCom);
	m_pRendererCom->Add_DebugRenderGroup(m_pSphereCom);


#endif // _DEBUG
}

HRESULT CGhost::Render()
{
//#ifdef _DEBUG
//	m_pOBBCom->Render();
//	m_pSphereCom->Render();
//
//
//#endif // _DEBUG

	return S_OK;
}

void CGhost::Set_Enable(_bool _bEnable)
{
	__super::Set_Enable(_bEnable);

	m_pSpawnPoint->Set_Enable(_bEnable);
}

void CGhost::OnEventMessage(const _tchar* pMessage)
{
	if (0 == lstrcmp(TEXT("Event"), pMessage))
	{
		m_EventFunc = std::bind(&CGhost::Light_Attack, std::placeholders::_1, std::placeholders::_2);
	}

	else if (0 == lstrcmp(TEXT("Attack"), pMessage))
	{
		m_EventFunc = std::bind(&CGhost::Attack, std::placeholders::_1, std::placeholders::_2);
	}

	else if (0 == lstrcmp(TEXT("Moving"), pMessage))
	{
		m_EventFunc = std::bind(&CGhost::Moving, std::placeholders::_1, std::placeholders::_2);
	}

}

void CGhost::Call_EventFunc(_float fTimeDelta)
{
	m_EventFunc(this,fTimeDelta);
}

void CGhost::Whispering()
{
	
}

void CGhost::Stop_Updating_SpawnPoint()
{
	m_pSpawnPoint->m_pGhost = nullptr;
}

void CGhost::Light_Attack(_float fTimeDelta)
{
	/*ºÒºû ±ôºý°Å¸², ±Í½Å ¸ðµ¨ ·»´õ¸µ, ÀüÀÚ Àåºñµé °íÀå*/
}

void CGhost::Attack(_float fTimeDelta)
{
	/*Ãâ±¸ ´ÝÈû&Àá±è, ±Í½Å attack collider set enable, ÀüÀÚ Àåºñµé °íÀå*/
}

void CGhost::Moving(_float fTimeDelta)
{
	//m_pTransformCom->Go_Backward(fTimeDelta);

	//if (m_fTime > 5.f)
	//{
	//	GAMEINSTANCE->Broadcast_Message(CGame_Manager::EVENT_GHOST, TEXT("Stop"));
	//}
}


HRESULT CGhost::Setup_Component()
{
	/* For.Com_OBB*/
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(4.f, 10.f, 4.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);
	ColliderDesc.pOwner = this;
	ColliderDesc.m_eObjID = COLLISION_TYPE::GHOST_ATTACK;

	if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Sphere*/
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(10.f, 10.f, 10.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);
	ColliderDesc.pOwner = this;
	ColliderDesc.m_eObjID = COLLISION_TYPE::GHOST;

	if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_Sphere"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Renderer*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGhost::Setup_SpawnPoint()
{
	if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Ghost_SpawnPoint"), TEXT("Prototype_GameObject_Ghost_SpawnPoint"), (CGameObject**)&m_pSpawnPoint, this)))
		return E_FAIL;
	return S_OK;
}

void CGhost::On_Collision_Enter(CCollider* pCollider)
{
	
}

void CGhost::On_Collision_Stay(CCollider* pCollider)
{
	
}

void CGhost::On_Collision_Exit(CCollider* pCollider)
{
}

CGhost* CGhost::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGhost* pInstance = new CGhost(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGhost");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGhost::Clone(void* pArg)
{
	CGhost* pInstance = new CGhost(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGhost");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhost::Free()
{
	__super::Free();

	Safe_Release(m_pOBBCom);
	Safe_Release(m_pSphereCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}
