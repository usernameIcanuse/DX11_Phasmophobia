#include "stdafx.h"
#include "..\public\ZFrustum.h"
#include "Camera.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CZFrustum)

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
	m_vtx[1].x = 1.f; m_vtx[1].y = -1.f; m_vtx[1].z = 0.f;
	m_vtx[2].x = 1.f; m_vtx[2].y = -1.f; m_vtx[2].z = 1.f;
	m_vtx[3].x = -1.f; m_vtx[3].y = -1.f; m_vtx[3].z = 1.f;
	m_vtx[4].x = -1.f; m_vtx[4].y = 1.f; m_vtx[4].z = 0.f;
	m_vtx[5].x = 1.f; m_vtx[5].y = 1.f; m_vtx[5].z = 0.f;
	m_vtx[6].x = 1.f; m_vtx[6].y = 1.f; m_vtx[6].z = 1.f;
	m_vtx[7].x = -1.f; m_vtx[7].y = 1.f; m_vtx[7].z = 1.f;
	
	D3DXMatrixInverse(&matInv, NULL, pmatViewProj);

	for (int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vtx[i], &m_vtx[i], &matInv);
	}

	m_vPos = CCamera::Get_Instance()->GetCameraPos();

	D3DXPlaneFromPoints(&m_pPlane[0], m_vtx + 4, m_vtx + 7, m_vtx + 6);//상
	D3DXPlaneFromPoints(&m_pPlane[1], m_vtx , m_vtx + 1, m_vtx + 2);//하
	D3DXPlaneFromPoints(&m_pPlane[2], m_vtx , m_vtx + 4, m_vtx + 5);//근
	D3DXPlaneFromPoints(&m_pPlane[3], m_vtx + 2, m_vtx + 6, m_vtx + 7);//원
	D3DXPlaneFromPoints(&m_pPlane[4], m_vtx, m_vtx + 3, m_vtx + 7);//좌
	D3DXPlaneFromPoints(&m_pPlane[5], m_vtx +1, m_vtx + 5, m_vtx + 6);//우



	return true;
}

bool CZFrustum::IsIn(D3DXVECTOR3 * vp)
{
	float fDist;

	for (int i = 0; i < 6; ++i)
	{
		fDist = D3DXPlaneDotCoord(&m_pPlane[i], vp);
		if (fDist > 5.f) return false;
	}

	return true;
}

bool CZFrustum::IsInsPhere(D3DXVECTOR3 * vp, float radius)
{
	float fDist;

	for (int i = 0; i < 6; ++i)
	{
		fDist = D3DXPlaneDotCoord(&m_pPlane[i], vp);
		if (fDist >radius+ 5.f) return false;
	}

	return true;

}

void CZFrustum::Draw()
{
	WORD		index[] = { 0, 1, 2,
		0, 2, 3,
		4, 7, 6,
		4, 6, 5,
		1, 5, 6,
		1, 6, 2,
		0, 3, 7,
		0, 7, 4,
		0, 4, 5,
		0, 5, 1,
		3, 7, 6,
		3, 6, 2 };

	m_pDevice->SetFVF(D3DFVF_XYZ);
	m_pDevice->SetStreamSource(0, NULL, 0, sizeof(D3DXVECTOR3));
	m_pDevice->SetTexture(0, NULL);
	m_pDevice->SetIndices(0);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 12, index, D3DFMT_INDEX16, m_vtx, sizeof(m_vtx[0]));

}

void CZFrustum::Release()
{
}

void CZFrustum::LateTick()
{
	D3DXMATRIX View,Projection;
	m_pDevice->GetTransform(D3DTS_VIEW, &View);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &Projection);

	D3DXMATRIX Mat = View * Projection;

	Make(&Mat);

}

