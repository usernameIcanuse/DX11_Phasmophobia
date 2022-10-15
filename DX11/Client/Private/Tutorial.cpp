#include "stdafx.h"
#include "../Public/Tutorial.h"
#include "GameInstance.h"

CTutorial::CTutorial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CHouse(pDevice,pContext)
{
}

CTutorial::CTutorial(const CTutorial& rhs)
    :CHouse(rhs)
{
}

HRESULT CTutorial::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Setup_Component()))
        return E_FAIL;


    return S_OK;
}

HRESULT CTutorial::Setup_Component()
{
    if (FAILED(__super::Setup_Component()))
        return E_FAIL;

    /* For.Com_Model*/
    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Model_FurnishedCabin"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;

    /* For.Com_OBB*/
    CCollider::COLLIDERDESC  ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

    ColliderDesc.vScale = _float3(11.f, 5.f, 16.f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y*0.5f, -ColliderDesc.vScale.z * 0.16f);
    ColliderDesc.pOwner = this;
    ColliderDesc.m_eObjID = COLLISION_TYPE::HOUSE;
    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom,&ColliderDesc)))
        return E_FAIL;
   
    return S_OK;
}


CTutorial* CTutorial::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTutorial* pInstance = new CTutorial(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CTutorial");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTutorial::Clone(void* pArg)
{
    CTutorial* pInstance = new CTutorial(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CTutorial");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTutorial::Free()
{
    __super::Free();

}
