#pragma once
#include "Define.h"

class CTerrain
{
public:
	CTerrain();
	CTerrain(
		int numVertsPerRow,
		int numVertsPerCol,
		int cellSpacing,
		float heightScale);
	~CTerrain();
public:

	int getHeightmapEntry(int row, int col);
	void setHeightmapEntry(int row, int col, int value);

	float getHeight(float x, float y);
	HRESULT	ProcessFrustumCull();//절두체 컬링 위한 인덱스 저장
	bool genTexture(D3DXVECTOR3* directionToLight);
	void draw( bool drawTris = false);

	void Release();
private:
	bool readRawFile();
	bool computeVertices();
	bool computeIndices();
	
	bool lightTerrain(D3DXVECTOR3* directionToLight);
	float computeShade(int cellRow, int cellCol, D3DXVECTOR3* directionToLight);

private:
	struct TerrainVertex
	{
		TerrainVertex() {};
		TerrainVertex(float x, float y, float z, float u, float v)
		{
			_x = x; _y = y; _z = z; _u = u; _v = v;
		}
		float _x, _y, _z;
		float _u, _v;

		static const DWORD FVF;
	};

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	IDirect3DTexture9*		m_pTexture;
	IDirect3DVertexBuffer9* m_pVB;
	IDirect3DIndexBuffer9*	m_pIB;

	int m_iNumVertsPerRow;
	int m_iNumVertsPerCol;
	int m_iCellSpacing;

	int m_iNumCellsPerRow;
	int m_iNumCellsPerCol;
	int m_iWidth;
	int m_iDepth;
	int m_iNumVertices;
	int m_iNumTriangles;

	float m_fHeightScale;

	std::vector<int> m_vheightmap;
};

