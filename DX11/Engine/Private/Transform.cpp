#include "..\Public\Transform.h"
#include "Shader.h"
#include "Navigation.h"

CTransform::CTransform(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_WorldMatrix(rhs.m_WorldMatrix)
{
}

void CTransform::Set_Scaled(_float3 vScale)
{
	_vector		vRight = XMVector3Normalize(Get_State(CTransform::STATE_RIGHT)) * vScale.x;
	_vector		vUp = XMVector3Normalize(Get_State(CTransform::STATE_UP)) * vScale.y;
	_vector		vLook = XMVector3Normalize(Get_State(CTransform::STATE_LOOK)) * vScale.z;

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

_float3 CTransform::Get_Scaled()
{
	return _float3(XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_RIGHT))), 
		XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_UP))),
		XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_LOOK))));	
}

HRESULT CTransform::Initialize_Prototype()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CTransform::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CTransform::Set_ShaderResource(CShader * pShader, const char * pConstantName)
{
	_float4x4		WorldMatrixTP;
	XMStoreFloat4x4(&WorldMatrixTP, XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix)));
	pShader->Set_RawValue(pConstantName, &WorldMatrixTP, sizeof(_float4x4));

	return S_OK;
}

HRESULT CTransform::Go_Straight(_float fTimeDelta, CNavigation* pNaviCom)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	//vPosition += XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;	
	_vector vDirection = XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;
	if (nullptr != pNaviCom)
	{
		/*if (false == pNaviCom->isMove(vPosition))
			return S_OK;*/
		_float fPositionY = 0.f;
		_vector vMovedPosition = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		if (pNaviCom->isMove(vPosition, fPositionY, vDirection, vMovedPosition))
		{
			_float4 vPos;
			XMStoreFloat4(&vPos, vMovedPosition);
			vPos.y = fPositionY;
			vPosition = XMLoadFloat4(&vPos);
		}
		else
			return S_OK;

		//_float fPositionY = 0.f;
		//pNaviCom->isMove(vPosition, fPositionY);

	}
	else
	{
		vPosition += XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	}

	Set_State(CTransform::STATE_TRANSLATION, vPosition);
	
	return S_OK;
}

HRESULT CTransform::Go_Backward(_float fTimeDelta, CNavigation* pNaviCom)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	//vPosition -= XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;
	_vector		vDirection = -1.f*XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	if (nullptr != pNaviCom)
	{
		/*if (false == pNaviCom->isMove(vPosition))
			return S_OK;*/
		_float fPositionY = 0.f;
		_vector vMovedPosition = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		if (pNaviCom->isMove(vPosition, fPositionY, vDirection, vMovedPosition))
		{
			_float4 vPos;
			XMStoreFloat4(&vPos, vMovedPosition);
			vPos.y = fPositionY;
			vPosition = XMLoadFloat4(&vPos);
		}
		else
			return S_OK;
		//_float fPositionY = 0.f;
		//pNaviCom->isMove(vPosition, fPositionY);

	}
	else
	{
		vPosition -= XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	}

	Set_State(CTransform::STATE_TRANSLATION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Left(_float fTimeDelta, CNavigation* pNaviCom)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);

	//vPosition -= XMVector3Normalize(vRight) * m_TransformDesc.fSpeedPerSec * fTimeDelta;
	_vector vDirection = -1.f*XMVector3Normalize(vRight) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	if (nullptr != pNaviCom)
	{
		/*if (false == pNaviCom->isMove(vPosition))
			return S_OK;*/
		_float fPositionY = 0.f;
		_vector vMovedPosition = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		if (pNaviCom->isMove(vPosition, fPositionY, vDirection, vMovedPosition))
		{
			_float4 vPos;
			XMStoreFloat4(&vPos, vMovedPosition);
			vPos.y = fPositionY;
			vPosition = XMLoadFloat4(&vPos);
		}
		else
			return S_OK;
		//_float fPositionY = 0.f;
		//pNaviCom->isMove(vPosition, fPositionY);
	}
	else
	{
		vPosition -= XMVector3Normalize(vRight) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	}

	Set_State(CTransform::STATE_TRANSLATION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Right(_float fTimeDelta, CNavigation* pNaviCom)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);

//	vPosition += XMVector3Normalize(vRight) * m_TransformDesc.fSpeedPerSec * fTimeDelta;
	_vector vDirection = XMVector3Normalize(vRight) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	if (nullptr != pNaviCom)
	{
		/*if (false == pNaviCom->isMove(vPosition))
			return S_OK;*/
		_float fPositionY = 0.f;
		_vector vMovedPosition = XMVectorSet(0.f,0.f,0.f,0.f);
		if (pNaviCom->isMove(vPosition, fPositionY,vDirection,vMovedPosition))
		{
			_float4 vPos;
			XMStoreFloat4(&vPos, vMovedPosition);
			vPos.y = fPositionY;
			vPosition = XMLoadFloat4(&vPos);
		}
		else
			return S_OK;
		/*
		_float fPositionY = 0.f;
		pNaviCom->isMove(vPosition, fPositionY);*/
	}
	else
	{
		vPosition += XMVector3Normalize(vRight) * m_TransformDesc.fSpeedPerSec * fTimeDelta;
	}
	Set_State(CTransform::STATE_TRANSLATION, vPosition);

	return S_OK;
}

void CTransform::Add_Direction(DIR eState)
{
	switch (eState)
	{
	case RIGHT:
		XMStoreFloat3(&m_vMoveDir, XMLoadFloat3(&m_vMoveDir) + Get_State(STATE_RIGHT));
		break;

	case LEFT:
		XMStoreFloat3(&m_vMoveDir, XMLoadFloat3(&m_vMoveDir) - Get_State(STATE_RIGHT));
		break;

	case FRONT:
		XMStoreFloat3(&m_vMoveDir, XMLoadFloat3(&m_vMoveDir) + Get_State(STATE_LOOK));
		break;

	case BACK:
		XMStoreFloat3(&m_vMoveDir, XMLoadFloat3(&m_vMoveDir) - Get_State(STATE_LOOK));
		break;
	}
}

HRESULT CTransform::Move(_float fTimeDelta, CNavigation* pNaviCom)
{
	XMStoreFloat3(&m_vMoveDir, XMVector3Normalize(XMLoadFloat3(&m_vMoveDir))*m_TransformDesc.fSpeedPerSec * fTimeDelta);

	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	

	if (nullptr != pNaviCom)
	{
		/*if (false == pNaviCom->isMove(vPosition))
			return S_OK;*/
		_float fPositionY = 0.f;
		_vector vMovedPosition = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		if (pNaviCom->isMove(vPosition, fPositionY, XMLoadFloat3(&m_vMoveDir), vMovedPosition))
		{
			_float4 vPos;
			XMStoreFloat4(&vPos, vMovedPosition);
			vPos.y = fPositionY;
			vPosition = XMLoadFloat4(&vPos);
		}
		else
			return S_OK;
		/*
		_float fPositionY = 0.f;
		pNaviCom->isMove(vPosition, fPositionY);*/
	}
	else
	{
		vPosition += XMLoadFloat3(&m_vMoveDir);
	}
	Set_State(CTransform::STATE_TRANSLATION, vPosition);

	return S_OK;
}

HRESULT CTransform::Slide_Object(_float3 vContactDirection, CNavigation* pNaviCom)
{
	_vector vDir = XMLoadFloat3(&m_vMoveDir);
	if (DBL_EPSILON < fabs(XMVectorGetX(XMVector3Length(vDir))))
	{
		_vector vContactDir = XMVector3Normalize(XMLoadFloat3(&vContactDirection));
		_vector vSlideDirection = vContactDir * XMVector3Dot(vDir, vContactDir);

		_vector vPosition = Get_State(CTransform::STATE_TRANSLATION) - vDir;


		if (nullptr != pNaviCom)
		{
			/*if (false == pNaviCom->isMove(vPosition))
				return S_OK;*/
			_float fPositionY = 0.f;
			_vector vMovedPosition = XMVectorSet(0.f, 0.f, 0.f, 0.f);
			if (pNaviCom->isMove(vPosition, fPositionY, vSlideDirection, vMovedPosition))
			{
				_float4 vPos;
				XMStoreFloat4(&vPos, vMovedPosition);
				vPos.y = fPositionY;
				vPosition = XMLoadFloat4(&vPos);
			}
			else
				return S_OK;
			/*
			_float fPositionY = 0.f;
			pNaviCom->isMove(vPosition, fPositionY);*/
		}
		else
		{
			vPosition += vSlideDirection;
		}
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}

	return S_OK;
}

void CTransform::Turn(_fvector vAxis, _float fTimeDelta)
{
	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, m_TransformDesc.fRotationPerSec * fTimeDelta);

	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = Get_State(CTransform::STATE_UP);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

// Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.0f));

void CTransform::Rotation(_fvector vAxis, _float fRadian)
{
	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = Get_State(CTransform::STATE_UP);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);


	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	vRight = XMVector3Normalize(vRight);
	vUp = XMVector3Normalize(vUp);
	vLook = XMVector3Normalize(vLook);

	_float3 vScale = Get_Scaled();

	Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
	Set_State(CTransform::STATE_UP, vUp * vScale.y);
	Set_State(CTransform::STATE_LOOK, vLook * vScale.z);

}

void CTransform::LookAt(_fvector vTargetPos)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);

	_vector		vLook = vTargetPos - vPosition;

	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);

	_vector		vUp = XMVector3Cross(vLook, vRight);

	_float3		vScale = Get_Scaled();

	Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
}

void CTransform::Chase(_fvector vTargetPos, _float fTimeDelta)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);

	_vector		vMoveDir = vTargetPos - vPosition;

	vPosition += XMVector3Normalize(vMoveDir) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_TRANSLATION, vPosition);

}



CTransform * CTransform::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTransform*		pInstance = new CTransform(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	CTransform*		pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}
