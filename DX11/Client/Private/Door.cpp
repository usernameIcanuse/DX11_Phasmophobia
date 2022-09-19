#include "stdafx.h"
#include "../Public/Door.h"
#include "GameInstance.h"

CDoor::CDoor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice,pContext)
{
}

CDoor::CDoor(const CDoor& rhs)
    :CGameObject(rhs)
{
}

HRESULT CDoor::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CDoor::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Setup_Component()))
        return E_FAIL;

    return S_OK;
}

void CDoor::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
 
}

void CDoor::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
   /* _float4 vPosition;
    XMStoreFloat4(&vPosition, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
    if (GAMEINSTANCE->CheckPoint(vPosition.x, vPosition.y, vPosition.z))
    {*/
        m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

    //}

}

HRESULT CDoor::Render()
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
        /*if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
            return E_FAIL;*/

        m_pShaderCom->Begin(0);

        m_pModelCom->Render(i);
    }

    return S_OK;
}


HRESULT CDoor::Setup_Component()
{
    /* For.Com_Shader*/
    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Shader_VtxModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    /* For.Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Sky"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    /* For.Com_Renderer*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
        return E_FAIL;
   
    return S_OK;
}

HRESULT CDoor::SetUp_ShaderResource()
{
    if (nullptr == m_pShaderCom)
        return E_FAIL;


    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
        return E_FAIL;


    LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);

    if (nullptr == pLightDesc)
        return E_FAIL;

    if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPosition(), sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Set_RawValue("g_vLightPos", &pLightDesc->vPosition, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Set_RawValue("g_fRange", &pLightDesc->fRange, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Set_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Set_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
        return E_FAIL;


    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

CDoor* CDoor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CDoor* pInstance = new CDoor(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CDoor");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CDoor::Clone(void* pArg)
{
    CDoor* pInstance = new CDoor(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CDoor");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CDoor::Free()
{
    __super::Free();
    
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pRendererCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pModelCom);
   
    //해당 클래스에 있는 변수들은 항상 safe_release해주기
}
