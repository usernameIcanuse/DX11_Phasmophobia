#include "stdafx.h"
#include "..\public\ZFrustum.h"
#include "Camera.h"
#include "Device.h"


CZFrustum::CZFrustum()
	:m_pDevice(CDevice::Get_Instance()->Get_Device())
{
	ZeroMemory(m_vtx, sizeof(m_vtx[0]) * 8);
	ZeroMemory(m_pPlane, sizeof(m_pPlane[0]) * 6);

}


CZFrustum::~CZFrustum()
{
	Release();
}

bool CZFrustum::Make(D3DXMATRIX * pmatViewProj)
{
	D3DXMATRIX	matInv;

	m_vtx[0].x = -1.f; m_vtx[0].y = -1.f; m_vtx[0].z = 0.f;
	m_vtx[0].x = -1.f; m_vtx[0].y = -1.f; m_vtx[0].z = 0.f;
	m_vtx[0].x = -1.f; m_vtx[0].y = -1.f; m_vtx[0].z = 0.f;
	m_vtx[0].x = -1.f; m_vtx[0].y = -1.f; m_vtx[0].z = 0.f;
	m_vtx[0].x = -1.f; m_vtx[0].y = -1.f; m_vtx[0].z = 0.f;
	m_vtx[0].x = -1.f; m_vtx[0].y = -1.f; m_vtx[0].z = 0.f;
	m_vtx[0].x = -1.f; m_vtx[0].y = -1.f; m_vtx[0].z = 0.f;
	m_vtx[0].x = -1.f; m_vtx[0].y = -1.f; m_vtx[0].z = 0.f;
	
	return false;
}

bool CZFrustum::IsIn(D3DXVECTOR3 * vp)
{
	return false;
}

bool CZFrustum::IsInsPhere(D3DXVECTOR3 * vp, float radius)
{
	return false;
}

void CZFrustum::Draw()
{
}

void CZFrustum::Release()
{
}
