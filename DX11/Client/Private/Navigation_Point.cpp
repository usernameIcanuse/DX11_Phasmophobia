#include "stdafx.h"
#include "../Public/Navigation_Point.h"
#include "GameInstance.h"

CNavigation_Point::CNavigation_Point(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice,pContext)
{
}

CNavigation_Point::CNavigation_Point(const CNavigation_Point& rhs)
    :CGameObject(rhs)
{
}

HRESULT CNavigation_Point::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CNavigation_Point::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Setup_Component()))
        return E_FAIL;
    return S_OK;
}

void CNavigation_Point::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
 
}

void CNavigation_Point::LateTick(_float fTimeDelta)
{
   
}

HRESULT CNavigation_Point::Render()
{
    return S_OK;
}



HRESULT CNavigation_Point::Setup_Component()
{
    /* For.Com_Renderer*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
        return E_FAIL;

    /* For.Com_Shader*/
    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Shader_VtxPointInstace"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    /* For.Com_VIBuffer*/
    if(FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_VIBuffer_Point_Instance"),TEXT("Com_VIBuffer"),(CComponent**)&m_pVIBuffer)))
        return E_FAIL;

    return S_OK;
}



void CNavigation_Point::Free()
{
    __super::Free();
    
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pRendererCom);

    //해당 클래스에 있는 변수들은 항상 safe_release해주기
}
