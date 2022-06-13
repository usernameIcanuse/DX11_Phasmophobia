#pragma once
#include "Define.h"

class CZFrustum
{
	DECLARE_SINGLETON(CZFrustum)

public:
	CZFrustum();
	~CZFrustum();

public:
	bool Make(D3DXMATRIX* pmatViewProj);//���� ����� �޾ƿͼ� ����ü ����
	bool IsIn(D3DXVECTOR3* vp);//�� ����
	bool IsInsPhere(D3DXVECTOR3* vp, float radius);//��豸 ����
	void Draw();

	void Release();
	void LateTick();
	

private:
	LPDIRECT3DDEVICE9 m_pDevice;
private:
	D3DXVECTOR3 m_vtx[8];//����ü ����
	D3DXVECTOR3 m_vPos;//���� ī�޶� ��ǥ
	D3DXPLANE m_pPlane[6];//����ü �� ���


};

