#include "stdafx.h"
#include "../Public/FlashLight.h"
#include "GameInstance.h"

CFlashLight::CFlashLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CItem(pDevice,pContext)
{
}

CFlashLight::CFlashLight(const CFlashLight& rhs)
    :CItem(rhs)
{
}

HRESULT CFlashLight::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CFlashLight::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Setup_Component()))
        return E_FAIL;

    return S_OK;
}

void CFlashLight::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CFlashLight::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
#ifdef _DEBUG
    m_pRendererCom->Add_DebugRenderGroup(m_pOBBCom);
#endif
}

HRESULT CFlashLight::Render()
{
    if (nullptr == m_pShaderCom ||
        nullptr == m_pModelCom)
        return E_FAIL;

    /* 셰이더 전역변수에 값을 던진다. */
    if (FAILED(SetUp_ShaderResource()))
        return E_FAIL;




    _uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
            return E_FAIL;
       /* if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
            return E_FAIL;*/

        m_pShaderCom->Begin(0);

        m_pModelCom->Render(i, m_pShaderCom);
    }
//
//#ifdef _DEBUG
//    m_pOBBCom->Render();
//#endif // _DEBUG


    return S_OK;
}


void CFlashLight::On_Collision_Enter(CCollider* pCollider)
{
    __super::On_Collision_Enter(pCollider);
}

void CFlashLight::On_Collision_Stay(CCollider* pCollider)
{
}

void CFlashLight::On_Collision_Exit(CCollider* pCollider)
{
}

HRESULT CFlashLight::Setup_Component()
{
    if (FAILED(__super::Setup_Component()))
        return E_FAIL;
        

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Model_FlashLight"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;

    /* For.Com_OBB*/
    CCollider::COLLIDERDESC			ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

    ColliderDesc.vScale = _float3(0.7f, 0.7f, 1.4f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y*0.5f, 0.f);
    ColliderDesc.pOwner = this;
    ColliderDesc.m_eObjID = COLLISION_TYPE::ITEM;

    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
        return E_FAIL;

    return S_OK;
}



CFlashLight* CFlashLight::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CFlashLight* pInstance = new CFlashLight(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CFlashLight");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CFlashLight::Clone(void* pArg)
{
    CFlashLight* pInstance = new CFlashLight(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CFlashLight");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CFlashLight::Free()
{
    __super::Free();
}
