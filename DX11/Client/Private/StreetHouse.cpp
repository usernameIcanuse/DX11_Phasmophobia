#include "stdafx.h"
#include "../Public/StreetHouse.h"
#include "GameInstance.h"

CStreetHouse::CStreetHouse(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CHouse(pDevice,pContext)
{
}

CStreetHouse::CStreetHouse(const CStreetHouse& rhs)
    :CHouse(rhs)
{
}

HRESULT CStreetHouse::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Setup_Component()))
        return E_FAIL;


    return S_OK;
}

HRESULT CStreetHouse::Setup_Component()
{
    if (FAILED(__super::Setup_Component()))
        return E_FAIL;

    /* For.Com_Model*/
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_StreetHouse"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;

    /* For.Com_OBB*/
    CCollider::COLLIDERDESC  ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

    ColliderDesc.vScale = _float3(15.f, 10.f, 10.f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(-ColliderDesc.vScale.x*0.1f, 0.f, -ColliderDesc.vScale.z * 0.5f);
    ColliderDesc.pOwner = this;
    ColliderDesc.m_eObjID = COLLISION_TYPE::HOUSE;
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom,&ColliderDesc)))
        return E_FAIL;
   
    return S_OK;
}


CStreetHouse* CStreetHouse::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CStreetHouse* pInstance = new CStreetHouse(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CStreetHouse");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CStreetHouse::Clone(void* pArg)
{
    CStreetHouse* pInstance = new CStreetHouse(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CStreetHouse");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CStreetHouse::Free()
{
    __super::Free();

}
