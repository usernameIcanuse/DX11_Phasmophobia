#pragma once

#include "Include.h"

class CToolView;
class CMyTerrain
{
public:
	CMyTerrain();
	~CMyTerrain();

public:
	void		Tile_Change(const D3DXVECTOR3& vPos, const BYTE& byDrawID);
	int			GetTileIndex(const D3DXVECTOR3& vPos);
	bool		PickingDot(const D3DXVECTOR3& vPos, const int& iIndex);
	void		SetMainView(CToolView* pView) { m_pMainView = pView; }
	const vector<TILE*> Get_TileVector() const { return m_vecTile; }
	void		Relese_Terrain() {
		for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
		m_vecTile.clear();
	}

	void	Set_Vector(vector<TILE*> vecTile) { m_vecTile = vecTile; }

public:
	void		Initialize(void);
	void		Update(void);
	void		Render(void);
	void		Mini_Render(void);
	void		Release(void);

private:
	void		Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY);

private:
	vector<TILE*>		m_vecTile;
	CToolView*			m_pMainView = nullptr;
};

