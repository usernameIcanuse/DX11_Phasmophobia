#include "stdafx.h"
#include "../Public/UVLight.h"
#include "GameInstance.h"

CUVLight::CUVLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CItem(pDevice,pContext)
{
}

CUVLight::CUVLight(const CUVLight& rhs)
    :CItem(rhs)
{
}

HRESULT CUVLight::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUVLight::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CUVLight::Tick(_float fTimeDelta)
{
}

void CUVLight::LateTick(_float fTimeDelta)
{
}

HRESULT CUVLight::Render()
{
    return S_OK;
}

HRESULT CUVLight::Setup_Component()
{
    return E_NOTIMPL;
}

HRESULT CUVLight::SetUp_ShaderResource()
{
    return S_OK;
}

CUVLight* CUVLight::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUVLight* pInstance = new CUVLight(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUVLight");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUVLight::Clone(void* pArg)
{
    CUVLight* pInstance = new CUVLight(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUVLight");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUVLight::Free()
{
    __super::Free();
}
