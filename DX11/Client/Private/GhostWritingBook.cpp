#include "stdafx.h"
#include "../Public/GhostWritingBook.h"
#include "GameInstance.h"

CGhostWritingBook::CGhostWritingBook(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CItem(pDevice,pContext)
{
}

CGhostWritingBook::CGhostWritingBook(const CGhostWritingBook& rhs)
    :CItem(rhs)
{
}

HRESULT CGhostWritingBook::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CGhostWritingBook::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CGhostWritingBook::Tick(_float fTimeDelta)
{
}

void CGhostWritingBook::LateTick(_float fTimeDelta)
{
}

HRESULT CGhostWritingBook::Render()
{
    return S_OK;
}

HRESULT CGhostWritingBook::Setup_Component()
{
    return E_NOTIMPL;
}

HRESULT CGhostWritingBook::SetUp_ShaderResource()
{
    return S_OK;
}

CGhostWritingBook* CGhostWritingBook::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CGhostWritingBook* pInstance = new CGhostWritingBook(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CGhostWritingBook");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CGhostWritingBook::Clone(void* pArg)
{
    CGhostWritingBook* pInstance = new CGhostWritingBook(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CGhostWritingBook");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CGhostWritingBook::Free()
{
    __super::Free();
}
