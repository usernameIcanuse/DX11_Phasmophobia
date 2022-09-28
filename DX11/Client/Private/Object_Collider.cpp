#include "stdafx.h"
#include "../Public/Object_Collider.h"
#include "GameInstance.h"

CObject_Collider::CObject_Collider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice,pContext)
{
}

CObject_Collider::CObject_Collider(const CObject_Collider& rhs)
    :CGameObject(rhs)
{
}

HRESULT CObject_Collider::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CObject_Collider::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Setup_Component()))
        return E_FAIL;

    return S_OK;
}

void CObject_Collider::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
 
    m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CObject_Collider::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
   
   // m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
#ifdef _DEBUG
    m_pRendererCom->Add_DebugRenderGroup(m_pOBBCom);
#endif

}

HRESULT CObject_Collider::Render()
{
//#ifdef _DEBUG
//    m_pOBBCom->Render();
//#endif // _DEBUG

    return S_OK;
}


HRESULT CObject_Collider::Setup_Component()
{

    /* For.Com_OBB*/
    CCollider::COLLIDERDESC			ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

    ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);
    ColliderDesc.pOwner = this;
    ColliderDesc.m_eObjID = COLLISION_TYPE::OBJECT;

    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
        return E_FAIL;
    /* For.Com_Renderer*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
        return E_FAIL;
   
    return S_OK;
}


CObject_Collider* CObject_Collider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CObject_Collider* pInstance = new CObject_Collider(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CObject_Collider");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CObject_Collider::Clone(void* pArg)
{
    CObject_Collider* pInstance = new CObject_Collider(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CObject_Collider");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CObject_Collider::Free()
{
    __super::Free();
    
    Safe_Release(m_pOBBCom);
    Safe_Release(m_pRendererCom);

   
    //해당 클래스에 있는 변수들은 항상 safe_release해주기
}
