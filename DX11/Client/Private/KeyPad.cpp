#include "stdafx.h"
#include "../Public/KeyPad.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "SoundMgr.h"

CKeyPad::CKeyPad(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice,pContext)
{
}

CKeyPad::CKeyPad(const CKeyPad& rhs)
    :CGameObject(rhs)
{
}

HRESULT CKeyPad::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CKeyPad::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(nullptr)))
        return E_FAIL;

    if (nullptr != pArg)
    {
        m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4((_float4x4*)pArg));
    }

    if (FAILED(Setup_Component()))
        return E_FAIL;

    return S_OK;
}

void CKeyPad::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

    if (true == m_bClicked)
    {
        CSoundMgr::Get_Instance()->PlaySound(TEXT("truckexitsoundeffect.wav"), CSoundMgr::ITEM_TRUCK, 0.8f,true);
        m_fTimeAcc += fTimeDelta;
        if (m_fTimeAcc > 5.f)
        {
            GAMEINSTANCE->Add_ReserveLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_LOBBY, false, true), LEVEL_LOBBY);
            CSoundMgr::Get_Instance()->StopAll();
        }
    }
 
}

void CKeyPad::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    GAMEINSTANCE->Add_Object_For_Culling(this,CRenderer::RENDER_NONALPHABLEND);

#ifdef _DEBUG
   // m_pRendererCom->Add_DebugRenderGroup(m_pOBBCom);
#endif
}

HRESULT CKeyPad::Render()
{
    _uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        _int    iPassIndex = 2;
        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
        {
            m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture");
        }
        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
        {
            iPassIndex = 0;
        }
        
        m_pModelCom->Render(i, m_pShaderCom, iPassIndex);
    }

    return S_OK;
}



HRESULT CKeyPad::Setup_Component()
{
    /*For.Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Default_Texture"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    /* For.Com_Shader*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    /* For.Com_Model*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_KeyPad"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;

    /* For.Com_OBB*/
    CCollider::COLLIDERDESC  ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

    ColliderDesc.vScale = _float3(0.3f, 0.2f, 0.3f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);
    ColliderDesc.pOwner = this;
    ColliderDesc.m_eObjID = COLLISION_TYPE::ITEM;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
        return E_FAIL;

#ifdef _DEBUG
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
        return E_FAIL;
#endif
   
    return S_OK;
}

void CKeyPad::On_Collision_Enter(CCollider* pCollider)
{
}

void CKeyPad::On_Collision_Stay(CCollider* pCollider)
{
    if (COLLISION_TYPE::SIGHT == pCollider->Get_Type())
    {
        if (GAMEINSTANCE->Is_KeyState(KEY::LBUTTON, KEY_STATE::TAP))
        {
            m_bClicked = true;
        }
    }
}

void CKeyPad::On_Collision_Exit(CCollider* pCollider)
{
}

HRESULT CKeyPad::SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix)
{
    if (nullptr == m_pShaderCom||
        nullptr == m_pModelCom)
        return E_FAIL;


    if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", pViewMatrix, sizeof(_float4x4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", pProjMatrix, sizeof(_float4x4))))
        return E_FAIL;


    return S_OK;
}

CKeyPad* CKeyPad::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CKeyPad* pInstance = new CKeyPad(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CKeyPad");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CKeyPad::Clone(void* pArg)
{
    CKeyPad* pInstance = new CKeyPad(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CKeyPad");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CKeyPad::Free()
{
    __super::Free();
    
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pOBBCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pTextureCom);

#ifdef _DEBUG
    Safe_Release(m_pRendererCom);
#endif
   
    //해당 클래스에 있는 변수들은 항상 safe_release해주기
}
