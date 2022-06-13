#include "stdafx.h"
#include "..\public\Player.h"
#include "Device.h"
#include "Camera.h"

CPlayer::CPlayer()
	:m_pDevice(CDevice::Get_Instance()->Get_Device()), LastX(640.f), LastY(360.f)
{
	
}


CPlayer::~CPlayer()
{
	Release(); 
}

void CPlayer::Initialize()
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

	vertices[0] = Vertex(-1.f, -1.f, -1.f, D3DCOLOR_ARGB(255,0,150, 150));
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
	indices[9] = 4; indices[10] =7; indices[11] = 6;

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
	ZeroMemory(&m_vPos, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vLook, sizeof(D3DXVECTOR3));
	m_vLook.z = 1.f;
	ZeroMemory(&m_vUp, sizeof(D3DXVECTOR3));
	m_vUp.y = 1.f;
	ZeroMemory(&m_vRight, sizeof(D3DXVECTOR3));
	m_vRight.x = 1.f;
}

void CPlayer::Tick()
{
	D3DXMatrixIdentity(&m_matWorld);
#pragma region 마우스회전
	SetCursor(NULL);
	if (!GetAsyncKeyState(VK_CONTROL))
	{
		POINT pt;
		GetCursorPos(&pt);


		float xoffset = pt.x - LastX;
		float yoffset = pt.y - LastY;


		D3DXMATRIX T;

		D3DXMatrixRotationAxis(&T, &m_vUp, xoffset*0.01f);//좌우 회전

		D3DXVec3TransformCoord(&m_vLook, &m_vLook, &T);
		D3DXVec3TransformCoord(&m_vRight, &m_vRight, &T);

		D3DXMatrixRotationAxis(&T, &m_vRight, yoffset*0.01f);//상하 회전

		D3DXVec3TransformCoord(&m_vLook, &m_vLook, &T);
		D3DXVec3TransformCoord(&m_vUp, &m_vUp, &T);

		D3DXVec3Normalize(&m_vLook, &m_vLook);
		D3DXVec3Normalize(&m_vRight, &m_vRight);
		D3DXVec3Normalize(&m_vUp, &m_vUp);
		
		pt = { 640,360 };
		ClientToScreen(g_hWnd, &pt);
		SetCursorPos(pt.x, pt.y);

		LastX = pt.x;
		LastY = pt.y;
	}

#pragma endregion 마우스회전
	
#pragma region 방향키 회전
	/*if (GetAsyncKeyState(VK_LEFT))
	{
		D3DXMATRIX T;
		D3DXMatrixRotationAxis(&T, &m_vUp, -0.01f);

		D3DXVec3TransformCoord(&m_vLook, &m_vLook, &T);
		D3DXVec3TransformCoord(&m_vRight, &m_vRight, &T);

		D3DXVec3Normalize(&m_vLook, &m_vLook);
		D3DXVec3Normalize(&m_vRight, &m_vRight);
		D3DXVec3Normalize(&m_vUp, &m_vUp);
	}
	else if (GetAsyncKeyState(VK_RIGHT))
	{
		D3DXMATRIX T;
		D3DXMatrixRotationAxis(&T, &m_vUp, 0.01f);

		D3DXVec3TransformCoord(&m_vLook, &m_vLook, &T);
		D3DXVec3TransformCoord(&m_vRight, &m_vRight, &T);

		D3DXVec3Normalize(&m_vLook, &m_vLook);
		D3DXVec3Normalize(&m_vRight, &m_vRight);
		D3DXVec3Normalize(&m_vUp, &m_vUp);
	}
	if (GetAsyncKeyState(VK_UP))
	{
		D3DXMATRIX T;
		D3DXMatrixRotationAxis(&T, &m_vRight, -0.01f);

		D3DXVec3TransformCoord(&m_vLook, &m_vLook, &T);
		D3DXVec3TransformCoord(&m_vUp, &m_vUp, &T);

		D3DXVec3Normalize(&m_vLook, &m_vLook);
		D3DXVec3Normalize(&m_vRight, &m_vRight);
		D3DXVec3Normalize(&m_vUp, &m_vUp);
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		D3DXMATRIX T;
		D3DXMatrixRotationAxis(&T, &m_vRight, 0.01f);

		D3DXVec3TransformCoord(&m_vLook, &m_vLook, &T);
		D3DXVec3TransformCoord(&m_vUp, &m_vUp, &T);
		
	
	}
	*/
#pragma endregion 방향키 회전

	D3DXVECTOR3 vDir;
	ZeroMemory(&vDir, sizeof(D3DXVECTOR3));
	if (GetAsyncKeyState('A'))
	{
		vDir -= D3DXVECTOR3(m_vRight.x,m_vRight.y,m_vRight.z);

	}
	else if (GetAsyncKeyState('D'))
	{
		vDir += D3DXVECTOR3(m_vRight.x, m_vRight.y, m_vRight.z);

	}

	if (GetAsyncKeyState('W'))
	{
		vDir += D3DXVECTOR3(m_vLook.x, m_vLook.y, m_vLook.z);


	}
	else if (GetAsyncKeyState('S'))
	{
		vDir -= D3DXVECTOR3(m_vLook.x, m_vLook.y, m_vLook.z);

	}

	if (GetAsyncKeyState(VK_SPACE))
	{
		
		m_fJumpSpeed = 1.f;
		
		m_bJump = true;
	}
	D3DXVec3Normalize(&vDir, &vDir);
	m_vPos += vDir*0.1f;
	
	if (m_bJump)
	{
		m_vPos.y += m_fJumpSpeed;
		m_fJumpSpeed += m_fJumpAccel;
	}
	
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

	CCamera::Get_Instance()->TransForm(m_matWorld);
}

void CPlayer::LateTick()
{
	if (m_vPos.y < -1.f)
	{
		m_bJump = false;
		float dy = -1.f - m_vPos.y;
		m_vPos.y += dy;
		m_fJumpSpeed = 0.f;
	}
}

void CPlayer::Render()
{
	
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pDevice->SetStreamSource(0, m_bVertex, 0, sizeof(Vertex));
	m_pDevice->SetIndices(m_bIndex);
	m_pDevice->SetFVF(VERTEX_FVF);
	m_pDevice->SetTransform(D3DTS_WORLD,&m_matWorld);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

}

void CPlayer::Release()
{
	m_bIndex->Release();
	m_bVertex->Release();
}
