#include "stdafx.h"
#include "../Public/DotsProjecter.h"
#include "GameInstance.h"

CDotsProjecter::CDotsProjecter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CItem(pDevice,pContext)
{
}

CDotsProjecter::CDotsProjecter(const CDotsProjecter& rhs)
    :CItem(rhs)
{
}

HRESULT CDotsProjecter::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CDotsProjecter::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(nullptr)))
        return E_FAIL;

    if (nullptr != pArg)
    {
        m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4((_float4x4*)pArg));
    }
   // m_pTransformCom->Set_Scaled(_float3(5.f, 5.f, 5.f));

    if (FAILED(Setup_Component()))
        return E_FAIL;

    if (FAILED(Setup_TempModel()))
        return E_FAIL;

    if (FAILED(Setup_Light()))
        return E_FAIL;

    GAMEINSTANCE->Add_EventObject(CGame_Manager::EVENT_ITEM, this);
    m_pEventFunc = std::bind(&CItem::Normal_Operation, std::placeholders::_1, std::placeholders::_2);


    return S_OK;
}

void CDotsProjecter::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    if (false == m_bInstalled)
    {
        m_pAreaCom->Set_Enable(false);
    }

    m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
    m_pRigidBodyCom->Update(fTimeDelta, m_pCurrNavigation);
    if (m_bInstalled)
    {
        m_pAreaCom->Update(m_pTransformCom->Get_WorldMatrix());
    }
}

void CDotsProjecter::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    if (m_bInstalled)
    {
        LIGHTDESC* pLightDesc = m_pLight->Get_LightDesc();

        _vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

        XMStoreFloat4(&pLightDesc->vPosition, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) - vLook);

        GAMEINSTANCE->Add_Light(m_pLight);
    }

    GAMEINSTANCE->Add_Object_For_Culling(this, CRenderer::RENDER_NONALPHABLEND);

#ifdef _DEBUG
   //m_pRendererCom->Add_DebugRenderGroup(m_pOBBCom);
   //m_pRendererCom->Add_DebugRenderGroup(m_pAreaCom);

#endif

}

HRESULT CDotsProjecter::Render()
{

    _uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        _int        iPassIndex = 2;
        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
            return E_FAIL;
      
        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
        {
            if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_OPACITY)))
            {
                iPassIndex = 0;
            }
        }

        
        m_pModelCom->Render(i, m_pShaderCom, iPassIndex);
    }
//
//#ifdef _DEBUG
//    m_pOBBCom->Render();
//#endif // _DEBUG
    m_pTempDotsModel->Set_Enable(false);

    return S_OK;
}


_bool CDotsProjecter::Install(_float3 vPosition, COLLISION_TYPE eType, _float4 vLook, CItem* pConnectItem)
{
    if (COLLISION_TYPE::OBJECT == eType|| eType == COLLISION_TYPE::NAVIGATION)
    {
        _float3 vScale = m_pTransformCom->Get_Scaled();
        _vector vecLook = XMVector3Normalize(XMLoadFloat4(&vLook));
        m_pTransformCom->Set_State(CTransform::STATE_LOOK, vecLook * vScale.z);
        _vector vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f);
        _vector vUp = XMVector3Cross(XMLoadFloat4(&vLook), vRight);


        vRight = XMVector3Normalize(vRight);
        vUp = XMVector3Normalize(vUp);

        m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
        m_pTransformCom->Set_State(CTransform::STATE_UP, vUp * vScale.y);
        m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&vPosition), 1.f));

        m_pTransformCom->Rotation(vUp, XMConvertToRadians(180.f));
        m_pTransformCom->Rotation(vecLook, XMConvertToRadians(180.f));
       
       // m_pTransformCom->Rotation(vUp, XMConvertToRadians(180.f));

        m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
        m_pAreaCom->Set_Enable(true);

        return true;
    }
    else if (COLLISION_TYPE::WALL == eType)
    {
        _float3 vScale = m_pTransformCom->Get_Scaled();
        _vector vecLook = XMVector3Normalize(XMLoadFloat4(&vLook));
        m_pTransformCom->Set_State(CTransform::STATE_LOOK, vecLook * vScale.z);
        _vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
        _vector vRight = XMVector3Cross(vUp, XMLoadFloat4(&vLook));

        vUp = XMVector3Cross(XMLoadFloat4(&vLook), vRight);


        vRight = XMVector3Normalize(vRight);
        vUp = XMVector3Normalize(vUp);

        m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
        m_pTransformCom->Set_State(CTransform::STATE_UP, vUp * vScale.y);
        m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&vPosition), 1.f));

        m_pTransformCom->Rotation(vUp, XMConvertToRadians(180.f));

        m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
        m_pAreaCom->Set_Enable(true);
        return true;
    }

    return false;
}

void CDotsProjecter::Set_TempModel_Pos(_float3 vPosition, COLLISION_TYPE eType, _float4 vLook, CItem* pConnectItem)
{
    if (m_pTempDotsModel)
    {
        m_pTempDotsModel->Set_Enable(true);
        m_pTempDotsModel->Set_TempModel_Pos(vPosition, eType, vLook, pConnectItem);
    }
}

void CDotsProjecter::MalFunction(_float fTimeDelta)
{
    if (false == m_bIsInHouse)
        return;
    if (m_bSwitch)
    {
        m_fBlinkTime += fTimeDelta;
        if (0.1f < m_fBlinkTime)
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dis(0, 100);

            _float fRatio = dis(gen) * 0.01f;
            XMStoreFloat4(&m_vBlinkDiffuse, XMLoadFloat4(&m_vDiffuse) * fRatio);
            m_fBlinkTime = 0.f;
        }
        LIGHTDESC* pLightDesc = m_pLight->Get_LightDesc();
        _vector vLerpDiffuse = XMQuaternionSlerp(XMLoadFloat4(&pLightDesc->vDiffuse), XMLoadFloat4(&m_vBlinkDiffuse), 0.3f);
        XMStoreFloat4(&pLightDesc->vDiffuse, vLerpDiffuse);
    }
}

void CDotsProjecter::Normal_Operation(_float fTimeDelta)
{
    /*±Í½Å ·»´õ?*/ 
}

void CDotsProjecter::Drop_Item(_vector vPower)
{
    _vector vLook = XMVectorSet(0.f, -1.f, 0.f, 0.f);
    _vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
    _vector vUp= XMVector3Cross(vLook, vRight);
   // vUp = XMVector3Cross(vLook, vRight);

    m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
    m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);
    m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);

    Add_Power(vPower);
}


void CDotsProjecter::On_Collision_Enter(CCollider* pCollider)
{
    if (COLLISION_TYPE::HOUSE == pCollider->Get_Type())
    {
        m_pCurrNavigation = m_pNaviHouseCom;
        m_bIsInHouse = true;
    }
}

void CDotsProjecter::On_Collision_Stay(CCollider* pCollider)
{
}

void CDotsProjecter::On_Collision_Exit(CCollider* pCollider)
{
    if (COLLISION_TYPE::HOUSE == pCollider->Get_Type())
    {
        m_pCurrNavigation = m_pNaviOutSideCom;
        m_bIsInHouse = false;
    }
}

HRESULT CDotsProjecter::Setup_Component()
{

    if (FAILED(__super::Setup_Component()))
        return E_FAIL;
 
    /* For.Com_Navigation*/
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
    CNavigation::NAVIDESC	NaviDesc;
    ZeroMemory(&NaviDesc, sizeof(CNavigation::NAVIDESC));
    NaviDesc.m_iCurrentIndex = 0;

    if (FAILED(__super::Add_Component(pGameInstance->Get_Next_Level(), TEXT("Prototype_Component_Navigation_House"), TEXT("Com_NaviHouse"), (CComponent**)&m_pNaviHouseCom, &NaviDesc)))
        return E_FAIL;

    ZeroMemory(&NaviDesc, sizeof(CNavigation::NAVIDESC));
    NaviDesc.m_iCurrentIndex = 0;

    if (FAILED(__super::Add_Component(pGameInstance->Get_Next_Level(), TEXT("Prototype_Component_Navigation_OutSide"), TEXT("Com_NaviOutSide"), (CComponent**)&m_pNaviOutSideCom, &NaviDesc)))
        return E_FAIL;

    RELEASE_INSTANCE(CGameInstance);
    m_pCurrNavigation = m_pNaviOutSideCom;

    /*For.Com_RigidBody*/
    CRigidBody::RIGIDBODYDESC RigidBodyDesc;
    ZeroMemory(&RigidBodyDesc, sizeof(CRigidBody::RIGIDBODYDESC));
    RigidBodyDesc.fWeight = 1.f;
    RigidBodyDesc.pOwnerTransform = m_pTransformCom;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBodyCom, &RigidBodyDesc)))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_DotsProjecter"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;

    /* For.Com_OBB*/
    CCollider::COLLIDERDESC			ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

    ColliderDesc.vScale = _float3(0.5f, 1.f, 0.5f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, ColliderDesc.vScale.z * -0.5f);
    ColliderDesc.pOwner = this;
    ColliderDesc.m_eObjID = COLLISION_TYPE::ITEM;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
        return E_FAIL;

    /* For.Com_Area*/

    ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

    ColliderDesc.vScale = _float3(5.f, 5.f, 5.f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);

    _vector     vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
    XMStoreFloat3(&ColliderDesc.vTranslation, XMVectorSet(0.f, 0.f, -ColliderDesc.vScale.z*0.5f,0.f));
    //ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);
    ColliderDesc.pOwner = this;
    ColliderDesc.m_eObjID = COLLISION_TYPE::DOTSPROJECTER;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_Area"), (CComponent**)&m_pAreaCom, &ColliderDesc)))
        return E_FAIL;

    m_pAreaCom->Set_Enable(false);

    /* For.Com_Texture*/

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_DotsProjecter"), TEXT("Com_Texture"), (CComponent**)&m_pDotsTexture)))
        return E_FAIL;


   

    return S_OK;
}


HRESULT CDotsProjecter::Setup_TempModel()
{
    /*For.TempModel*/
    if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_TempModel"), TEXT("Prototype_GameObject_TempDotsProjecter"), (CGameObject**)&m_pTempDotsModel)))
        return E_FAIL;

    m_pTempDotsModel->Set_Enable(false);
    return S_OK;
}

HRESULT CDotsProjecter::Setup_Light()
{
    LIGHTDESC LightDesc;
    ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

    LightDesc.eType = LIGHTDESC::TYPE_POINT;
    LightDesc.vDiffuse = m_vDiffuse= _float4(0.f, 1.f, 0.f, 1.f);
    LightDesc.vAmbient = _float4(0.f, 1.f, 0.f, 1.f);
    LightDesc.vSpecular = _float4(0.f, 1.f, 0.f, 1.f);

    LightDesc.fRange = 100.f;
    LightDesc.fAttenuation0 = 1.f;
    LightDesc.fAttenuation1 = 0.045f;
    LightDesc.fAttenuation2 = 0.0075f;

    LightDesc.bDotsProjecter = true;

    _vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

    XMStoreFloat4(&LightDesc.vPosition, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) - vLook);

    m_pLight = CLight::Create(m_pDevice, m_pContext, LightDesc);
    if (nullptr == m_pLight)
        return E_FAIL;

    m_pLight->Set_DotsTexture(m_pDotsTexture);

    return S_OK;
}

CDotsProjecter* CDotsProjecter::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CDotsProjecter* pInstance = new CDotsProjecter(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CDotsProjecter");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CDotsProjecter::Clone(void* pArg)
{
    CDotsProjecter* pInstance = new CDotsProjecter(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CDotsProjecter");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CDotsProjecter::Free()
{
    __super::Free();

    Safe_Release(m_pLight);
    Safe_Release(m_pAreaCom);

    Safe_Release(m_pNaviHouseCom);
    Safe_Release(m_pNaviOutSideCom);

    Safe_Release(m_pRigidBodyCom);

    Safe_Release(m_pDotsTexture);
}
