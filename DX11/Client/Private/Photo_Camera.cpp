#include "stdafx.h"
#include "../Public/Photo_Camera.h"
#include "GameInstance.h"

CPhoto_Camera::CPhoto_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CItem(pDevice, pContext)
{
}

CPhoto_Camera::CPhoto_Camera(const CPhoto_Camera& rhs)
    :CItem(rhs)
{
}

HRESULT CPhoto_Camera::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPhoto_Camera::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Setup_Component()))
        return E_FAIL;

    m_vAdjustpos = _float3(1.f, 1.f, 1.9f);


    return S_OK;
}

void CPhoto_Camera::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
    m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CPhoto_Camera::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    GAMEINSTANCE->Add_Object_For_Culling(this, CRenderer::RENDER_NONALPHABLEND);
#ifdef _DEBUG
    m_pRendererCom->Add_DebugRenderGroup(m_pOBBCom);
#endif
}

HRESULT CPhoto_Camera::Render()
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


        m_pModelCom->Render(i,m_pShaderCom,0);
    }

//#ifdef _DEBUG
//    m_pOBBCom->Render();
//#endif // _DEBUG


    return S_OK;
}

_bool CPhoto_Camera::Install(_float3 vPosition, COLLISION_TYPE eType, _float4 vLook)
{
    if (eType == COLLISION_TYPE::OBJECT)
    {
        _vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
        _vector vecLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK) * -1.f;
        _vector vRight = XMVector3Cross(vUp, vecLook);
        vecLook = XMVector3Cross(vRight, vUp);

        m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
        m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);
        m_pTransformCom->Set_State(CTransform::STATE_LOOK, vecLook);

        m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&vPosition), 1.f));
        return true;
    }
    return false;
}


void CPhoto_Camera::On_Collision_Enter(CCollider* pCollider)
{
    __super::On_Collision_Enter(pCollider);
}

void CPhoto_Camera::On_Collision_Stay(CCollider* pCollider)
{
}

void CPhoto_Camera::On_Collision_Exit(CCollider* pCollider)
{
}

HRESULT CPhoto_Camera::Setup_Component()
{

    if (FAILED(__super::Setup_Component()))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Video_Camera"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;

    /* For.Com_OBB*/
    CCollider::COLLIDERDESC			ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

    ColliderDesc.vScale = _float3(1.0f, 0.5f, 0.5f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);
    ColliderDesc.pOwner = this;
    ColliderDesc.m_eObjID = COLLISION_TYPE::ITEM;

    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
        return E_FAIL;


    return S_OK;
}


CPhoto_Camera* CPhoto_Camera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPhoto_Camera* pInstance = new CPhoto_Camera(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CPhoto_Camera");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CPhoto_Camera::Clone(void* pArg)
{
    CPhoto_Camera* pInstance = new CPhoto_Camera(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CPhoto_Camera");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPhoto_Camera::Free()
{
    __super::Free();
}
