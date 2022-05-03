#pragma once

typedef struct tagInfo
{
	D3DXVECTOR3		vPos;
	D3DXVECTOR3		vDir;
	D3DXVECTOR3		vLook;
	D3DXVECTOR3		vSize;

	D3DXMATRIX		matWorld;
}INFO;

typedef		struct tagTexture
{
	LPDIRECT3DTEXTURE9	pTexture;	// ��ü, �̹����� ���õ� ���� ����� �����ϴ� Ŭ����

	D3DXIMAGE_INFO		tImgInfo;	// ����ü, �̹����� ���õ� �������� �����͸� �����ϴ� ����ü

}TEXINFO;





static D3DXVECTOR3		Get_Mouse()
{
	POINT	Pt{};

	GetCursorPos(&Pt);
	ScreenToClient(g_hWnd, &Pt);

	return D3DXVECTOR3((float)Pt.x, (float)Pt.y, 0.f);
}