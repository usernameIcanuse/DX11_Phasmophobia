#include "stdafx.h"
#include "../Public/Screen.h"
#include "GameInstance.h"
#include "Computer.h"

CScreen::CScreen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice,pContext)
{
}

CScreen::CScreen(const CScreen& rhs)
    :CGameObject(rhs)
{
}

HRESULT CScreen::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CScreen::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(nullptr)))
        return E_FAIL;

    if (nullptr != pArg)
    {
        m_pComputer = (CComputer*)pArg;
        Safe_AddRef(m_pComputer);
        CTransform* pComputerTransform = (CTransform*)m_pComputer->Get_Component(CGameObject::m_pTransformTag);

        m_pTransformCom->Set_WorldMatrix(pComputerTransform->Get_WorldMatrix());
        m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), -1.f*XMConvertToRadians(76.55f));
        m_pTransformCom->Rotation(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), XMConvertToRadians(4.1f));
        _matrix WorldMat = m_pTransformCom->Get_WorldMatrix();
       
        WorldMat.r[3] = WorldMat.r[3] - WorldMat.r[2]*0.008f- WorldMat.r[1]*0.01f;

        m_pTransformCom->Set_WorldMatrix(WorldMat);
   
        m_pTransformCom->Set_Scaled(_float3(5.5f, 3.5f, 1.f));
            
    }

    if (FAILED(Setup_Component()))
        return E_FAIL;

    return S_OK;
}

void CScreen::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
 
}

void CScreen::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

   m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT,this);
}

HRESULT CScreen::Render()
{

    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Render();


    return S_OK;
}



HRESULT CScreen::Setup_Component()
{
    /* For.Com_Shader*/
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    /* For.Com_VIBuffer*/
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_NorRect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    /*For.Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Black"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    /*For.Com_Renderer*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
        return E_FAIL;
  

    return S_OK;
}

HRESULT CScreen::SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix)
{
    if (nullptr == m_pShaderCom||
        nullptr == m_pVIBufferCom)
        return E_FAIL;


    if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", pViewMatrix, sizeof(_float4x4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", pProjMatrix, sizeof(_float4x4))))
        return E_FAIL;

    if (FAILED(m_pComputer->Set_Screen_RSV(m_pShaderCom)))
    {
        m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 0);
    }


    return S_OK;
}

CScreen* CScreen::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CScreen* pInstance = new CScreen(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CScreen");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CScreen::Clone(void* pArg)
{
    CScreen* pInstance = new CScreen(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CScreen");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CScreen::Free()
{
    __super::Free();
    
    Safe_Release(m_pComputer);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pRendererCom);
   
    //해당 클래스에 있는 변수들은 항상 safe_release해주기
}
