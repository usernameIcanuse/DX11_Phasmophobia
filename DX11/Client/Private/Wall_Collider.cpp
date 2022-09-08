#include "stdafx.h"
#include "../Public/Wall_Collider.h"
#include "GameInstance.h"

CWall_Collider::CWall_Collider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice,pContext)
{
}

CWall_Collider::CWall_Collider(const CWall_Collider& rhs)
    :CGameObject(rhs)
{
}

HRESULT CWall_Collider::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CWall_Collider::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Setup_Component()))
        return E_FAIL;

    return S_OK;
}

void CWall_Collider::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
 
    _matrix matWorld = m_pTransformCom->Get_WorldMatrix();
    m_pOBBCom->Update(matWorld);
    m_pOBBDirCom->Update(matWorld);
}

void CWall_Collider::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
   
    m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);


}

HRESULT CWall_Collider::Render()
{
#ifdef _DEBUG
    m_pOBBCom->Render();
    m_pOBBDirCom->Render();
#endif // _DEBUG

    return S_OK;
}


HRESULT CWall_Collider::Setup_Component()
{

    /* For.Com_OBB*/
    CCollider::COLLIDERDESC			ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

    ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);
    ColliderDesc.pOwner = this;
    ColliderDesc.m_eObjID = COLLISION_TYPE::WALL;

    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
        return E_FAIL;


    ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

    _vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

    ColliderDesc.vScale = _float3(1.f, 1.f, 2.f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);
    XMStoreFloat3(&ColliderDesc.vTranslation, XMLoadFloat3(&ColliderDesc.vTranslation) + vLook * 2.f);
    ColliderDesc.pOwner = this;
    ColliderDesc.m_eObjID = COLLISION_TYPE::DEFAULT;

    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBBDir"), (CComponent**)&m_pOBBDirCom, &ColliderDesc)))
        return E_FAIL;

    /* For.Com_Renderer*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
        return E_FAIL;
   
    return S_OK;
}


CWall_Collider* CWall_Collider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CWall_Collider* pInstance = new CWall_Collider(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CWall_Collider");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CWall_Collider::Clone(void* pArg)
{
    CWall_Collider* pInstance = new CWall_Collider(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CWall_Collider");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CWall_Collider::Free()
{
    __super::Free();
    
    Safe_Release(m_pOBBCom);
    Safe_Release(m_pOBBDirCom);
    Safe_Release(m_pRendererCom);

   
    //해당 클래스에 있는 변수들은 항상 safe_release해주기
}
