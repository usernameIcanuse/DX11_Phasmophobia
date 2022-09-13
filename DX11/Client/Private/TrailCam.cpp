#include "stdafx.h"
#include "../Public/TrailCam.h"
#include "GameInstance.h"

CTrailCam::CTrailCam(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CItem(pDevice, pContext)
{
}

CTrailCam::CTrailCam(const CTrailCam& rhs)
    :CItem(rhs)
{
}

HRESULT CTrailCam::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTrailCam::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Setup_Component()))
        return E_FAIL;



    return S_OK;
}

void CTrailCam::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    _matrix matWorld = m_pTransformCom->Get_WorldMatrix();
    m_pOBBCom->Update(matWorld);
    m_pAreaCom->Update(matWorld);
}

void CTrailCam::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

}

HRESULT CTrailCam::Render()
{
    if (nullptr == m_pShaderCom ||
        nullptr == m_pModelCom)
        return E_FAIL;

    /* ���̴� ���������� ���� ������. */
    if (FAILED(SetUp_ShaderResource()))
        return E_FAIL;




    _uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
            return E_FAIL;
      /*  if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
            return E_FAIL;*/

        m_pShaderCom->Begin(0);

        m_pModelCom->Render(i);
    }

#ifdef _DEBUG
      m_pOBBCom->Render();
      m_pAreaCom->Render();
#endif // _DEBUG


    return S_OK;
}

void CTrailCam::On_Collision_Enter(CCollider* pCollider)
{
    __super::On_Collision_Enter(pCollider);


  
}


void CTrailCam::On_Collision_Stay(CCollider* pCollider)
{
    if (COLLISION_TYPE::PLAYER == pCollider->Get_Type() || COLLISION_TYPE::GHOST == pCollider->Get_Type())
    {

    }
}

void CTrailCam::On_Collision_Exit(CCollider* pCollider)
{
}

HRESULT CTrailCam::Setup_Component()
{
    if (FAILED(__super::Setup_Component()))
        return E_FAIL;

    /* For.Com_Shader*/
    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Shader_VtxModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;


    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Model_TrailCam"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;

    /* For.Com_OBB*/
    CCollider::COLLIDERDESC			ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

    ColliderDesc.vScale = _float3(1.f, 2.f, 0.5f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);
    ColliderDesc.pOwner = this;
    ColliderDesc.m_eObjID = COLLISION_TYPE::ITEM;

    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
        return E_FAIL;

    /* For.Com_Area*/
   
    ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

    ColliderDesc.vScale = _float3(5.f, 5.f, 5.f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);

    _vector     vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
    XMStoreFloat3(&ColliderDesc.vTranslation, XMVectorSet(0.f, 0.f, 0.f, 0.f) + ColliderDesc.vScale.z*0.55f*vLook);
    //ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);
    ColliderDesc.pOwner = this;
    ColliderDesc.m_eObjID = COLLISION_TYPE::ITEM;

    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_Area"), (CComponent**)&m_pAreaCom, &ColliderDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CTrailCam::SetUp_ShaderResource()
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

_bool CTrailCam::Install(_float3 vPosition, COLLISION_TYPE eType, _float4 vLook)
{
    if (eType == COLLISION_TYPE::WALL)
    {
        _float3 vScale = m_pTransformCom->Get_Scaled();
        _vector vecLook = XMVector3Normalize(XMLoadFloat4(&vLook));
        m_pTransformCom->Set_State(CTransform::STATE_LOOK, vecLook *  vScale.z);
        _vector vUp = m_pTransformCom->Get_State(CTransform::STATE_UP);
        _vector vRight = XMVector3Cross(vUp, XMLoadFloat4(&vLook));

        vUp = XMVector3Cross( XMLoadFloat4(&vLook),vRight);


        vRight = XMVector3Normalize(vRight);
        vUp = XMVector3Normalize(vUp);

        m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
        m_pTransformCom->Set_State(CTransform::STATE_UP, vUp * vScale.y);
        m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&vPosition), 1.f));

        m_pTransformCom->Rotation(vUp, XMConvertToRadians(180.f));

        m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

        return true;
    }

    return false;
}

CTrailCam* CTrailCam::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTrailCam* pInstance = new CTrailCam(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CTrailCam");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTrailCam::Clone(void* pArg)
{
    CTrailCam* pInstance = new CTrailCam(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CTrailCam");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTrailCam::Free()
{
    __super::Free();
    Safe_Release(m_pAreaCom);
}