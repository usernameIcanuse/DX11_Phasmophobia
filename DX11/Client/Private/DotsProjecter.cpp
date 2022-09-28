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
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

   // m_pTransformCom->Set_Scaled(_float3(5.f, 5.f, 5.f));

    if (FAILED(Setup_Component()))
        return E_FAIL;



    return S_OK;
}

void CDotsProjecter::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
 
    //if (GAMEINSTANCE->Is_KeyState(KEY::E, KEY_STATE::TAP))
    //{
    //    _float4 fDist;
    //    if (Picking((CVIBuffer*)m_pVIBufferCom,fDist))
    //    {
    //        int a = 0;
    //    }
    //}
    m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CDotsProjecter::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

#ifdef _DEBUG
    m_pRendererCom->Add_DebugRenderGroup(m_pOBBCom);
#endif

}

HRESULT CDotsProjecter::Render()
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

        m_pModelCom->Render(i, m_pShaderCom);
    }
//
//#ifdef _DEBUG
//    m_pOBBCom->Render();
//#endif // _DEBUG


    return S_OK;
}


void CDotsProjecter::Set_TempModel_Pos(_float3 vPosition, COLLISION_TYPE eType, _float4 vLook, CItem* pConnectItem)
{
}

void CDotsProjecter::On_Collision_Enter(CCollider* pCollider)
{
    __super::On_Collision_Enter(pCollider);
}

void CDotsProjecter::On_Collision_Stay(CCollider* pCollider)
{
}

void CDotsProjecter::On_Collision_Exit(CCollider* pCollider)
{
}

HRESULT CDotsProjecter::Setup_Component()
{

    if (FAILED(__super::Setup_Component()))
        return E_FAIL;
 
    /* For.Com_Model */
  /*  if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Model_MapleTree"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;*/

    /* For.Com_OBB*/
    CCollider::COLLIDERDESC			ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

    ColliderDesc.vScale = _float3(1.f, 2.f, 1.f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);
    ColliderDesc.pOwner = this;
    ColliderDesc.m_eObjID = COLLISION_TYPE::ITEM;

    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
        return E_FAIL;
   

    return S_OK;
}


HRESULT CDotsProjecter::Setup_TempModel()
{
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

}
