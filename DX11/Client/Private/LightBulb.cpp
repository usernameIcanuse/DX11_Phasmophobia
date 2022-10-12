#include "stdafx.h"
#include "../Public/LightBulb.h"
#include "GameInstance.h"

CLightBulb::CLightBulb(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice, pContext)
{
}

CLightBulb::CLightBulb(const CLightBulb& rhs)
    :CGameObject(rhs)
{
}

HRESULT CLightBulb::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CLightBulb::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(nullptr)))
        return E_FAIL;

    if (nullptr != pArg)
    {
        m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4((_float4x4*)pArg));
    }

    if (FAILED(Setup_Component()))
        return E_FAIL;

    if (FAILED(Setup_Light()))
        return E_FAIL;

    m_pEventFunc = m_pEventFunc = std::bind(&CLightBulb::Normal_Operation, std::placeholders::_1, std::placeholders::_2);

    GAMEINSTANCE->Add_EventObject(CGame_Manager::EVENT_ITEM, this);

    return S_OK;
}

void CLightBulb::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
#ifdef _DEBUG
    m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
#endif

}

void CLightBulb::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    if (m_bSwitch)
    {
        GAMEINSTANCE->Add_Light(m_pLight);
    }

    GAMEINSTANCE->Add_Object_For_Culling(this, CRenderer::RENDER_NONALPHABLEND);
}

HRESULT CLightBulb::Render()
{

    _uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        _int       iPassIndex = 2;

        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
            return E_FAIL;
        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
        {
            iPassIndex = 0;
        }
        if (m_bSwitch)
        {
            iPassIndex = 3;
            if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_EmissiveTexture", i, aiTextureType_EMISSIVE)))
                return E_FAIL;
        }

        m_pModelCom->Render(i, m_pShaderCom, iPassIndex);
    }


    return S_OK;
}

HRESULT CLightBulb::SetUp_ModelCom(const _tchar* pPrototypeTag)
{
    
    return S_OK;
}


HRESULT CLightBulb::Setup_Component()
{
    /* For.Com_Shader*/
    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Shader_VtxModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Model_LightBulb"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;

#ifdef _DEBUG
    /* For.Com_OBB*/
    CCollider::COLLIDERDESC  ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

    ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);
    ColliderDesc.pOwner = this;
    ColliderDesc.m_eObjID = COLLISION_TYPE::LIGHTSWITCH;

    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
        return E_FAIL;
#endif

    return S_OK;
}

HRESULT		CLightBulb::Setup_Light()
{
    LIGHTDESC LightDesc;

    LightDesc.eType = LIGHTDESC::TYPE_POINT;
    LightDesc.vDiffuse = _float4(0.6f, 0.6f, 0.6f, 1.f);
    LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
    LightDesc.vSpecular = _float4(0.7f, 0.7f, 0.7f, 1.f);
    XMStoreFloat4(&LightDesc.vPosition, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
    
    LightDesc.fRange = 65.f;
    LightDesc.fAttenuation0 = 1.f;
    LightDesc.fAttenuation1 = 0.08f;
    LightDesc.fAttenuation2 = 0.02f;

    m_pLight = CLight::Create(m_pDevice, m_pContext, LightDesc);
    if (nullptr == m_pLight)
        return E_FAIL;

    return S_OK;
}

void CLightBulb::OnEventMessage(const _tchar* pMessage)
{
    if (0 == lstrcmp(pMessage, TEXT("Event")))
    {
        m_pEventFunc = std::bind(&CLightBulb::MalFunction, std::placeholders::_1, std::placeholders::_2);
    }
    else if (0 == lstrcmp(pMessage, TEXT("Normal_Operation")))
    {
        m_pEventFunc = std::bind(&CLightBulb::Normal_Operation, std::placeholders::_1, std::placeholders::_2);
    }
}

HRESULT CLightBulb::SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix)
{
    if (nullptr == m_pShaderCom ||
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

void CLightBulb::Call_EventFunc(_float fTimeDelta)
{
    m_pEventFunc(this, fTimeDelta);
}

void CLightBulb::MalFunction(_float fTimeDelta)
{

}

void CLightBulb::Normal_Operation(_float fTimeDelta)
{

}

void CLightBulb::On_Collision_Enter(CCollider* pCollider)
{
}

void CLightBulb::On_Collision_Stay(CCollider* pCollider)
{
}

void CLightBulb::On_Collision_Exit(CCollider* pCollider)
{
}

CLightBulb* CLightBulb::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CLightBulb* pInstance = new CLightBulb(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CLightBulb");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CLightBulb::Clone(void* pArg)
{
    CLightBulb* pInstance = new CLightBulb(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CLightBulb");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLightBulb::Free()
{
    __super::Free();
    
#ifdef _DEBUG
    Safe_Release(m_pOBBCom);
#endif

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pLight);
   
    //해당 클래스에 있는 변수들은 항상 safe_release해주기
}
