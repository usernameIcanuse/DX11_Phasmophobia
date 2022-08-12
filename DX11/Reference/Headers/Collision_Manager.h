#pragma once
#include "Base.h"

BEGIN(Engine)

class CCollider;
class CGameObject;

class CCollision_Manager final :
    public CBase
{
    DECLARE_SINGLETON(CCollision_Manager)

public:
    CCollision_Manager();
    virtual ~CCollision_Manager() = default;

public:
    void Initialize();
    void Tick();
    void CheckGroup(COLLISION_TYPE _eLeft, COLLISION_TYPE _eRight);
    void Reset();

public:
    void Add_Collider(CCollider* pCollider);
    void Erase_Collider(CCollider* pCollider);
    list<CCollider*>* Get_ColliderList(COLLISION_TYPE _eType);
    void Clear_ColliderList();

private:
    void CollisionGroupUpdate(COLLISION_TYPE _eLeft, COLLISION_TYPE _eRight);

    bool Is3DCollision(CCollider* _pLeft, CCollider* _pRight, _float* _fDistance = nullptr);
    bool IsOBBCollision(CCollider* _pLeft, CCollider* _pRight, _float* _fDistance = nullptr);
    bool IsSphereCollision(CCollider* _pLeft, CCollider* _pRight, _float* _fDistance = nullptr);
    bool IsMesh_To_SphereCollision(CCollider* _pLeft, CCollider* _pRight);
    bool IsRay_To_SphereCollision(CCollider* _pLeft, CCollider* _pRight);
private:
    list<CCollider*>	m_ColliderList[(UINT)COLLISION_TYPE::TYPE_END]; // 충돌 컴포넌트들만 충돌 타입별로 모아놓은 리스트
    list<_ulong>    m_Erase_ColliderIDList;

    UINT m_arrCheck[(UINT)COLLISION_TYPE::TYPE_END];

    map<ULONGLONG, bool> m_mapColInfo;//충돌체 간의 이전 프레임 충돌 정보

public:
    virtual void Free() override;

};

END
