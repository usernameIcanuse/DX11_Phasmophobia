#include "stdafx.h"
#include "../Public/Item.h"
#include "GameInstance.h"

CItem::CItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice,pContext)
{
}

CItem::CItem(const CItem& rhs)
    :CGameObject(rhs)
{
}

HRESULT CItem::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CItem::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CItem::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
 
}

void CItem::LateTick(_float fTimeDelta)
{
   
}

HRESULT CItem::Render()
{
    return S_OK;
}


HRESULT CItem::Setup_Component()
{
    /* For.Com_Renderer*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
        return E_FAIL;

    /* For.Com_AABB*/
    CCollider::COLLIDERDESC			ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

    ColliderDesc.vScale = _float3(1.f, 2.f, 1.f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);
    ColliderDesc.pOwner = this;
    ColliderDesc.m_eObjID = COLLISION_TYPE::ITEM;

    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
        return E_FAIL;
    
    return S_OK;
}

void CItem::On_Collision_Enter(CCollider* pCollider)
{
   
}

void CItem::On_Collision_Stay(CCollider* pCollider)
{

}

void CItem::On_Collision_Exit(CCollider* pCollider)
{
}

void CItem::Free()
{
    __super::Free();
    
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pRendererCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pAABBCom);
    //해당 클래스에 있는 변수들은 항상 safe_release해주기
}
