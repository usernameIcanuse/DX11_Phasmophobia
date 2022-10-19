#include "stdafx.h"
#include "../Public/Lobby.h"
#include "GameInstance.h"

CLobby::CLobby(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice,pContext)
{
}

CLobby::CLobby(const CLobby& rhs)
    :CGameObject(rhs)
{
}

HRESULT CLobby::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CLobby::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(nullptr)))
        return E_FAIL;

    if (nullptr != pArg)
    {
        m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4((_float4x4*)pArg));
    }

    if (FAILED(Setup_Component()))
        return E_FAIL;

   

    return S_OK;
}

void CLobby::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

 
}

void CLobby::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CLobby::Render()
{
    _uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        _int        iPassIndex = 2;

        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
            continue;
        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
        {
            iPassIndex = 4;
            if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_EmissiveTexture", i, aiTextureType_EMISSIVE)))
            {
                iPassIndex = 0;
            }
        }


        m_pModelCom->Render(i, m_pShaderCom, iPassIndex);
    }

    return S_OK;
}


HRESULT CLobby::Setup_Component()
{
    /* For.Com_Shader*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;


    /* For.Com_Renderer*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
        return E_FAIL;

    /* For.Com_Model*/
    if (FAILED(__super::Add_Component(LEVEL_LOBBY, TEXT("Prototype_Component_Model_Lobby"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;


    return S_OK;
}

void CLobby::On_Collision_Enter(CCollider* pCollider)
{

}

void CLobby::On_Collision_Stay(CCollider* pCollider)
{
}

void CLobby::On_Collision_Exit(CCollider* pCollider)
{
}

HRESULT CLobby::SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix)
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

CLobby* CLobby::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CLobby* pInstance = new CLobby(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CLobby");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CLobby::Clone(void* pArg)
{
    CLobby* pInstance = new CLobby(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CLobby");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLobby::Free()
{
    __super::Free();
    
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pRendererCom);
    Safe_Release(m_pModelCom);

   
    //해당 클래스에 있는 변수들은 항상 safe_release해주기
}
