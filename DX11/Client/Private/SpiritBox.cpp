#include "stdafx.h"
#include "../Public/SpiritBox.h"
#include "GameInstance.h"

CSpiritBox::CSpiritBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CItem(pDevice,pContext)
{
}

CSpiritBox::CSpiritBox(const CSpiritBox& rhs)
    :CItem(rhs)
{
}

HRESULT CSpiritBox::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSpiritBox::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CSpiritBox::Tick(_float fTimeDelta)
{
}

void CSpiritBox::LateTick(_float fTimeDelta)
{
}

HRESULT CSpiritBox::Render()
{
    return S_OK;
}

HRESULT CSpiritBox::Setup_Component()
{
    return E_NOTIMPL;
}

HRESULT CSpiritBox::SetUp_ShaderResource()
{
    return S_OK;
}

CSpiritBox* CSpiritBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSpiritBox* pInstance = new CSpiritBox(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CSpiritBox");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSpiritBox::Clone(void* pArg)
{
    CSpiritBox* pInstance = new CSpiritBox(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CSpiritBox");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSpiritBox::Free()
{
    __super::Free();
}
