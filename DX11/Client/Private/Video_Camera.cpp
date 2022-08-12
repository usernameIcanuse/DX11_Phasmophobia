#include "stdafx.h"
#include "../Public/Video_Camera.h"
#include "GameInstance.h"

CVideo_Camera::CVideo_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CItem(pDevice,pContext)
{
}

CVideo_Camera::CVideo_Camera(const CVideo_Camera& rhs)
    :CItem(rhs)
{
}

HRESULT CVideo_Camera::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CVideo_Camera::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CVideo_Camera::Tick(_float fTimeDelta)
{
}

void CVideo_Camera::LateTick(_float fTimeDelta)
{
}

HRESULT CVideo_Camera::Render()
{
    return S_OK;
}

HRESULT CVideo_Camera::Setup_Component()
{
    return E_NOTIMPL;
}

HRESULT CVideo_Camera::SetUp_ShaderResource()
{
    return S_OK;
}

CVideo_Camera* CVideo_Camera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CVideo_Camera* pInstance = new CVideo_Camera(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CVideo_Camera");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CVideo_Camera::Clone(void* pArg)
{
    CVideo_Camera* pInstance = new CVideo_Camera(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CVideo_Camera");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CVideo_Camera::Free()
{
    __super::Free();
}
