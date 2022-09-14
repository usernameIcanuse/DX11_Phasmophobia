#include "stdafx.h"
#include "../Public/Video_Camera.h"
#include "GameInstance.h"
#include "Tripod.h"

CVideo_Camera::CVideo_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CItem(pDevice, pContext)
{
}

CVideo_Camera::CVideo_Camera(const CVideo_Camera& rhs)
    :CItem(rhs)
{
}

HRESULT CVideo_Camera::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CVideo_Camera::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Setup_Component()))
        return E_FAIL;

    m_vAdjustpos = _float3(1.f, 1.f, 1.9f);


    return S_OK;
}

void CVideo_Camera::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
    m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CVideo_Camera::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

}

HRESULT CVideo_Camera::Render()
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

        m_pModelCom->Render(i);
    }

#ifdef _DEBUG
    m_pOBBCom->Render();
#endif // _DEBUG


    return S_OK;
}

_bool CVideo_Camera::Install(_float3 vPosition, COLLISION_TYPE eType, _float4 vLook, CItem* pConnectItem)
{
    if (eType == COLLISION_TYPE::OBJECT )
    {
        _float3 vScale = m_pTransformCom->Get_Scaled();

        _vector vUp = XMLoadFloat4(&vLook);
        _vector vecLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK) * -1.f;
        _vector vRight = XMVector3Cross(vUp, vecLook);
        vecLook = XMVector3Cross(vRight, vUp);

        vecLook = XMVector3Normalize(vecLook);
        vUp= XMVector3Normalize(vUp);
        vRight= XMVector3Normalize(vRight);

        m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight*vScale.x);
        m_pTransformCom->Set_State(CTransform::STATE_UP, vUp * vScale.y);
        m_pTransformCom->Set_State(CTransform::STATE_LOOK, vecLook*vScale.z);

        m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&vPosition), 1.f));
        
        m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());


        return true;
    }

   if (eType == COLLISION_TYPE::TRIPOD)
    {
       CTransform* pTransform = (CTransform*)pConnectItem->Get_Component(CGameObject::m_pTransformTag);
       
       _float3 vScale = m_pTransformCom->Get_Scaled();

       _vector vUp = XMLoadFloat4(&vLook);
       _vector vecLook = pTransform->Get_State(CTransform::STATE_LOOK);
       _vector vRight = pTransform->Get_State(CTransform::STATE_RIGHT);
     

       vecLook = XMVector3Normalize(vecLook);
       vUp = XMVector3Normalize(vUp);
       vRight = XMVector3Normalize(vRight);

       m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
       m_pTransformCom->Set_State(CTransform::STATE_UP, vUp * vScale.y);
       m_pTransformCom->Set_State(CTransform::STATE_LOOK, vecLook * vScale.z);

       m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&vPosition), 1.f));

       m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
       
       Connect_Tripod((CTripod*)pConnectItem);
       

       return true;
    }

    return false;
}

void CVideo_Camera::Connect_Tripod(CTripod* pTripod)
{
    if (nullptr == pTripod)
        return;

    m_pTripod = pTripod;
    m_pTripod->Connect_Camera(this);
}

void CVideo_Camera::Disconnect_Tripod()
{
    if(m_pTripod)
        m_pTripod->Seperate_Camera();
    m_pTripod = nullptr;
}


void CVideo_Camera::On_Collision_Enter(CCollider* pCollider)
{
    __super::On_Collision_Enter(pCollider);

}

void CVideo_Camera::On_Collision_Stay(CCollider* pCollider)
{
}

void CVideo_Camera::On_Collision_Exit(CCollider* pCollider)
{
}

HRESULT CVideo_Camera::Setup_Component()
{

    if (FAILED(__super::Setup_Component()))
        return E_FAIL;

    /* For.Com_Shader*/
    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Shader_VtxModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Video_Camera"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;

    /* For.Com_OBB*/
    CCollider::COLLIDERDESC			ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

    ColliderDesc.vScale = _float3(1.0f, 0.5f, 0.5f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.1f, ColliderDesc.vScale.y * 0.5f, 0.f);
    ColliderDesc.pOwner = this;
    ColliderDesc.m_eObjID = COLLISION_TYPE::CAMERA;

    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
        return E_FAIL;


    return S_OK;
}

HRESULT CVideo_Camera::SetUp_ShaderResource()
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

CVideo_Camera* CVideo_Camera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CVideo_Camera* pInstance = new CVideo_Camera(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CVideo_Camera");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CVideo_Camera::Clone(void* pArg)
{
    CVideo_Camera* pInstance = new CVideo_Camera(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CVideo_Camera");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CVideo_Camera::Free()
{
    __super::Free();
}
