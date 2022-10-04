#include "stdafx.h"
#include "../Public/Truck.h"
#include "GameInstance.h"

CTruck::CTruck(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice,pContext)
{
}

CTruck::CTruck(const CTruck& rhs)
    :CGameObject(rhs)
{
}

HRESULT CTruck::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTruck::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Setup_Component()))
        return E_FAIL;

    return S_OK;
}

void CTruck::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
 
}

void CTruck::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
   /* _float4 vPosition;
    XMStoreFloat4(&vPosition, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
    if (GAMEINSTANCE->CheckPoint(vPosition.x, vPosition.y, vPosition.z))
    {*/
        GAMEINSTANCE->Add_Object_For_Culling(this,CRenderer::RENDER_NONALPHABLEND);

    //}

}

HRESULT CTruck::Render()
{
    _uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
            return E_FAIL;
        /*if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
            return E_FAIL;*/

        
        m_pModelCom->Render(i, m_pShaderCom);
    }

    return S_OK;
}



HRESULT CTruck::Setup_Component()
{
    /* For.Com_Shader*/
    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Shader_VtxModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;


    /* For.Com_Renderer*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
        return E_FAIL;

    /* For.Com_Model*/
    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Truck"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;
   
    return S_OK;
}

HRESULT CTruck::SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix)
{
    if (nullptr == m_pShaderCom||
        nullptr == m_pModelCom)
        return E_FAIL;


    if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", pViewMatrix, sizeof(_float4x4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", pProjMatrix, sizeof(_float4x4))))
        return E_FAIL;


    return S_OK;
}

CTruck* CTruck::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTruck* pInstance = new CTruck(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CTruck");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTruck::Clone(void* pArg)
{
    CTruck* pInstance = new CTruck(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CTruck");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTruck::Free()
{
    __super::Free();
    
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pRendererCom);
    Safe_Release(m_pModelCom);
   
    //해당 클래스에 있는 변수들은 항상 safe_release해주기
}
