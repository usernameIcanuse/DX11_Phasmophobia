#include "stdafx.h"
#include "Terrain.h"
#include "Device.h"
#include "TextureMgr.h"


CTerrain::CTerrain()
{
}


CTerrain::~CTerrain()
{
}

HRESULT CTerrain::Initialize(void)
{
	if (FAILED(Load_Tile(L"../Data/Map.dat")))
		return E_FAIL;

	return S_OK;
}

int CTerrain::Update(void)
{
	


	return OBJ_NOEVENT;
}

void CTerrain::Late_Update(void)
{
}

void CTerrain::Render(void)
{
	D3DXMATRIX		matWorld, matScale, matTrans;

	TCHAR		szBuf[MIN_STR] = L"";
	int			iIndex = 0;

		for (auto iter : m_vecTile)
	{
		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, iter->vPos.x ,  // x는 0
			iter->vPos.y,  // y는 1
			iter->vPos.z);

		matWorld = matScale *  matTrans;

		swprintf_s(szBuf, L"%d", iIndex);

		RECT	rc{};

		// GetClientRect : 현재 클라이언트의 렉트를 얻어옴
		// ::함수 : 전역 함수 호출 구문, 일반적인 클래스 멤버함수와 구분하기 위해 표기하기도 한다.
		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		const TEXINFO*		pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", iter->byDrawID);

		float		fX = pTexInfo->tImgInfo.Width / 2.f;
		float		fY = pTexInfo->tImgInfo.Height / 2.f;

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,
			&D3DXVECTOR3(fX, fY, 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));


		CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
			szBuf,
			lstrlen(szBuf),
			nullptr,
			0,
			D3DCOLOR_ARGB(255, 0, 0, 0));

		++iIndex;
	}
}

void CTerrain::Release(void)
{
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
}

HRESULT CTerrain::Load_Tile(const TCHAR* pFilePath)
{
	HANDLE	hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD		dwByte = 0;
	TILE*		pTile = nullptr;

	while (true)
	{
		pTile = new TILE;

		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pTile);
			break;
		}

		m_vecTile.push_back(pTile);

	}

	CloseHandle(hFile);


	return S_OK;
}
