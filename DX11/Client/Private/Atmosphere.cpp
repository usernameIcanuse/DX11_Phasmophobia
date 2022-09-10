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
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    //방마다 온도 랜덤 지정

    return S_OK;
}

void CAtmosphere::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
 
}

void CAtmosphere::LateTick(_float fTimeDelta)
{
   
}

HRESULT CAtmosphere::Render()
{
    return S_OK;
}


HRESULT CAtmosphere::Setup_Component()
{
 

 
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
    

    //해당 클래스에 있는 변수들은 항상 safe_release해주기
}
