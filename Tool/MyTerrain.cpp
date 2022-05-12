#include "stdafx.h"
#include "MyTerrain.h"
#include "TextureMgr.h"
#include "Device.h"
#include "ToolView.h"

CMyTerrain::CMyTerrain()
{
}


CMyTerrain::~CMyTerrain()
{
	Release();
}

void CMyTerrain::Tile_Change(const D3DXVECTOR3 & vPos, const BYTE & byDrawID)
{
	int		iIndex = GetTileIndex(vPos);

	if (-1 == iIndex)
		return;

	m_vecTile[iIndex]->byDrawID = byDrawID;
	m_vecTile[iIndex]->byOption = 1;
}

int CMyTerrain::GetTileIndex(const D3DXVECTOR3 & vPos)
{
	for (size_t iIndex = 0; iIndex < m_vecTile.size(); ++iIndex)
	{
		if (PickingDot(vPos, iIndex))
		{
			return iIndex;
		}
	}

	return -1;		// ��ŷ�� ����� ������� �ʾ��� ��� -1�� ��ȯ
}




bool CMyTerrain::PickingDot(const D3DXVECTOR3& vPos, const int& iIndex)
{
	// 12��, 3��, 6��, 9�� ����
	D3DXVECTOR3	vPoint[4] = {
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + (TILECY / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x + (TILECX / 2.f), m_vecTile[iIndex]->vPos.y, 0.f },
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - (TILECY / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x - (TILECX / 2.f), m_vecTile[iIndex]->vPos.y, 0.f }
	};

	// �ð� �������� ���ϴ� �� ���⺤�͸� ������

	D3DXVECTOR3	vDir[4] ={

		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};

	// �� ���� ���͸� ������
	// (x, y)�� ���� �������ʹ� (-y, x)
	D3DXVECTOR3	vNormal[4] = {

		{ -vDir[0].y, vDir[0].x, 0.f },
		{ -vDir[1].y, vDir[1].x, 0.f },
		{ -vDir[2].y, vDir[2].x, 0.f },
		{ -vDir[3].y, vDir[3].x, 0.f }

	};
	
	
	D3DXVECTOR3	vMouseDir[4] = {

		vPos - vPoint[0],
		vPos - vPoint[1],
		vPos - vPoint[2],
		vPos - vPoint[3]
	};

	for (int i = 0; i < 4; ++i)
	{
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);
		D3DXVec3Normalize(&vMouseDir[i], &vMouseDir[i]);
	}

	for (int i = 0; i < 4; ++i)
	{
		// ������ ��� ���, ������ ��� �簢�� �ٱ����� ��ŷ�� ��Ȳ
		if (0.f < D3DXVec3Dot(&vNormal[i], &vMouseDir[i]))
			return false;
	}

	return true;
}

void CMyTerrain::Initialize(void)
{
	if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Stage/Terrain/Tile/Tile%d.png", TEX_MULTI, L"Terrain", L"Tile", 12)))
	{
		AfxMessageBox(L"Tile Image Insert Failed");
		return;
	}

	m_vecTile.reserve(TILEX * TILEY);

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			TILE*		pTile = new TILE;

			float fX = (TILECX * j) + ((i % 2) * (TILECX / 2.f));
			float fY = (TILECY / 2.f) * i;

			pTile->vPos = { fX , fY , 0.f };
			pTile->vSize = { (float)TILECX, (float)TILECY, 0.f };
			pTile->byOption = 0;
			pTile->byDrawID = 0;

			m_vecTile.push_back(pTile);

		}
	}



}

void CMyTerrain::Update(void)
{
}

void CMyTerrain::Render(void)
{
	D3DXMATRIX		matWorld, matScale, matTrans;

	TCHAR		szBuf[MIN_STR] = L"";
	int			iIndex = 0;


	for (auto iter : m_vecTile)
	{
		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, iter->vPos.x - m_pMainView->GetScrollPos(0),  // x�� 0
										 iter->vPos.y - m_pMainView->GetScrollPos(1),  // y�� 1
										 iter->vPos.z);

		matWorld = matScale *  matTrans;

		swprintf_s(szBuf, L"%d", iIndex);

		RECT	rc{};

		// GetClientRect : ���� Ŭ���̾�Ʈ�� ��Ʈ�� ����
		// ::�Լ� : ���� �Լ� ȣ�� ����, �Ϲ����� Ŭ���� ����Լ��� �����ϱ� ���� ǥ���ϱ⵵ �Ѵ�.
		::GetClientRect(m_pMainView->m_hWnd, &rc);
		
		float fCX = WINCX / float(rc.right - rc.left);
		float fCY = WINCY / float(rc.bottom - rc.top);
		
		Set_Ratio(&matWorld, fCX, fCY);
		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		const TEXINFO*		pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", iter->byDrawID);

		float		fX = pTexInfo->tImgInfo.Width / 2.f;
		float		fY = pTexInfo->tImgInfo.Height / 2.f;
				
		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, 
													nullptr,	
													&D3DXVECTOR3(fX, fY, 0.f),
													nullptr,	
													D3DCOLOR_ARGB(255, 255, 255, 255));	


		/*CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
			szBuf,
			lstrlen(szBuf),
			nullptr,
			0,
			D3DCOLOR_ARGB(255, 0, 0, 0));*/

		++iIndex;
	}

}

void CMyTerrain::Mini_Render(void)
{
	D3DXMATRIX		matWorld, matScale, matTrans;

	for (auto iter : m_vecTile)
	{
		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, iter->vPos.x,
										 iter->vPos.y,
										 iter->vPos.z);

		matWorld = matScale *  matTrans;

		Set_Ratio(&matWorld, 0.3f, 0.3f);
		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		const TEXINFO*		pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", iter->byDrawID);

		float		fX = pTexInfo->tImgInfo.Width / 2.f;
		float		fY = pTexInfo->tImgInfo.Height / 2.f;

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,
			&D3DXVECTOR3(fX, fY, 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CMyTerrain::Release(void)
{
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();

	vector<TILE*>().swap(m_vecTile);
	//vector <TILE*>().shrink_to_fit();
}

void CMyTerrain::Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY)
{
	pOut->_11 *= fRatioX;
	pOut->_21 *= fRatioX;
	pOut->_31 *= fRatioX;
	pOut->_41 *= fRatioX;

	pOut->_12 *= fRatioY;
	pOut->_22 *= fRatioY;
	pOut->_32 *= fRatioY;
	pOut->_42 *= fRatioY;
}
