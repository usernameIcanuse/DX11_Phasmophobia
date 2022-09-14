#include "stdafx.h"
#include "../Public/Tripod.h"
#include "GameInstance.h"
#include "Video_Camera.h"

CTripod::CTripod(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CItem(pDevice, pContext)
{
}

CTripod::CTripod(const CTripod& rhs)
    :CItem(rhs)
{
}

HRESULT CTripod::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTripod::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Setup_Component()))
        return E_FAIL;

    m_vAdjustpos = _float3(1.f, 6.5f, 2.f);

    return S_OK;
}

void CTripod::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);


    _matrix matWorld = m_pTransformCom->Get_WorldMatrix();

    if (m_pConnectedCamera)
    {
        BoundingOrientedBox* Tripod = nullptr;
        Tripod = (BoundingOrientedBox*)m_pTripodCom->Get_Collider();
        _float4     vLook;
        XMStoreFloat4(&vLook, m_pTransformCom->Get_State(CTransform::STATE_UP));

        m_pConnectedCamera -> Install(Tripod->Center, COLLISION_TYPE::TRIPOD, vLook,this);
    }
    m_pOBBCom->Update(matWorld);
    m_pTripodCom->Update(matWorld);

}

void CTripod::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CTripod::Render()
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

        if (m_bSwitch)
        {
            if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_EMISSIVE)))
                return E_FAIL;

            m_pShaderCom->Begin(1);
        }
        else
         m_pShaderCom->Begin(0);

        m_pModelCom->Render(i);
    }

#ifdef _DEBUG
      m_pOBBCom->Render();
      m_pTripodCom->Render();
#endif // _DEBUG


    return S_OK;
}

void	CTripod::Update_Collider()
{
    __super::Update_Collider();

    m_pTripodCom->Update(m_pTransformCom->Get_WorldMatrix());
    if (m_pConnectedCamera)
    {
        BoundingOrientedBox* Tripod = nullptr;
        Tripod = (BoundingOrientedBox*)m_pTripodCom->Get_Collider();
        _float4     vLook;
        XMStoreFloat4(&vLook, m_pTransformCom->Get_State(CTransform::STATE_UP));


        m_pConnectedCamera->Install(Tripod->Center, COLLISION_TYPE::TRIPOD, vLook,this);

    }
}



void CTripod::On_Collision_Enter(CCollider* pCollider)
{
    __super::On_Collision_Enter(pCollider);
}

void CTripod::On_Collision_Stay(CCollider* pCollider)
{
}

void CTripod::On_Collision_Exit(CCollider* pCollider)
{
}

HRESULT CTripod::Setup_Component()
{
    if (FAILED(__super::Setup_Component()))
        return E_FAIL;

    /* For.Com_Shader*/
    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Shader_VtxModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;


    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Tripod"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;

    /* For.Com_OBB*/
    CCollider::COLLIDERDESC			ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

    ColliderDesc.vScale = _float3(1.f, 6.f, 1.f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);
    ColliderDesc.pOwner = this;
    ColliderDesc.m_eObjID = COLLISION_TYPE::ITEM;

    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
        return E_FAIL;

    /* For.Com_Tripod*/
  
    ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

    ColliderDesc.vScale = _float3(1.f, 0.2f, 1.f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, 6.f, 0.f);
    ColliderDesc.pOwner = this;
    ColliderDesc.m_eObjID = COLLISION_TYPE::TRIPOD;

    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_Tripod"), (CComponent**)&m_pTripodCom, &ColliderDesc)))
        return E_FAIL;


    return S_OK;
}

HRESULT CTripod::SetUp_ShaderResource()
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

CTripod* CTripod::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTripod* pInstance = new CTripod(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CTripod");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTripod::Clone(void* pArg)
{
    CTripod* pInstance = new CTripod(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CTripod");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTripod::Free()
{
    __super::Free();

    Safe_Release(m_pTripodCom);
}
