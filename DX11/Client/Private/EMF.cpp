#include "stdafx.h"
#include "../Public/EMF.h"
#include "GameInstance.h"
#include "Ghost_SpawnPoint.h"

CEMF::CEMF(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CItem(pDevice, pContext)
{
}

CEMF::CEMF(const CEMF& rhs)
    :CItem(rhs)
{
}

HRESULT CEMF::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEMF::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(nullptr)))
        return E_FAIL;

    if (nullptr != pArg)
    {
        m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4((_float4x4*)pArg));
    }

    if (FAILED(Setup_Component()))
        return E_FAIL;

    return S_OK;
}

void CEMF::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    if(m_bSwitch)
        m_iEMFLevel = 1;

    m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CEMF::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    GAMEINSTANCE->Add_Object_For_Culling(this, CRenderer::RENDER_NONALPHABLEND);

#ifdef _DEBUG
    //m_pRendererCom->Add_DebugRenderGroup(m_pOBBCom);
#endif

}

HRESULT CEMF::Render()
{

    _uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        _int    iPassIndex = 2;

        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
            return E_FAIL;
        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
            return E_FAIL;
      /*  if (m_bSwitch)
        {Emissive 없음 만들어 줘야함
            if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_EmissiveTexture", i, aiTextureType_EMISSIVE)))
                return E_FAIL;
            iPassIndex = 3;
        }*/

        
        m_pModelCom->Render(i, m_pShaderCom,iPassIndex);
    }


    if (m_bSwitch)
    {
        wsprintf(m_szDegree, TEXT("EMF : %d"), m_iEMFLevel);
        GAMEINSTANCE->Render_Font(TEXT("Font_Dream"), m_szDegree, _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
    }
//
//#ifdef _DEBUG
//      m_pOBBCom->Render();
//#endif // _DEBUG


    return S_OK;
}

void CEMF::OnEventMessage(const _tchar* pMessage)
{
}


void CEMF::On_Collision_Enter(CCollider* pCollider)
{
    if (COLLISION_TYPE::HOUSE == pCollider->Get_Type())
    {
        m_pCurrNavigation = m_pNaviHouseCom;
    }
}

void CEMF::On_Collision_Stay(CCollider* pCollider)
{
    if (COLLISION_TYPE::GHOST_AREA == pCollider->Get_Type())
    {
        m_iEMFLevel = static_cast<CGhost_SpawnPoint*>(pCollider->Get_Owner())->Get_EMFLevel();
    }
}

void CEMF::On_Collision_Exit(CCollider* pCollider)
{
    if (COLLISION_TYPE::HOUSE == pCollider->Get_Type())
    {
        m_pCurrNavigation = m_pNaviOutSideCom;
    }
}

HRESULT CEMF::Setup_Component()
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

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_EMF"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;

    /* For.Com_OBB*/
    CCollider::COLLIDERDESC			ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

    ColliderDesc.vScale = _float3(0.5f, 1.2f, 0.3f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y*0.5f, ColliderDesc.vScale.z * -0.5f);
    ColliderDesc.pOwner = this;
    ColliderDesc.m_eObjID = COLLISION_TYPE::ITEM;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
        return E_FAIL;

    return S_OK;
}


CEMF* CEMF::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CEMF* pInstance = new CEMF(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CEMF");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CEMF::Clone(void* pArg)
{
    CEMF* pInstance = new CEMF(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CEMF");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CEMF::Free()
{
    __super::Free();
    Safe_Release(m_pNaviHouseCom);
    Safe_Release(m_pNaviOutSideCom);
}
