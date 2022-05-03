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
	LPDIRECT3DTEXTURE9	pTexture;	// 객체, 이미지와 관련된 각종 기능을 제공하는 클래스

	D3DXIMAGE_INFO		tImgInfo;	// 구조체, 이미지와 관련된 실제적인 데이터를 저장하는 구조체

}TEXINFO;





static D3DXVECTOR3		Get_Mouse()
{
	POINT	Pt{};

	GetCursorPos(&Pt);
	ScreenToClient(g_hWnd, &Pt);

	return D3DXVECTOR3((float)Pt.x, (float)Pt.y, 0.f);
}