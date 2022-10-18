#include "stdafx.h"
#include "../Public/Atmosphere.h"
#include "GameInstance.h"

CAtmosphere::CAtmosphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice,pContext)
{
}

CAtmosphere::CAtmosphere(const CAtmosphere& rhs)
    :CGameObject(rhs)
{
}

HRESULT CAtmosphere::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CAtmosphere::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(nullptr)))
        return E_FAIL;

    if (nullptr != pArg)
    {
        m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4((_float4x4*)pArg));
    }

    if (FAILED(Setup_Component()))
        return E_FAIL;
    //방마다 온도 랜덤 지정

    m_iRoomDefault = rand() % 5 + 15;

    return S_OK;
}

void CAtmosphere::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
 
    m_iRoomTemperature = m_iRoomDefault - rand() % 5;


    m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());


}

void CAtmosphere::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

#ifdef _DEBUG
   // m_pRendererCom->Add_DebugRenderGroup(m_pOBBCom);
#endif
}

HRESULT CAtmosphere::Render()
{
    return S_OK;
}


HRESULT CAtmosphere::Setup_Component()
{
 
    /*For.Com_OBB*/
    CCollider::COLLIDERDESC  ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

    ColliderDesc.vScale=_float3(30.f, 30.f, 30.f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f,1.f);
    ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);
    ColliderDesc.pOwner = this;
    ColliderDesc.m_eObjID = COLLISION_TYPE::ATMOSPHERE;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
         return E_FAIL;

    /* For.Com_Renderer*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CAtmosphere::SetUp_ShaderResource()
{
    return S_OK;
}

void CAtmosphere::On_Collision_Enter(CCollider* pCollider)
{
   
}

void CAtmosphere::On_Collision_Stay(CCollider* pCollider)
{

}

void CAtmosphere::On_Collision_Exit(CCollider* pCollider)
{
}

CAtmosphere* CAtmosphere::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CAtmosphere* pInstance = new CAtmosphere(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CAtmosphere");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CAtmosphere::Clone(void* pArg)
{
    CAtmosphere* pInstance = new CAtmosphere(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CAtmosphere");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAtmosphere::Free()
{
    __super::Free();
    
    Safe_Release(m_pOBBCom);
    Safe_Release(m_pRendererCom);
    //해당 클래스에 있는 변수들은 항상 safe_release해주기
}
