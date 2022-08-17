#include "Collider.h"
#include "GameInstance.h"

_uint CCollider::g_iNextID = 0;

CCollider::CCollider(const CCollider& Prototype)
{
}

void CCollider::Link_Transform(CTransform* _TransformCom)
{
    m_pMyTransformCom = _TransformCom;
    m_pMyTransformCom->Set_WeakPtr(&m_pMyTransformCom);
}

void CCollider::Set_Collision_Type(COLLISION_TYPE _eType)
{
    m_eCollision_Type = _eType;
}

void CCollider::Link_Pre_Collider(CCollider_Pre* _PreCol)
{
    m_pPreCollider = _PreCol;
    WEAK_PTR(m_pPreCollider);
}

CCollider_Pre* CCollider::Get_Pre_Collider() const
{
    return m_pPreCollider;
}

void CCollider::Set_OffSet(_float3 _vOffset)
{
    m_vOffSet = _vOffset;
}

_float3 CCollider::Get_Offset()
{
    return m_vOffSet;
}

void CCollider::Tick(_float fTimeDelta)
{
    GAMEINSTANCE->Add_Collider(this);
}

void CCollider::OnEnable(void* _Arg)
{
    m_eCollision_Type = *(COLLISION_TYPE*)_Arg;
}

void CCollider::OnDisable()
{
    GAMEINSTANCE->Erase_Collider(this);
}


void CCollider::Free()
{
    __super::Free();
    
    if (m_pMesh)
    {
        m_pMesh->Release();

        m_pMesh = nullptr;
    }

    GAMEINSTANCE->Erase_Collider(this);

    RETURN_WEAKPTR(m_pMyTransformCom);
    RETURN_WEAKPTR(m_pPreCollider);

}
