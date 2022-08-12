#include "stdafx.h"
#include "../Public/Thermometer.h"
#include "GameInstance.h"

CThermometer::CThermometer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CItem(pDevice,pContext)
{
}

CThermometer::CThermometer(const CThermometer& rhs)
    :CItem(rhs)
{
}

HRESULT CThermometer::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CThermometer::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CThermometer::Tick(_float fTimeDelta)
{
}

void CThermometer::LateTick(_float fTimeDelta)
{
}

HRESULT CThermometer::Render()
{
    return S_OK;
}

HRESULT CThermometer::Setup_Component()
{
    return E_NOTIMPL;
}

HRESULT CThermometer::SetUp_ShaderResource()
{
    return S_OK;
}

CThermometer* CThermometer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CThermometer* pInstance = new CThermometer(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CThermometer");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CThermometer::Clone(void* pArg)
{
    CThermometer* pInstance = new CThermometer(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CThermometer");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CThermometer::Free()
{
    __super::Free();
}
