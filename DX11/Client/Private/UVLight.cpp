#include "stdafx.h"
#include "../Public/UVLight.h"
#include "GameInstance.h"

CUVLight::CUVLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CItem(pDevice, pContext)
{
}

CUVLight::CUVLight(const CUVLight& rhs)
    :CItem(rhs)
{
}

HRESULT CUVLight::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUVLight::Initialize(void* pArg)
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

    m_eType = UVLIGHT;

    return S_OK;
}

void CUVLight::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CUVLight::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    if (m_bSwitch)
    {   //Update Lightdesc;
        LIGHTDESC* pLightDesc = m_pSpotLight->Get_LightDesc();

        _vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);


        XMStoreFloat4(&pLightDesc->vPosition, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) + vLook);
        XMStoreFloat4(&pLightDesc->vDirection, vLook);


        GAMEINSTANCE->Add_Light(m_pSpotLight);
    }
    GAMEINSTANCE->Add_Object_For_Culling(this, CRenderer::RENDER_NONALPHABLEND);
#ifdef _DEBUG
  //  m_pRendererCom->Add_DebugRenderGroup(m_pOBBCom);
#endif
}

HRESULT CUVLight::Render()
{

    _uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
            return E_FAIL;
        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
            return E_FAIL;


        m_pModelCom->Render(i, m_pShaderCom, 2);
    }
    //
    //#ifdef _DEBUG
    //    m_pOBBCom->Render();
    //#endif // _DEBUG


    return S_OK;
}


void CUVLight::On_Collision_Enter(CCollider* pCollider)
{
    __super::On_Collision_Enter(pCollider);
}

void CUVLight::On_Collision_Stay(CCollider* pCollider)
{
}

void CUVLight::On_Collision_Exit(CCollider* pCollider)
{
}

void CUVLight::MalFunction(_float fTimeDelta)
{
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
        LIGHTDESC* pLightDesc = m_pSpotLight->Get_LightDesc();
        _vector vLerpDiffuse = XMQuaternionSlerp(XMLoadFloat4(&pLightDesc->vDiffuse), XMLoadFloat4(&m_vBlinkDiffuse), 0.3f);
        XMStoreFloat4(&pLightDesc->vDiffuse, vLerpDiffuse);
    }
}

void CUVLight::Normal_Operation(_float fTimeDelta)
{
    if (m_bSwitch)
    {
        LIGHTDESC* pLightDesc = m_pSpotLight->Get_LightDesc();
        pLightDesc->vDiffuse = m_vDiffuse;

    }
}

HRESULT CUVLight::Setup_Component()
{
    if (FAILED(__super::Setup_Component()))
        return E_FAIL;


    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_UVLight"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;

    /* For.Com_OBB*/
    CCollider::COLLIDERDESC			ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

    ColliderDesc.vScale = _float3(0.7f, 0.7f, 1.2f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);
    ColliderDesc.pOwner = this;
    ColliderDesc.m_eObjID = COLLISION_TYPE::ITEM;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUVLight::Setup_Light()
{
    LIGHTDESC			LightDesc;
    ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

    LightDesc.eType = tagLightDesc::TYPE_SPOTLIGHT;
    LightDesc.bUVLight = true;

    LightDesc.vDiffuse = m_vDiffuse=_float4(1.f, 0.f, 1.f, 1.f);
    LightDesc.vAmbient = _float4(0.3f, 0.f, 0.3f, 1.f);
    LightDesc.vSpecular = _float4(1.f, 0.f, 1.f, 1.f);

    _vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);


    XMStoreFloat4(&LightDesc.vPosition, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) + vLook);
    XMStoreFloat4(&LightDesc.vDirection, m_pTransformCom->Get_State(CTransform::STATE_LOOK));
    LightDesc.fRange = 65.f;

    LightDesc.fAttenuation0 = 1.f;
    LightDesc.fAttenuation1 = 0.07f;
    LightDesc.fAttenuation2 = 0.017;

    LightDesc.fFallOff = 2.f;
    LightDesc.fTheta = XMConvertToRadians(45.f);
    LightDesc.fPhi = XMConvertToRadians(90.f);


    m_pSpotLight = CLight::Create(m_pDevice, m_pContext, LightDesc);
    if (nullptr == m_pSpotLight)
    {
        return E_FAIL;
    }
    return S_OK;
}



CUVLight* CUVLight::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUVLight* pInstance = new CUVLight(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUVLight");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUVLight::Clone(void* pArg)
{
    CUVLight* pInstance = new CUVLight(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUVLight");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUVLight::Free()
{
    __super::Free();

    Safe_Release(m_pSpotLight);
}
