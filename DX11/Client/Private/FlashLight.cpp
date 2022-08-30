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
    /* For.Com_Shader*/
    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Shader_VtxModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    /* For.Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Sky"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    /* For.Com_Renderer*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Model_FlashLight"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;

    return S_OK;
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
