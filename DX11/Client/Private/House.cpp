#include "stdafx.h"
#include "../Public/House.h"
#include "GameInstance.h"

CHouse::CHouse(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice,pContext)
{
}

CHouse::CHouse(const CHouse& rhs)
    :CGameObject(rhs)
{
}

HRESULT CHouse::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CHouse::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(nullptr)))
        return E_FAIL;

    if (nullptr != pArg)
    {
        m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4((_float4x4*)pArg));
    }

    m_fCullingRange = 100.f;
   

    return S_OK;
}

void CHouse::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
    m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
 
}

void CHouse::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    GAMEINSTANCE->Add_Object_For_Culling( this, CRenderer::RENDER_NONALPHABLEND);

#ifdef _DEBUG
    m_pRendererCom->Add_DebugRenderGroup(m_pOBBCom);
#endif

}

HRESULT CHouse::Render()
{
    _uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        _int        iPassIndex = 2;

        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
            return E_FAIL;
        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
        {
            iPassIndex = 0;
        }

        m_pModelCom->Render(i, m_pShaderCom, iPassIndex);
    }

    return S_OK;
}

HRESULT CHouse::SetUp_ModelCom(const _tchar* pPrototypeTag)
{
    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, pPrototypeTag, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;

    return S_OK;
}


HRESULT CHouse::Setup_Component()
{
    /* For.Com_Shader*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

#ifdef _DEBUG
    /* For.Com_Renderer*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
        return E_FAIL;
#endif
    return S_OK;
}

void CHouse::On_Collision_Enter(CCollider* pCollider)
{

}

void CHouse::On_Collision_Stay(CCollider* pCollider)
{
}

void CHouse::On_Collision_Exit(CCollider* pCollider)
{
}

HRESULT CHouse::SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix)
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

CHouse* CHouse::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CHouse* pInstance = new CHouse(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CHouse");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CHouse::Clone(void* pArg)
{
    CHouse* pInstance = new CHouse(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CHouse");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CHouse::Free()
{
    __super::Free();
    
    Safe_Release(m_pShaderCom);

#ifdef _DEBUG
    Safe_Release(m_pRendererCom);
#endif
    Safe_Release(m_pModelCom);
    Safe_Release(m_pOBBCom);
   
    //해당 클래스에 있는 변수들은 항상 safe_release해주기
}
