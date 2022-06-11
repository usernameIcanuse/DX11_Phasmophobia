#include "stdafx.h"
#include "Terrain.h"
#include "Define.h"
#include <fstream>

const DWORD	CTerrain::TerrainVertex::FVF = D3DFVF_XYZ | D3DFVF_TEX1;

CTerrain::CTerrain(IDirect3DDevice9 * device, std::string heightmapFileName, int numVertsPerRow, int numVertsPerCol, int cellSpacing, float heightScale)
{
	_device = device;
	_numVertsPerRow = numVertsPerRow;
	_numVertsPerCol = numVertsPerCol;
	_cellSpacing = cellSpacing;

	_numCellsPerRow = _numVertsPerRow - 1;
	_numCellsPerCol = _numVertsPerCol - 1;

	_width = _numCellsPerRow * _cellSpacing;
	_depth = _numCellsPerCol * _cellSpacing;

	_numVertices = _numVertsPerRow * _numVertsPerCol;//전체 점 개수
	_numTriangles = _numCellsPerRow * _numCellsPerCol * 2;//전체 삼각형 개수

	_heightScale = heightScale;

	if (!readRawFile(heightmapFileName))
	{
		ERR_MSG(0, "readRawFile - FAILED", 0, 0);
		PostQuitMessage(0);
	}

	for (int i = 0; i < _heightmap.size(); ++i)
		_heightmap[i] *= heightScale;

	if (!computeVertices())
	{
		ERR_MSG(0, "computeVertices- FAILED", 0, 0);
		PostQuitMessage(0);
	}

	if (computeIndices())
	{
		ERR_MSG(0, "computeIndices - FAILED", 0, 0);
		PostQuitMessage(0);

	}
}

CTerrain::~CTerrain()
{
	Release();
}

int CTerrain::getHeightmapEntry(int row, int col)
{
	return _heightmap[row * _numVertsPerRow +col];
}

void CTerrain::setHeightmapEntry(int row, int col, int value)
{
	_heightmap[row * _numVertsPerRow + col] = value;
}

float CTerrain::getHeight(float x, float y)
{
	x = ((float)_width / 2.0f) + x;
	y = ((float)_depth / 2.0f) + y;

	x /= (float)_cellSpacing;
	y /= (float)_cellSpacing;

	float col = ::floorf(x);
	float row = ::floorf(y);

	float A = getHeightmapEntry(row, col);
	float B = getHeightmapEntry(row, col + 1);
	float C = getHeightmapEntry(row+1, col);
	float D = getHeightmapEntry(row+1, col+1);


	float dx = x - col;
	float dy = y - row;

	float height = 0.0f;

	if (dy < 1.0f - dx)
	{
		float uy = B - A;
		float vy = C - A;

		height = A + d3d::Lerp(0.0f, uy, dx) + d3d::Lerp(0.0f, vy, dy);
	}
	else
	{
		float uy = C - D;
		float vy = B - D;

		height = D+ d3d::Lerp(0.0f, uy, 1.0f - dx) + d3d::Lerp(0.0f, vy,1.0f- dy);
	}

	return height;
}

bool CTerrain::loadTexture(std::string fileName)
{
	if(FAILED(D3DXCreateTextureFromFile(
		_device,
		(LPCWSTR)fileName.c_str(),
		&_tex
	)));
	{
		ERR_MSG(0, "CreateTextureFromFile - FAILED", 0, 0);
		return false;
	}

	return true;
}

bool CTerrain::genTexture(D3DXVECTOR3 * directionToLight)
{
	int texWidth = _numCellsPerRow;
	int texHeight = _numCellsPerCol;

	if (FAILED(D3DXCreateTexture(
		_device,
		texWidth, texHeight,
		0, 0,
		D3DFMT_X8R8G8B8,
		D3DPOOL_MANAGED, &_tex
	)))
	{
		ERR_MSG(0, "CreateTexture- FAILED", 0, 0);
		return false;
	}
	
	D3DSURFACE_DESC textureDesc;
	_tex->GetLevelDesc(0, &textureDesc);

	if (textureDesc.Format != D3DFMT_X8R8G8B8)
	{
		return false;
	}

	D3DLOCKED_RECT lockedRect;
	_tex->LockRect(0, &lockedRect, 0, 0);

	DWORD* imageData = (DWORD*)lockedRect.pBits;

	for (int i = 0; i < texHeight; ++i)
	{
		for (int j = 0; j < texWidth; ++j)
		{
			D3DXCOLOR c;

			float height = (float)getHeightmapEntry(i, j) / _heightScale;

			/*고도에 따른 색 지정*/
			
			imageData[i*lockedRect.Pitch / 4 + j] = (D3DCOLOR)c;
		}
	}

	_tex->UnlockRect(0);

	if (!lightTerrain(directionToLight))
	{
		ERR_MSG(0, "lightTerrain() - FAILED", 0, 0);
		return false;
	}

	if (FAILED(D3DXFilterTexture(
		_tex,
		0, 0,
		D3DX_DEFAULT
	)))
	{
		ERR_MSG(0, "D3DXFilterTexture() - FAILED", 0, 0);
		return false;
	}

	return true;
}

bool CTerrain::draw(D3DXMATRIX * world, bool drawTris)
{
	
	return false;
}

bool CTerrain::readRawFile(std::string fileName)
{
	std::vector<BYTE> in(_numVertices);
	std::ifstream inFile(fileName.c_str(), std::ios_base::binary);
	
	if (!inFile)
		return false;

	inFile.read((char*)&in[0], in.size());

	inFile.close();

	_heightmap.resize(_numVertices);

	for (int i = 0; i < in.size(); ++i)
		_heightmap[i] = in[i];

	return true;
	
}

bool CTerrain::computeVertices()
{
	if (FAILED(_device->CreateVertexBuffer(
		_numVertices * sizeof(TerrainVertex),
		D3DUSAGE_WRITEONLY,
		TerrainVertex::FVF,
		D3DPOOL_MANAGED,
		&_vb,
		0
	)))
	{
		ERR_MSG(0, "CreateVertexBuffer - FAILED", 0, 0);
		return false;
	}

	int startX = -_width / 2;
	int startZ = _depth / 2;

	int endX = _width / 2;
	int endZ = -_depth / 2;

	float uCoordIncrementSize = 1.0f / (float)_numCellsPerRow;
	float vCoordIncrementSize = 1.0f / (float)_numCellsPerCol;

	TerrainVertex* v = 0;
	_vb->Lock(0, 0, (void**)&v, 0);

	int i = 0;
	for (int z = startZ; z >= endZ; z -= _cellSpacing)
	{
		int j = 0;
		for (int x = startX; x <= endX; x += _cellSpacing)
		{
			int index = i*_numVertsPerRow + j;

			v[index] = TerrainVertex(
				(float)x,
				(float)_heightmap[index],
				(float)z,
				(float)j * uCoordIncrementSize,
				(float)i * vCoordIncrementSize);
			++j;
		}
		++i;
	}

	_vb->Unlock();
	return true;
}

bool CTerrain::computeIndices()
{
	if (FAILED(_device->CreateIndexBuffer(
		_numTriangles * 3 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&_ib,
		0
	)))
	{
		ERR_MSG(0, "CreateIndexBuffer - FAILED", 0, 0);
		return false;
	}
		
	int baseIndex = 0;

	WORD* indices = 0;
	_ib->Lock(0, 0, (void**)&indices, 0);

	for (int i = 0; i < _numCellsPerCol; ++i)
	{
		for (int j = 0; j < _numCellsPerRow; ++j)
		{
			indices[baseIndex] = i*_numVertsPerRow + j;
			indices[baseIndex + 1] = i * _numVertsPerRow + j + 1;
			indices[baseIndex + 2] = (i + 1) * _numVertsPerRow + j;

			indices[baseIndex + 3] = (i + 1) * _numVertsPerRow + j;
			indices[baseIndex + 4] = i * _numVertsPerRow + j + 1;
			indices[baseIndex + 5] = (i + 1) * _numVertsPerRow + j +1;

			baseIndex += 6;
		}
	}
	_ib->Unlock();

	return true;
}

bool CTerrain::lightTerrain(D3DXVECTOR3 * directionalToLight)
{
	D3DSURFACE_DESC textureDesc;
	_tex -> GetLevelDesc(0, &textureDesc);

	if(textureDesc.Format != D3DFMT_X8R8G8B8)
		return false;
	
	D3DLOCKED_RECT lockedRect;
	_tex->LockRect(0, &lockedRect, 0, 0);

	DWORD* imageData = (DWORD*)lockedRect.pBits;
	for (int i = 0; i < textureDesc.Width; ++i)
	{
		for (int j = 0; j < lockedRect.Pitch / 4; ++j)
		{
			int index = i*lockedRect.Pitch / 4 + j;
		
			D3DXCOLOR c(imageData[index]);

			c *= computeShade(i, j, directionalToLight);

			imageData[index] = (D3DCOLOR)c;
		}
	}

	_tex->UnlockRect(0);

	return true;

}

float CTerrain::computeShade(int cellRow, int cellCol, D3DXVECTOR3 * directionToLight)
{
	float heightA = getHeightmapEntry(cellRow, cellCol);
	float heightB = getHeightmapEntry(cellRow, cellCol+1);
	float heightC = getHeightmapEntry(cellRow+1, cellCol);

	D3DXVECTOR3 u(_cellSpacing, heightB - heightA, 0.0f);
	D3DXVECTOR3 v(0.0f, heightC - heightA, _cellSpacing);

	D3DXVECTOR3 n;
	D3DXVec3Cross(&n, &u, &v);
	D3DXVec3Normalize(&n, &n);

	float cosine = D3DXVec3Dot(&n, directionToLight);

	if (cosine < 0.0f)
		cosine = 0.0f;

	return cosine;


	return 0.0f;
}

void CTerrain::Release()
{
	_vb->Release();
	_ib->Release();
	_tex->Release();
}
