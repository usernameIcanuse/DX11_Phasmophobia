#pragma once
#include "Obj.h"
class CTerrain :
	public CObj
{
public:
	CTerrain();
	virtual ~CTerrain();

public:
	virtual HRESULT Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

private:
	HRESULT		Load_Tile(const TCHAR* pFilePath);

private:
	vector<TILE*>		m_vecTile;

};

