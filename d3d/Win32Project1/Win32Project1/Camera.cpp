#include "stdafx.h"
#include "Camera.h"


CCamera::CCamera()
{
}

CCamera::CCamera(CameraType cameraType)
{
	m_eCameraType = cameraType;

	m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vLook = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
}


CCamera::~CCamera()
{
}

void CCamera::walk(float units)
{
	if (0 == units)
		return;

	if (LANDOBJECT == m_eCameraType)
	{
		D3DXVECTOR3 vDir;
		D3DXVECTOR3 vWorld_Up{ 0.f,1.f,0.f };
		D3DXVec3Cross(&vDir, &m_vRight, &vWorld_Up);
		m_vPos += D3DXVECTOR3(vDir.x, 0.f, vDir.z)*units;
	}

	else
	{
		m_vPos += m_vLook*units;
	}
}

void CCamera::fly(float units)
{
	if (AIRCRAFT == m_eCameraType)
	{
		m_vPos += m_vUp*units;
	}
}

void CCamera::strafe(float units)
{
	if (LANDOBJECT == m_eCameraType)
	{
		m_vPos += D3DXVECTOR3(m_vRight.x, 0.0f, m_vRight.z);
	}

	else
	{
		m_vPos += m_vRight*units;
	}
}

void CCamera::pitch(float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &m_vRight, angle);
	
	D3DXVec3TransformCoord(&m_vUp, &m_vUp,&T);
	D3DXVec3TransformCoord(&m_vLook, &m_vLook,&T);
}

void CCamera::yaw(float angle)
{
	D3DXMATRIX T;

	if (LANDOBJECT == m_eCameraType)
		D3DXMatrixRotationY(&T, angle);

	else
		D3DXMatrixRotationAxis(&T, &m_vUp, angle);

	D3DXVec3TransformCoord(&m_vRight, &m_vRight, &T);
	D3DXVec3TransformCoord(&m_vLook, &m_vLook, &T);

}

void CCamera::roll(float angle)
{
	if (AIRCRAFT == m_eCameraType)
	{
		D3DXMATRIX T;
		D3DXMatrixRotationAxis(&T, &m_vLook, angle);

		D3DXVec3TransformCoord(&m_vUp, &m_vUp, &T);
		D3DXVec3TransformCoord(&m_vRight, &m_vRight, &T);

	}
}

void CCamera::getViewMatrix(D3DXMATRIX * V)
{
	D3DXVec3Normalize(&m_vLook, &m_vLook);
	
	D3DXVec3Cross(&m_vUp, &m_vLook, &m_vRight);
	D3DXVec3Normalize(&m_vUp, &m_vUp);

	D3DXVec3Cross(&m_vRight, &m_vUp, &m_vLook);
	D3DXVec3Normalize(&m_vRight, &m_vRight);

	float x = -D3DXVec3Dot(&m_vRight, &m_vPos);
	float y = -D3DXVec3Dot(&m_vUp, &m_vPos);
	float z = -D3DXVec3Dot(&m_vLook, &m_vPos);

	(*V)(0, 0) = m_vRight.x;
	(*V)(1, 0) = m_vRight.y;
	(*V)(2, 0) = m_vRight.z;
	(*V)(3, 0) = x;

	(*V)(0, 1) = m_vUp.x;
	(*V)(1, 1) = m_vUp.y;
	(*V)(2, 1) = m_vUp.z;
	(*V)(3, 1) = y;

	(*V)(0, 2) = m_vLook.x;
	(*V)(1, 2) = m_vLook.y;
	(*V)(2, 2) = m_vLook.z;
	(*V)(3, 2) = z;

	(*V)(0, 3) = 0.f;
	(*V)(1, 3) = 0.f;
	(*V)(2, 3) = 0.f;
	(*V)(3, 3) = 1.f;


}

void CCamera::setCameraType(CameraType cameraType)
{
	m_eCameraType = cameraType;
}

void CCamera::getPosition(D3DXVECTOR3 * pos)
{
	*pos = m_vPos;
}

void CCamera::setPositino(D3DXVECTOR3 * pos)
{
	m_vPos = *pos;
}

void CCamera::getRight(D3DXVECTOR3 * right)
{
	*right = m_vRight;
}

void CCamera::getUp(D3DXVECTOR3 * up)
{
	*up = m_vUp;
}

void CCamera::getLook(D3DXVECTOR3 * look)
{
	*look = m_vLook;
}
