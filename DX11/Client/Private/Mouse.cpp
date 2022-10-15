#include "stdafx.h"
#include "../Public/Mouse.h"
#include "GameInstance.h"

CMouse::CMouse(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice,pContext)
{
}

CMouse::CMouse(const CMouse& rhs)
    :CGameObject(rhs)
{
}

HRESULT CMouse::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMouse::Initialize(void* pArg)
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

void CMouse::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
 
}

void CMouse::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
   /* _float4 vPosition;
    XMStoreFloat4(&vPosition, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
    if (GAMEINSTANCE->CheckPoint(vPosition.x, vPosition.y, vPosition.z))
    {*/
        GAMEINSTANCE->Add_Object_For_Culling(this,CRenderer::RENDER_NONALPHABLEND);

    //}

}

HRESULT CMouse::Render()
{
    _uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        _int    iPassIndex = 2;
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



HRESULT CMouse::Setup_Component()
{
    /* For.Com_Shader*/
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    /* For.Com_Model*/
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Mouse"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;
   
    return S_OK;
}

HRESULT CMouse::SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix)
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

CMouse* CMouse::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMouse* pInstance = new CMouse(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CMouse");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMouse::Clone(void* pArg)
{
    CMouse* pInstance = new CMouse(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CMouse");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMouse::Free()
{
    __super::Free();
    
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pOBBCom);
    Safe_Release(m_pModelCom);
   
    //해당 클래스에 있는 변수들은 항상 safe_release해주기
}
