#include "stdafx.h"
#include "../Public/Truck_Inside.h"
#include "GameInstance.h"

CTruck_Inside::CTruck_Inside(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice,pContext)
{
}

CTruck_Inside::CTruck_Inside(const CTruck_Inside& rhs)
    :CGameObject(rhs)
{
}

HRESULT CTruck_Inside::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTruck_Inside::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(nullptr)))
        return E_FAIL;

    if (nullptr != pArg)
    {
        m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4((_float4x4*)pArg));
    }

    m_fCullingRange = 15.f;

    if (FAILED(Setup_Component()))
        return E_FAIL;

    if (FAILED(Setup_Light()))
        return E_FAIL;

    return S_OK;
}

void CTruck_Inside::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
 
    
}

void CTruck_Inside::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    GAMEINSTANCE->Add_Object_For_Culling(this,CRenderer::RENDER_NONALPHABLEND);

    for (auto& elem : m_vecLight)
        GAMEINSTANCE->Add_Light(elem);
}

HRESULT CTruck_Inside::Render()
{
    _uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        _int    iPassIndex = 2;
        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
            return E_FAIL;
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



HRESULT CTruck_Inside::Setup_Component()
{
    /* For.Com_Shader*/
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    /* For.Com_Model*/
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Truck_Inside"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;


   
    return S_OK;
}

HRESULT CTruck_Inside::Setup_Light()
{
    LIGHTDESC LightDesc;
    LightDesc.eType = LIGHTDESC::TYPE_POINT;
    LightDesc.vDiffuse = _float4(1.f, 1.f, 0.95f, 1.f);
    LightDesc.vAmbient = _float4(0.9f, 0.9f, 0.9f, 1.f);
    LightDesc.vDiffuse = _float4(1.f, 1.f, 0.95f, 1.f);

    XMStoreFloat4(&LightDesc.vPosition, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) + XMVectorSet(0.f, 20.f, 0.f, 0.f));

    LightDesc.fRange = 65.f;
    LightDesc.fAttenuation0 = 1.f;
    LightDesc.fAttenuation1 = 0.08f;
    LightDesc.fAttenuation2 = 0.02f;

    CLight* pLight = CLight::Create(m_pDevice, m_pContext, LightDesc);
    if (nullptr == pLight)
        return E_FAIL;

    m_vecLight.push_back(pLight);

    LightDesc.eType = LIGHTDESC::TYPE_POINT;
    LightDesc.vDiffuse = _float4(1.f, 1.f, 0.95f, 1.f);
    LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
    LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);

    _vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
    XMStoreFloat4(&LightDesc.vPosition, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) + XMVectorSet(0.f, 20.f, 0.f, 0.f)- vLook*1.5f);

    LightDesc.fRange = 160.f;
    LightDesc.fAttenuation0 = 1.f;
    LightDesc.fAttenuation1 = 0.022f;
    LightDesc.fAttenuation2 = 0.0019f;

    pLight = CLight::Create(m_pDevice, m_pContext, LightDesc);
    if (nullptr == pLight)
        return E_FAIL;

    m_vecLight.push_back(pLight);

    return S_OK;
}

HRESULT CTruck_Inside::SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix)
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

CTruck_Inside* CTruck_Inside::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTruck_Inside* pInstance = new CTruck_Inside(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CTruck_Inside");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTruck_Inside::Clone(void* pArg)
{
    CTruck_Inside* pInstance = new CTruck_Inside(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CTruck_Inside");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTruck_Inside::Free()
{
    __super::Free();
    
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pOBBCom);
    Safe_Release(m_pModelCom);
   
    for (auto& elem : m_vecLight)
        Safe_Release(elem);
    //해당 클래스에 있는 변수들은 항상 safe_release해주기
}
