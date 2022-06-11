#pragma once

typedef struct Vertex
{
	Vertex(float _x, float _y, float _z,D3DCOLOR color)
	{
		vPos.x = _x;
		vPos.y = _y;
		vPos.z = _z;

		_color = color;
	}

	D3DXVECTOR3		vPos;
	
	D3DCOLOR		_color;
	
}Vertex;

typedef struct Vertex_Tex
{
	Vertex_Tex(D3DXVECTOR3 Pos,/* D3DXVECTOR3 Normal, */float u, float v)
	{
		vPos = Pos;
		//vNormal = Normal;
		_u = u; _v = v;
	}
	D3DXVECTOR3 vPos;
	//D3DXVECTOR3		vNormal;
	float _u, _v;
}Vertex_Tex;

#define VERTEX_FVF		(D3DFVF_XYZ | D3DFVF_DIFFUSE)
#define VTXTEX_FVF		(D3DFVF_XYZ | D3DFVF_TEX1)