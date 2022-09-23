#include "..\Public\Collider.h"
#include "DebugDraw.h"
#include "PipeLine.h"
#include "GameInstance.h"

_ulong		CCollider::g_iNextID = 0;

CCollider::CCollider(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CCollider::CCollider(const CCollider & rhs)
	: CComponent(rhs)
	, m_eCollisionType(rhs.m_eCollisionType)
#ifdef _DEBUG
	, m_pBatch(rhs.m_pBatch)
	, m_pEffect(rhs.m_pEffect)
	, m_pInputLayout(rhs.m_pInputLayout)
#endif
{	
#ifdef _DEBUG
	Safe_AddRef(m_pInputLayout);
#endif
}

HRESULT CCollider::Initialize_Prototype(TYPE eType)
{
	m_eCollisionType = eType;


#ifdef _DEBUG
	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
	m_pEffect = new BasicEffect(m_pDevice);
	m_pEffect->SetVertexColorEnabled(true);

	const void*		pShaderByteCode = nullptr;
	size_t		ShaderByteCodeLength = 0;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &ShaderByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCode, ShaderByteCodeLength, &m_pInputLayout)))
		return E_FAIL;

#endif // _DEBUG

	

	return S_OK;
}

HRESULT CCollider::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	m_iID = g_iNextID++;


	memcpy(&m_ColliderDesc, pArg, sizeof(COLLIDERDESC));

	switch (m_eCollisionType)
	{
	case TYPE_AABB:
		m_pAABB_Original = new BoundingBox(m_ColliderDesc.vTranslation, _float3(m_ColliderDesc.vScale.x * 0.5f, m_ColliderDesc.vScale.y * 0.5f, m_ColliderDesc.vScale.z * 0.5f));
		m_pAABB = new BoundingBox(*m_pAABB_Original);
		break;
	case TYPE_OBB:
		m_pOBB_Original = new BoundingOrientedBox(m_ColliderDesc.vTranslation, _float3(m_ColliderDesc.vScale.x * 0.5f, m_ColliderDesc.vScale.y * 0.5f, m_ColliderDesc.vScale.z * 0.5f), m_ColliderDesc.vRotation);
		m_pOBB = new BoundingOrientedBox(*m_pOBB_Original);
		break;
	case TYPE_SPHERE:
		m_pSphere_Original = new BoundingSphere(m_ColliderDesc.vTranslation, m_ColliderDesc.vScale.x * 0.5f);
		m_pSphere = new BoundingSphere(*m_pSphere_Original);
		break;

	case TYPE_RAY:
		m_tRay = CMath_Utility::Get_MouseRayInWorldSpace();
		m_tRay.fLength = m_ColliderDesc.fRayLength;
		m_fDist = FLT_MAX;
		break;
	}


		
	return S_OK;
}

void CCollider::Update(_fmatrix TransformMatrix)
{
	if (false == m_bEnable)
		return;

	switch (m_eCollisionType)
	{
	case TYPE_AABB:
		m_pAABB_Original->Transform(*m_pAABB, Remove_Rotation(TransformMatrix));
		break;
	case TYPE_OBB:
		m_pOBB_Original->Transform(*m_pOBB, TransformMatrix);
		break;
	case TYPE_SPHERE:
		m_pSphere_Original->Transform(*m_pSphere, TransformMatrix);
		break;
		
	case TYPE_RAY:
		m_tRay = CMath_Utility::Get_MouseRayInWorldSpace();
		m_tRay.fLength = m_ColliderDesc.fRayLength;
		m_fDist = FLT_MAX;
		break;
	}
	GAMEINSTANCE->Add_Collider(this);
	
}

_bool CCollider::Collision(CCollider * pTargetCollider)
{
	
	if (TYPE_OBB == m_eCollisionType)
	{
		switch (pTargetCollider->Get_Collision_Type())
		{
		case TYPE_AABB:
			return m_bColl = m_pOBB->Intersects(*(BoundingBox*)pTargetCollider->Get_Collider());

		case TYPE_OBB:
			return m_bColl = m_pOBB->Intersects(*(BoundingOrientedBox*)pTargetCollider->Get_Collider());

		case TYPE_SPHERE:
			return m_bColl = m_pOBB->Intersects(*(BoundingSphere*)pTargetCollider->Get_Collider());


		case TYPE_RAY:
			RAY _tRay = *(RAY*)pTargetCollider->Get_Collider();
			_float fDist = 0;
			XMStoreFloat3(&_tRay.vDir, XMVector3Normalize(XMLoadFloat3(&_tRay.vDir)));
			if ( m_pOBB->Intersects(XMVectorSetW(XMLoadFloat3(&_tRay.vPos), 1.f), XMVectorSetW(XMLoadFloat3(&_tRay.vDir), 0.f), fDist))
			{
				if (fDist <= m_tRay.fLength)
				{
					XMStoreFloat3(&m_vCollidePos, XMLoadFloat3(&_tRay.vPos) + XMLoadFloat3(&_tRay.vDir) * fDist);
					m_fDist = fDist;
					return m_bColl = true;
				}
			}

		}
	}

	else if (TYPE_SPHERE== m_eCollisionType)
	{
		switch (pTargetCollider->Get_Collision_Type())
		{
		case TYPE_AABB:
			return m_bColl = m_pSphere->Intersects(*(BoundingBox*)pTargetCollider->Get_Collider());

		case TYPE_OBB:
			return m_bColl = m_pSphere->Intersects(*(BoundingOrientedBox*)pTargetCollider->Get_Collider());

		case TYPE_SPHERE:
			return m_bColl = m_pSphere->Intersects(*(BoundingSphere*)pTargetCollider->Get_Collider());


		case TYPE_RAY:
			RAY _tRay = *(RAY*)pTargetCollider->Get_Collider();
			_float fDist = 0;
			XMStoreFloat3(&_tRay.vDir, XMVector3Normalize(XMLoadFloat3(&_tRay.vDir)));
			if ( m_pSphere->Intersects(XMVectorSetW(XMLoadFloat3(&_tRay.vPos), 1.f), XMVectorSetW(XMLoadFloat3(&_tRay.vDir), 0.f), fDist))
			{
				if (fDist <= m_tRay.fLength)
				{
					XMStoreFloat3(&m_vCollidePos, XMLoadFloat3(&_tRay.vPos) + XMLoadFloat3(&_tRay.vDir) * fDist);
					m_fDist = fDist;
					return m_bColl = true;
				}
			}

		}
	}

	else if (TYPE_RAY == m_eCollisionType)
	{
		m_vCollidePos = _float3(0.f, 0.f, 0.f);
		void* pCollider = nullptr;
		_float	fDist=0;
		XMStoreFloat3(&m_tRay.vDir, XMVector3Normalize(XMLoadFloat3(&m_tRay.vDir)));
		
		switch (pTargetCollider->Get_Collision_Type())
		{
	
		case TYPE_OBB:
			 pCollider = (BoundingOrientedBox*)pTargetCollider->Get_Collider();
			if (((BoundingOrientedBox*)pCollider)->Intersects(XMVectorSetW(XMLoadFloat3(&m_tRay.vPos), 1.f), XMVectorSetW(XMLoadFloat3(&m_tRay.vDir), 0.f), fDist))
			{
				if (fDist <= m_tRay.fLength)
				{
					XMStoreFloat3(&m_vCollidePos, XMLoadFloat3(&m_tRay.vPos) + XMLoadFloat3(&m_tRay.vDir) * fDist);
					m_fDist = fDist;
					return m_bColl = true;
				}
			}
			break;

		case TYPE_SPHERE:
			 pCollider = (BoundingSphere*)pTargetCollider->Get_Collider();
			if (((BoundingSphere*)pCollider)->Intersects(XMVectorSetW(XMLoadFloat3(&m_tRay.vPos), 1.f), XMVectorSetW(XMLoadFloat3(&m_tRay.vDir), 0.f), fDist))
			{
				if (fDist <= m_tRay.fLength)
				{
					XMStoreFloat3(&m_vCollidePos, XMLoadFloat3(&m_tRay.vPos) + XMLoadFloat3(&m_tRay.vDir) * fDist);
					m_fDist = fDist;
					return m_bColl = true;
				}
			}
			break;

		}
	}


	return m_bColl = false;
}

_bool CCollider::Collision(RAY _tRay, _float& fDist)
{
	XMStoreFloat3(&_tRay.vDir, XMVector3Normalize(XMLoadFloat3(&_tRay.vDir)));
	if (m_pAABB)
	{
		return  m_pAABB->Intersects(XMVectorSetW(XMLoadFloat3(&_tRay.vPos), 1.f), XMVectorSetW(XMLoadFloat3(&_tRay.vDir), 0.f), fDist);
	}
	if (m_pOBB)
	{
		return m_pOBB->Intersects(XMVectorSetW(XMLoadFloat3(&_tRay.vPos), 1.f), XMVectorSetW(XMLoadFloat3(&_tRay.vDir), 0.f), fDist);
	}
	if (m_pSphere)
	{
		return m_pSphere->Intersects(XMVectorSetW(XMLoadFloat3(&_tRay.vPos), 1.f), XMVectorSetW(XMLoadFloat3(&_tRay.vDir), 0.f), fDist);
	}
}

_matrix CCollider::Remove_Rotation(_fmatrix TransformMatrix)
{
	_matrix			Transform = TransformMatrix;
	
	Transform.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[0]));
	Transform.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[1]));
	Transform.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[2]));

	return Transform;
}

HRESULT CCollider::Render()
{
#ifdef _DEBUG
	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pEffect->SetWorld(XMMatrixIdentity());

	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);	

	m_pEffect->SetView(pPipeLine->Get_Transform(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(pPipeLine->Get_Transform(CPipeLine::D3DTS_PROJ));

	RELEASE_INSTANCE(CPipeLine);

	m_pEffect->Apply(m_pContext);

	_vector		vColor = m_bColl == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f);


	m_pBatch->Begin();

	switch (m_eCollisionType)
	{
	case TYPE_AABB:
		DX::Draw(m_pBatch, *m_pAABB, vColor);
		break;
	case TYPE_OBB:
		DX::Draw(m_pBatch, *m_pOBB, vColor);
		break;
	case TYPE_SPHERE:
		DX::Draw(m_pBatch, *m_pSphere, vColor);
		break;
	}

	m_pBatch->End();
#endif
	return S_OK;
}

CCollider * CCollider::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, TYPE eType)
{
	CCollider*		pInstance = new CCollider(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eType)))
	{
		MSG_BOX("Failed to Created : CCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollider::Clone(void * pArg)
{
	CCollider*		pInstance = new CCollider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CCollider::Free()
{
	__super::Free();

#ifdef _DEBUG
	if (false == m_isCloned)
	{
		Safe_Delete(m_pEffect);
		Safe_Delete(m_pBatch);
	}
		Safe_Release(m_pInputLayout);
#endif


	Safe_Delete(m_pAABB_Original);
	Safe_Delete(m_pOBB_Original);
	Safe_Delete(m_pSphere_Original);

	Safe_Delete(m_pAABB);
	Safe_Delete(m_pOBB);
	Safe_Delete(m_pSphere);

}
