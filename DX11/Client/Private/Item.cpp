#include "stdafx.h"
#include "../Public/Item.h"
#include "GameInstance.h"

CItem::CItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice,pContext)
{
}

CItem::CItem(const CItem& rhs)
    :CGameObject(rhs)
{
}

HRESULT CItem::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CItem::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    GAMEINSTANCE->Add_EventObject(CGame_Manager::EVENT_ITEM, this);
    m_pEventFunc = std::bind(&CItem::Normal_Operation, std::placeholders::_1, std::placeholders::_2);

    return S_OK;
}

void CItem::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
 
}

void CItem::LateTick(_float fTimeDelta)
{
   
}

HRESULT CItem::Render()
{
    return S_OK;
}

void CItem::OnEventMessage(const _tchar* pMessage)
{
    if ( 0 == lstrcmp(TEXT("Event"),pMessage))
    {
        m_pEventFunc = std::bind(&CItem::MalFunction, std::placeholders::_1, std::placeholders::_2);

    }
    else if (0 == lstrcmp(TEXT("Normal_Operation"), pMessage))
    {
        m_pEventFunc = std::bind(&CItem::Normal_Operation, std::placeholders::_1, std::placeholders::_2);
    }
}

_float3 CItem::Get_AdjustPos()
{
    return m_vAdjustpos;
}

void CItem::Update_Collider()
{
    m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CItem::Call_EventFunc(_float fTimeDelta)
{
    m_pEventFunc(this, fTimeDelta);
}


HRESULT CItem::Setup_Component()
{
    /* For.Com_Renderer*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
        return E_FAIL;

    /* For.Com_Shader*/
    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Shader_VtxModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

 
    return S_OK;
}

HRESULT CItem::SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix)
{
    if (nullptr == m_pShaderCom ||
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

void CItem::On_Collision_Enter(CCollider* pCollider)
{
   
}

void CItem::On_Collision_Stay(CCollider* pCollider)
{

}

void CItem::On_Collision_Exit(CCollider* pCollider)
{
}

void	CItem::Adjust_Item(CTransform* _pPlayerTransform)
{
    Set_Enable(true);

    _vector     vPlayerPos = _pPlayerTransform->Get_State(CTransform::STATE_TRANSLATION);

    _vector     vRight = _pPlayerTransform->Get_State(CTransform::STATE_RIGHT);
    _vector		vUp = _pPlayerTransform->Get_State(CTransform::STATE_UP);
    _vector		vLook = _pPlayerTransform->Get_State(CTransform::STATE_LOOK);

    vPlayerPos += XMVector3Normalize(vRight) * m_vAdjustpos.x;
    vPlayerPos -= XMVector3Normalize(vUp) * m_vAdjustpos.y;
    vPlayerPos += vLook * m_vAdjustpos.z;


    m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPlayerPos + XMVectorSet(0.f, 10.f, 0.f, 0.f));
    m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
    m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);
    m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);

    Update_Collider();
}


void CItem::Free()
{
    __super::Free();
    
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pRendererCom);
    //Safe_Release(m_pTextureCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pOBBCom);
    //해당 클래스에 있는 변수들은 항상 safe_release해주기
}
