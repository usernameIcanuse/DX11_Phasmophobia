#include "..\Public\RigidBody.h"
#include "Transform.h"
#include "Navigation.h"


CRigidBody::CRigidBody(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CRigidBody::CRigidBody(const CRigidBody & rhs)
	: CComponent(rhs)

{
}


HRESULT CRigidBody::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CRigidBody::Initialize(void * pArg)
{
	if (nullptr != pArg)
	{
		m_RigidBodyDesc.fWeight = ((RIGIDBODYDESC*)pArg)->fWeight;
		m_RigidBodyDesc.pOwnerTransform = ((RIGIDBODYDESC*)pArg)->pOwnerTransform;
	}

	return S_OK;
}

void CRigidBody::Add_Power(_vector _vPower)
{
	_vector vPower = XMLoadFloat4(&m_vCurPower);
	
	vPower += _vPower;
	XMStoreFloat4(&m_vCurPower, vPower);
	m_bGetPower = true;
}

void CRigidBody::Update(_float fTimeDelta, CNavigation* pCurNavi)
{
	Gravity(fTimeDelta);
	Friction();
	Apply_Transform(fTimeDelta, pCurNavi);
	
}

void CRigidBody::Gravity(_float ftimeDelta)
{
	if (true == m_bGetPower && false == m_bOnGround)
	{
		_vector vGravity = XMVectorSet(0.f, 1.f, 0.f, 0.f) * -9.8f*ftimeDelta;

		_vector vCurPower = XMLoadFloat4(&m_vCurPower);
		vCurPower += vGravity;
		XMStoreFloat4(&m_vCurPower, vCurPower);
	}

}

void CRigidBody::Friction()
{
	if (true == m_bGetPower && true == m_bOnGround)
	{
		_vector vFriction = XMLoadFloat4(&m_vCurPower) * -0.5f;

		_vector vCurPower = XMLoadFloat4(&m_vCurPower);
		vCurPower += vFriction;

		if (0.01f > XMVectorGetX(XMVector3Length(vCurPower)))
		{
			vCurPower = XMVectorSet(0.f, 0.f, 0.f, 0.f);
			m_bGetPower = false;
		}

		XMStoreFloat4(&m_vCurPower, vCurPower);
	}
}

void CRigidBody::Apply_Transform(_float fTimeDelta, CNavigation* pCurNavi)
{
	_vector vCurPower = XMLoadFloat4(&m_vCurPower);
	if (DBL_EPSILON < m_RigidBodyDesc.fWeight)
		vCurPower /= m_RigidBodyDesc.fWeight;

	m_RigidBodyDesc.pOwnerTransform->Move(vCurPower, fTimeDelta, m_bOnGround,pCurNavi);
}


CRigidBody * CRigidBody::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRigidBody*		pInstance = new CRigidBody(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRigidBody");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CRigidBody::Clone(void * pArg)
{
	CRigidBody*		pInstance = new CRigidBody(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CRigidBody");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRigidBody::Free()
{
	__super::Free();
}
