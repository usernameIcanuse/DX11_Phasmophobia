#pragma once

#include "Component.h"

BEGIN(Engine)

class CGameObject;
/* 충돌체. */
/* 각종 충돌을 위한 함수를. */
/* 화면에 그려서 보여줄수 있음 좋겄다. */

class ENGINE_DLL CCollider final : public CComponent
{
public:
	enum TYPE { TYPE_SPHERE, TYPE_AABB, TYPE_OBB, TYPE_RAY,TYPE_END };

public:
	typedef struct tagColliderDesc
	{
		_float3			vScale;
		_float4			vRotation;
		_float3			vTranslation;
		CGameObject*    pOwner;
		COLLISION_TYPE				m_eObjID;
		_float			fRayLength;

	}COLLIDERDESC;

private:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype(TYPE eType);
	virtual HRESULT Initialize(void* pArg);
public:
	void Update(_fmatrix TransformMatrix);
	_bool Collision(CCollider* pTargetCollider);
	_bool Collision(RAY _tRay,_float& fDist);//Ray Collision
	_matrix Remove_Rotation(_fmatrix TransformMatrix);

public:
	_float	Get_Collision_Dist()
	{
		return m_fDist;
	}

	_float3 Get_Scale()
	{
		return m_ColliderDesc.vScale;
	}

	_ulong Get_ID()
	{
		return m_iID;
	}

	CGameObject* Get_Owner()
	{
		return m_ColliderDesc.pOwner;
	}


	COLLISION_TYPE Get_Type()
	{
		return m_ColliderDesc.m_eObjID;
	}
	TYPE	Get_Collision_Type()
	{
		return m_eCollisionType;
	}

	void* Get_Collider()
	{
		switch (m_eCollisionType)
		{
		case TYPE_AABB:
			return m_pAABB;
			 
		case TYPE_OBB:
			return m_pOBB;

		case TYPE_SPHERE:
			return m_pSphere;

		case TYPE_RAY:
			return &m_tRay;
		}
	}

	_float3 Get_CollidePos()
	{
		return m_vCollidePos;
	}
public:
	HRESULT Render();


private:
	BoundingSphere*				m_pSphere_Original = nullptr;
	BoundingBox*				m_pAABB_Original = nullptr;
	BoundingOrientedBox*		m_pOBB_Original = nullptr;

	BoundingSphere*				m_pSphere = nullptr;
	BoundingBox*				m_pAABB = nullptr;	
	BoundingOrientedBox*		m_pOBB = nullptr;
	TYPE						m_eCollisionType = TYPE_END;
	COLLIDERDESC				m_ColliderDesc;
	
	/*Ray일 때만*/
	RAY							m_tRay;
	_float3						m_vCollidePos;
	_float						m_fDist= FLT_MAX;

	static _ulong				g_iNextID;
	_ulong						m_iID;

	_bool						m_bColl = false;

#ifdef _DEBUG
private:
	BasicEffect*									m_pEffect = nullptr;	
	PrimitiveBatch<VertexPositionColor>*			m_pBatch = nullptr;
	ID3D11InputLayout*								m_pInputLayout = nullptr;
#endif // _DEBUG

public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END