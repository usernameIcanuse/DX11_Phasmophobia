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
