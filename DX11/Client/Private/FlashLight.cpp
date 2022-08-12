#include "stdafx.h"
#include "../Public/FlashLight.h"
#include "GameInstance.h"

CFlashLight::CFlashLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CItem(pDevice,pContext)
{
}

CFlashLight::CFlashLight(const CFlashLight& rhs)
    :CItem(rhs)
{
}

HRESULT CFlashLight::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CFlashLight::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CFlashLight::Tick(_float fTimeDelta)
{
}

void CFlashLight::LateTick(_float fTimeDelta)
{
}

HRESULT CFlashLight::Render()
{
    return S_OK;
}

HRESULT CFlashLight::Setup_Component()
{
    return E_NOTIMPL;
}

HRESULT CFlashLight::SetUp_ShaderResource()
{
    return S_OK;
}

CFlashLight* CFlashLight::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CFlashLight* pInstance = new CFlashLight(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CFlashLight");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CFlashLight::Clone(void* pArg)
{
    CFlashLight* pInstance = new CFlashLight(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CFlashLight");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CFlashLight::Free()
{
    __super::Free();
}
