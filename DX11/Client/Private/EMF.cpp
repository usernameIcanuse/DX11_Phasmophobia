#include "stdafx.h"
#include "../Public/EMF.h"
#include "GameInstance.h"

CEMF::CEMF(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CItem(pDevice,pContext)
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
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CEMF::Tick(_float fTimeDelta)
{
}

void CEMF::LateTick(_float fTimeDelta)
{
}

HRESULT CEMF::Render()
{
    return S_OK;
}

HRESULT CEMF::Setup_Component()
{
    return E_NOTIMPL;
}

HRESULT CEMF::SetUp_ShaderResource()
{
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
}
