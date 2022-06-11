#include "stdafx.h"
#include "..\public\Camera.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CCamera)

CCamera::CCamera()
	:m_pDevice(CDevice::Get_Instance()->Get_Device()), LastX(640.f), LastY(360.f)
{
}


CCamera::~CCamera()
{
}

void CCamera::Initialize()
{
	ZeroMemory(&vPos, sizeof(D3DXVECTOR3));
	ZeroMemory(&vLook, sizeof(D3DXVECTOR3));
	vLook.z = 1.f;
	ZeroMemory(&vUp, sizeof(D3DXVECTOR3));
	vUp.y = 1.f;
	ZeroMemory(&vRight, sizeof(D3DXVECTOR3));
	vRight.x = 1.f;

	

}

void CCamera::TransForm(D3DXMATRIX& World)
{
	
	vPos.x = World._41;
	vPos.y = World._42;
	vPos.z = World._43;

	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
	{
		m_bCtrl = false;
		if (!m_bRButton)
		{
			POINT pt;
			GetCursorPos(&pt);

			xoffset = pt.x - LastX;
			yoffset = pt.y - LastY;

			pt = { 640,360 };
			ClientToScreen(g_hWnd, &pt);
			SetCursorPos(pt.x, pt.y);

			LastX = pt.x;
			LastY = pt.y;

			D3DXMATRIX T;

			D3DXMatrixRotationAxis(&T, &vUp, xoffset*0.01f);//좌우 회전

			D3DXVec3TransformCoord(&vRight, &vRight, &T);
			D3DXVec3TransformCoord(&vLook, &vLook, &T);
			

			D3DXMatrixRotationAxis(&T, &vRight, yoffset*0.01f);//상하 회전

			D3DXVec3TransformCoord(&vLook, &vLook, &T);
			D3DXVec3TransformCoord(&vUp, &vUp, &T);
	
			D3DXVec3Normalize(&vLook, &vLook);
			D3DXVec3Normalize(&vRight, &vRight);
			D3DXVec3Normalize(&vUp, &vUp);
			
	
			m_bCtrl = true;
		}
	}
	else 
	{
		m_bCtrl = false;

		vRight.x = World._11;
		vRight.y = World._12;
		vRight.z = World._13;

		vLook.x = World._31;
		vLook.y = World._32;
		vLook.z = World._33;

		vUp.x = World._21;
		vUp.y = World._22;
		vUp.z = World._23;

	}
	vPos-= vLook*5.f;
	vPos+= vUp*3.f;

	ZeroMemory(&vCenter, sizeof(D3DXVECTOR3));

	if (!m_bCtrl&&GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		if (5 > m_iCnt)
		{
			++m_iCnt;
		}
			
		m_bRButton = true;
		
	}
	else
	{	
		if (0 < m_iCnt)
		{
			--m_iCnt;
		}		
		m_bRButton = false;
	}
	vCenter = vLook*m_iCnt;
	vCenter -= vUp*0.6f*m_iCnt;

	vPos += vCenter;
}

void CCamera::TransView()
{
	
	D3DXVECTOR3 vAt = vPos + vLook;

	D3DXMatrixLookAtLH(&View, &vPos, &vAt, &vUp);

	m_pDevice->SetTransform(D3DTS_VIEW, &View);
}

D3DXMATRIX CCamera::GetCameraMatrix()
{
	D3DXMATRIX matTemp;
	D3DXMatrixIdentity(&matTemp);
	matTemp._11 = vRight.x;
	matTemp._12 = vRight.y;
	matTemp._13 = vRight.z;

	matTemp._21 = vUp.x;
	matTemp._22 = vUp.y;
	matTemp._23 = vUp.z;

	matTemp._31 = vLook.x;
	matTemp._32 = vLook.y;
	matTemp._33 = vLook.z;

	matTemp._41 = vPos.x;
	matTemp._42 = vPos.y;
	matTemp._43 = vPos.z;

	return matTemp;
}

