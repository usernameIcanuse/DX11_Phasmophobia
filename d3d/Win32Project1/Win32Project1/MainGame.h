#pragma once

#include "Define.h"
class CDevice;
class CMainGame
{
public:
	HRESULT			Initialize();
	void Update();
	void Late_Update();
	void Render();
	void Release();

public:
	CMainGame();
	~CMainGame();

private:
	CDevice*		m_pGraphicDev = nullptr;

	void		ComputeNormal(D3DXVECTOR3* p0, D3DXVECTOR3* p1, D3DXVECTOR3* p2, D3DXVECTOR3* out);

	DWORD  FrameCnt;
	float TimeElapsed;
	float FPS;

};

struct BoundingSphere
{
	BoundingSphere()
	{
		_fRadius = 0.0f;
	}
	
	D3DXVECTOR3 _vCenter;
	float	_fRadius;

};

struct BoundingBox
{
	BoundingBox() 
	{
		_min.x = INFINITY;
		_min.y = INFINITY;
		_min.z = INFINITY;
		_max.x = INFINITY;
		_max.y = INFINITY;
		_max.z = INFINITY;
	}

	BOOL	isPointInside(D3DXVECTOR3& p)
	{
		return (p.x >= _min.x && p.y >= _min.y && p.z >= _min.z&&
			p.x <= _max.x && p.y <= _max.y && p.z <= _max.z);
	}
	D3DXVECTOR3	_min;
	D3DXVECTOR3 _max;

};

