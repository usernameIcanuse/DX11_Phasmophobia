#pragma once

#include "Component.h"

BEGIN(Engine)

/* 충돌체. */
/* 각종 충돌을 위한 함수를. */
/* 화면에 그려서 보여줄수 있음 좋겄다. */

class ENGINE_DLL CCollider final : public CComponent
{
public:
	enum TYPE { TYPE_SPHERE, TYPE_AABB, TYPE_OBB, TYPE_END };

public:
	typedef struct tagColliderDesc
	{
		_float3			vScale;
		_float4			vRotation;
		_float3			vTranslation;
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



#ifdef _DEBUG
public:
	HRESULT Render();
#endif // _DEBUG

private:
	BoundingSphere*				m_pSphere_Original = nullptr;
	BoundingBox*				m_pAABB_Original = nullptr;
	BoundingOrientedBox*		m_pOBB_Original = nullptr;

	BoundingSphere*				m_pSphere = nullptr;
	BoundingBox*				m_pAABB = nullptr;	
	BoundingOrientedBox*		m_pOBB = nullptr;
	TYPE						m_eType = TYPE_END;
	COLLIDERDESC				m_ColliderDesc;

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