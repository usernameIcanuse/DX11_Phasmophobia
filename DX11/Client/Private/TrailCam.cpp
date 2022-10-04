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

    if (FAILED(Setup_TempModel()))
        return E_FAIL;

    if (FAILED(Setup_Light()))
        return E_FAIL;

    return S_OK;
}

void CTrailCam::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    _matrix matWorld = m_pTransformCom->Get_WorldMatrix();
    m_pOBBCom->Update(matWorld);
    m_pAreaCom->Update(matWorld);

    if (!m_bInstalled && m_bSwitch)
    {
        m_bSwitch = false;
    }
}

void CTrailCam::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);


    if (m_bSwitch)
    {
        LIGHTDESC* pLightDesc = m_pLight->Get_LightDesc();

        _vector vUp= m_pTransformCom->Get_State(CTransform::STATE_UP);

        XMStoreFloat4(&pLightDesc->vPosition, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) + vUp);

        GAMEINSTANCE->Add_Light(m_pLight);
    }
   // m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
    GAMEINSTANCE->Add_Object_For_Culling(this, CRenderer::RENDER_NONALPHABLEND);

#ifdef _DEBUG
    m_pRendererCom->Add_DebugRenderGroup(m_pOBBCom);
    m_pRendererCom->Add_DebugRenderGroup(m_pAreaCom);
#endif
}

HRESULT CTrailCam::Render()
{

    _uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
            return E_FAIL;
        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
            return E_FAIL;

        
        m_pModelCom->Render(i, m_pShaderCom,2);
    }

//#ifdef _DEBUG
//      m_pOBBCom->Render();
//      m_pAreaCom->Render();
//#endif // _DEBUG

      m_pTempTrailCamModel->Set_Enable(false);

    return S_OK;
}

void CTrailCam::On_Collision_Enter(CCollider* pCollider)
{
    if (!m_bInstalled)
        return;
    if (COLLISION_TYPE::PLAYER == pCollider->Get_Type() || COLLISION_TYPE::GHOST == pCollider->Get_Type())
    {
        m_bSwitch = true;
    }
  
}


void CTrailCam::On_Collision_Stay(CCollider* pCollider)
{
 
}

void CTrailCam::On_Collision_Exit(CCollider* pCollider)
{
    if (!m_bInstalled)
        return;
    if (COLLISION_TYPE::PLAYER == pCollider->Get_Type() || COLLISION_TYPE::GHOST == pCollider->Get_Type())
    {
        m_bSwitch = false;
    }
}

HRESULT CTrailCam::Setup_Component()
{
    if (FAILED(__super::Setup_Component()))
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
    ColliderDesc.m_eObjID = COLLISION_TYPE::ITEM_AREA;

    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_Area"), (CComponent**)&m_pAreaCom, &ColliderDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CTrailCam::Setup_TempModel()
{
    /*For.TempModel*/
    if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_TempModel"), TEXT("Prototype_GameObject_TempTrailCam"), (CGameObject**)&m_pTempTrailCamModel)))
        return E_FAIL;

    m_pTempTrailCamModel->Set_Enable(false);

    return S_OK;
}

HRESULT CTrailCam::Setup_Light()
{
    LIGHTDESC LightDesc;
    ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

    LightDesc.eType = LIGHTDESC::TYPE_POINT;

    LightDesc.vDiffuse = _float4(1.f, 1.f, 0.8f, 1.f);
    LightDesc.vAmbient = _float4(1.f, 1.f, 0.8f, 1.f);
    LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

    LightDesc.fRange = 32.f;

    LightDesc.fAttenuation0 = 1.f;
    LightDesc.fAttenuation1 = 0.14f;
    LightDesc.fAttenuation2 = 0.07f;

    m_pLight = CLight::Create(m_pDevice, m_pContext, LightDesc);
    if (nullptr == m_pLight)
        return E_FAIL;


    return S_OK;
}

_bool CTrailCam::Install(_float3 vPosition, COLLISION_TYPE eType, _float4 vLook, CItem* pConnectItem)
{
    if (eType == COLLISION_TYPE::WALL)
    {
        _float3 vScale = m_pTransformCom->Get_Scaled();
        _vector vecLook = XMVector3Normalize(XMLoadFloat4(&vLook));
        m_pTransformCom->Set_State(CTransform::STATE_LOOK, vecLook *  vScale.z);
        _vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
        _vector vRight = XMVector3Cross(vUp, XMLoadFloat4(&vLook));

        vUp = XMVector3Cross( XMLoadFloat4(&vLook),vRight);


        vRight = XMVector3Normalize(vRight);
        vUp = XMVector3Normalize(vUp);

        m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
        m_pTransformCom->Set_State(CTransform::STATE_UP, vUp * vScale.y);
        m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&vPosition), 1.f));

       // m_pTransformCom->Rotation(vUp, XMConvertToRadians(180.f));

        m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

        return true;
    }

    return false;
}

void CTrailCam::Set_TempModel_Pos(_float3 vPosition, COLLISION_TYPE eType, _float4 vLook, CItem* pConnectItem)
{
    if (m_pTempTrailCamModel)
    {
        m_pTempTrailCamModel->Set_Enable(true);
        m_pTempTrailCamModel->Set_TempModel_Pos(vPosition, eType, vLook, pConnectItem);
    }
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
    Safe_Release(m_pLight);
}
