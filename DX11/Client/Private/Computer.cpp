#include "stdafx.h"
#include "../Public/Computer.h"
#include "GameInstance.h"

CComputer::CComputer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice,pContext)
{
}

CComputer::CComputer(const CComputer& rhs)
    :CGameObject(rhs)
{
}

HRESULT CComputer::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CComputer::Initialize(void* pArg)
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

void CComputer::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
 
}

void CComputer::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
   /* _float4 vPosition;
    XMStoreFloat4(&vPosition, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
    if (GAMEINSTANCE->CheckPoint(vPosition.x, vPosition.y, vPosition.z))
    {*/
        GAMEINSTANCE->Add_Object_For_Culling(this,CRenderer::RENDER_NONALPHABLEND);

    //}

}

HRESULT CComputer::Render()
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



HRESULT CComputer::Setup_Component()
{
    /* For.Com_Shader*/
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    /* For.Com_Model*/
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Computer"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;
   
    return S_OK;
}

HRESULT CComputer::SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix)
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

CComputer* CComputer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CComputer* pInstance = new CComputer(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CComputer");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CComputer::Clone(void* pArg)
{
    CComputer* pInstance = new CComputer(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CComputer");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CComputer::Free()
{
    __super::Free();
    
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pOBBCom);
    Safe_Release(m_pModelCom);
   
    //해당 클래스에 있는 변수들은 항상 safe_release해주기
}
