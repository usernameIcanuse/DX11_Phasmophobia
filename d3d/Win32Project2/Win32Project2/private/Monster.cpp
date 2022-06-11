#include "stdafx.h"
#include "..\public\Monster.h"
#include "Device.h"
#include "Struct.h"

CMonster::CMonster(CPlayer* pPlayer)
	:m_pDevice(CDevice::Get_Instance()->Get_Device()),m_pPlayer(pPlayer)
{
}


CMonster::CMonster()
{
}

CMonster::~CMonster()
{
}

void CMonster::Initialize()
{
	m_pDevice->CreateVertexBuffer(
		8 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		VERTEX_FVF,
		D3DPOOL_MANAGED,
		&m_bVertex,
		0);


	m_pDevice->CreateIndexBuffer(
		36 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_bIndex,
		0);

	Vertex*		vertices = nullptr;

	m_bVertex->Lock(0, 0, (void**)&vertices, 0);

	vertices[0] = Vertex(-1.f, -1.f, -1.f, D3DCOLOR_ARGB(255, 0, 150, 150));
	vertices[1] = Vertex(-1.f, 1.f, -1.f, D3DCOLOR_ARGB(255, 0, 150, 150));
	vertices[2] = Vertex(1.f, 1.f, -1.f, D3DCOLOR_ARGB(255, 0, 150, 150));
	vertices[3] = Vertex(1.f, -1.f, -1.f, D3DCOLOR_ARGB(255, 0, 150, 150));
	vertices[4] = Vertex(-1.f, -1.f, 1.f, D3DCOLOR_ARGB(255, 0, 150, 150));
	vertices[5] = Vertex(-1.f, 1.f, 1.f, D3DCOLOR_ARGB(255, 0, 150, 150));
	vertices[6] = Vertex(1.f, 1.f, 1.f, D3DCOLOR_ARGB(255, 0, 150, 150));
	vertices[7] = Vertex(1.f, -1.f, 1.f, D3DCOLOR_ARGB(255, 0, 150, 150));

	m_bVertex->Unlock();

	WORD*		indices = 0;
	m_bIndex->Lock(0, 0, (void**)&indices, 0);
	//front
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	//back
	indices[6] = 4; indices[7] = 6; indices[8] = 5;
	indices[9] = 4; indices[10] = 7; indices[11] = 6;

	//left
	indices[12] = 4; indices[13] = 5; indices[14] = 1;
	indices[15] = 4; indices[16] = 1; indices[17] = 0;

	//right
	indices[18] = 3; indices[19] = 2; indices[20] = 6;
	indices[21] = 3; indices[22] = 6; indices[23] = 7;

	//top
	indices[24] = 1; indices[25] = 5; indices[26] = 6;
	indices[27] = 1; indices[28] = 6; indices[29] = 2;

	//bottom
	indices[30] = 4; indices[31] = 0; indices[32] = 3;
	indices[33] = 4; indices[34] = 3; indices[35] = 7;

	m_bIndex->Unlock();

	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_TargetWorld);

	ZeroMemory(&m_vPos, sizeof(D3DXVECTOR3));
	m_vPos = D3DXVECTOR3(5.f, 5.f, 5.f);
	ZeroMemory(&m_vLook, sizeof(D3DXVECTOR3));
	m_vLook.z = 1.f;
	ZeroMemory(&m_vUp, sizeof(D3DXVECTOR3));
	m_vUp.y = 1.f;
	ZeroMemory(&m_vRight, sizeof(D3DXVECTOR3));
	m_vRight.x = 1.f;

}

void CMonster::Tick()
{
	D3DXVECTOR3 vTarget;
	vTarget.x = m_TargetWorld._41;
	vTarget.y = m_TargetWorld._42;
	vTarget.z = m_TargetWorld._43;

	D3DXVECTOR3 vDist = vTarget - m_vPos;
	D3DXVec3Normalize(&vDist, &vDist);

	m_vLook = vDist;

	D3DXVec3Cross(&m_vRight, &m_vUp, &m_vLook);
	D3DXVec3Normalize(&m_vRight, &m_vRight);

	D3DXVec3Cross(&m_vUp, &m_vLook, &m_vRight);
	D3DXVec3Normalize(&m_vUp, &m_vUp);

	m_vPos += m_vLook*0.05f;

	m_matWorld._11 = m_vRight.x;
	m_matWorld._12 = m_vRight.y;
	m_matWorld._13 = m_vRight.z;

	m_matWorld._21 = m_vUp.x;
	m_matWorld._22 = m_vUp.y;
	m_matWorld._23 = m_vUp.z;

	m_matWorld._31 = m_vLook.x;
	m_matWorld._32 = m_vLook.y;
	m_matWorld._33 = m_vLook.z;

	m_matWorld._41 = m_vPos.x;
	m_matWorld._42 = m_vPos.y;
	m_matWorld._43 = m_vPos.z;

}

void CMonster::LateTick()
{
}

void CMonster::Render()
{
	m_pDevice->GetTransform(D3DTS_WORLD, &m_TargetWorld);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pDevice->SetStreamSource(0, m_bVertex, 0, sizeof(Vertex));
	m_pDevice->SetIndices(m_bIndex);
	m_pDevice->SetFVF(VERTEX_FVF);
	m_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);


}

void CMonster::Release()
{
	m_bIndex->Release();
	m_bVertex->Release();
}
