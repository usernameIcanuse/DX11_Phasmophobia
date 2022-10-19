#include "stdafx.h"
#include "../Public/KeyBoard.h"
#include "GameInstance.h"

CKeyBoard::CKeyBoard(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice,pContext)
{
}

CKeyBoard::CKeyBoard(const CKeyBoard& rhs)
    :CGameObject(rhs)
{
}

HRESULT CKeyBoard::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CKeyBoard::Initialize(void* pArg)
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

void CKeyBoard::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
 
    m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
    m_bClicked = false;
}

void CKeyBoard::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    GAMEINSTANCE->Add_Object_For_Culling(this,CRenderer::RENDER_NONALPHABLEND);
#ifdef _DEBUG
   // m_pRendererCom->Add_DebugRenderGroup(m_pOBBCom);
#endif
}

HRESULT CKeyBoard::Render()
{
    _uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        _int    iPassIndex = 2;
        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
            return E_FAIL;
        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
        {
            iPassIndex = 0;
        }
        
        m_pModelCom->Render(i, m_pShaderCom, iPassIndex);
    }

    return S_OK;
}



HRESULT CKeyBoard::Setup_Component()
{
    /* For.Com_Shader*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    /* For.Com_Model*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_KeyBoard"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;

    /* For.Com_OBB*/
    CCollider::COLLIDERDESC  ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

    ColliderDesc.vScale = _float3(0.5f, 0.05f, 0.2f);
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

void CKeyBoard::On_Collision_Enter(CCollider* pCollider)
{
}

void CKeyBoard::On_Collision_Stay(CCollider* pCollider)
{
    if (COLLISION_TYPE::SIGHT == pCollider->Get_Type())
    {
        if (GAMEINSTANCE->Is_KeyState(KEY::LBUTTON, KEY_STATE::TAP))
        {
            m_bClicked = true;
        }
    }
}

void CKeyBoard::On_Collision_Exit(CCollider* pCollider)
{
}

HRESULT CKeyBoard::SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix)
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

CKeyBoard* CKeyBoard::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CKeyBoard* pInstance = new CKeyBoard(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CKeyBoard");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CKeyBoard::Clone(void* pArg)
{
    CKeyBoard* pInstance = new CKeyBoard(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CKeyBoard");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CKeyBoard::Free()
{
    __super::Free();
    
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pOBBCom);
    Safe_Release(m_pModelCom);

#ifdef _DEBUG
    Safe_Release(m_pRendererCom);
#endif
   
    //해당 클래스에 있는 변수들은 항상 safe_release해주기
}
