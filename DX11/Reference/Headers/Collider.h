#pragma once
#include "Component.h"

BEGIN(Engine)

class CTransform;
class CCollider_Pre;

class ENGINE_DLL CCollider abstract :
    public CComponent
{
protected:
    CCollider() = default;
    CCollider(const CCollider& Prototype);
    virtual ~CCollider() = default;

public:
    void Link_Transform(CTransform * _TransformCom);

    virtual void Set_Collider_Size(const _float3& _Size) PURE;
    virtual _float3 Get_Collider_Size() PURE;
    virtual _float3 Get_Collider_Position() PURE;
    

    void Set_Collision_Type(COLLISION_TYPE _eType);
    COLLISION_TYPE Get_Collision_Type() const { return m_eCollision_Type; };
    _uint Get_ID() const { return m_iID; };
    COLLIDER_SHAPE Get_Collider_Shape() const { return m_eShape; };

    void Link_Pre_Collider(CCollider_Pre* _PreCol);
    CCollider_Pre* Get_Pre_Collider() const;
    void Set_OffSet(_float3 _vOffset);
    _float3 Get_Offset();

public:
    virtual void Tick(_float fTimeDelta) override;


protected:
    COLLISION_TYPE m_eCollision_Type;
    COLLIDER_SHAPE m_eShape = COLLIDER_SHAPE::SHAPE_END;

    static _uint g_iNextID;
    _uint m_iID; //충돌체 고유 인덱스 (모든 충돌체는 인덱스가 서로 다름)

    _float3 m_vOffSet;
protected:
    CTransform* m_pMyTransformCom = nullptr;
    ID3DXMesh* m_pMesh = nullptr;
private:
    CCollider_Pre* m_pPreCollider = nullptr;

public:
public:
    //객체의 상태가 활성화 상태로 변경될 때, 호출되는 이벤트입니다.
    virtual void OnEnable(void* _Arg = nullptr) override;

    //객체의 상태가 비활성화 상태로 변경될 때, 호출되는 이벤트입니다.
    virtual void OnDisable() override;

public:
    virtual	CComponent* Clone(void* pArg) PURE;
    virtual void Free() override;

};

END