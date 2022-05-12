#include "stdafx.h"
#include "Obj.h"
#include "TextureMgr.h"
#include "ToolView.h"
#include "Device.h"
#include "MainFrm.h"
#include "MyForm.h"
#include "Tab3.h"

CObj::CObj()
{
	
}


CObj::~CObj()
{
	Release();
}

void CObj::Object_Pick(const D3DXVECTOR3 & vPos, const BYTE & byDrawID)
{
	int		iIndex = GetObjectPos(vPos);

	if (-1 == iIndex)
		return;

	OBJECT*		pTemp = new OBJECT;

	pTemp->vPos.x = m_vecPos[iIndex]->x;
	pTemp->vPos.y = m_vecPos[iIndex]->y;
	pTemp->vPos.z = 0.f;

	pTemp->fRadian = 0.f;

	pTemp->vSize = { 1.f,1.f,1.f };

	pTemp->byDrawID = byDrawID;
	pTemp->byOption = 1;
	
	m_vecObj.push_back(pTemp);
}

int CObj::GetObjectPos(const D3DXVECTOR3 & vPos)
{
	for (size_t iIndex = 0; iIndex < m_vecPos.size(); ++iIndex)
	{
		if (PickingDot(vPos, iIndex))
		{
			return iIndex;
		}
	}

	return -1;
			// 피킹이 제대로 수행되지 않았을 경우 -1을 반환
}

bool CObj::PickingDot(const D3DXVECTOR3 & vPos, const int & iIndex)
{
	// 12시, 3시, 6시, 9시 순서
	D3DXVECTOR3	vPoint[4] = {
		{ m_vecPos[iIndex]->x, m_vecPos[iIndex]->y + (TILECY / 2.f), 0.f },
		{ m_vecPos[iIndex]->x + (TILECX / 2.f), m_vecPos[iIndex]->y, 0.f },
		{ m_vecPos[iIndex]->x, m_vecPos[iIndex]->y - (TILECY / 2.f), 0.f },
		{ m_vecPos[iIndex]->x - (TILECX / 2.f), m_vecPos[iIndex]->y, 0.f }
	};

	// 시계 방향으로 향하는 네 방향벡터를 구하자

	D3DXVECTOR3	vDir[4] = {

		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};

	// 각 법선 벡터를 구하자
	// (x, y)에 대한 법선벡터는 (-y, x)
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
		// 예각인 경우 양수, 예각인 경우 사각형 바깥쪽을 피킹한 상황
		if (0.f < D3DXVec3Dot(&vNormal[i], &vMouseDir[i]))
			return false;
	}

	return true;
}

OBJECT* CObj::PickObject(const D3DXVECTOR3& vMouse,int&	 Index)
{
	int		iIndex = GetObjectPick(vMouse);

	if (-1 == iIndex)
		return nullptr;

	Index = iIndex;

	return m_vecObj[iIndex];
}

int CObj::GetObjectPick(const D3DXVECTOR3& vMouse)
{
	for (size_t iIndex = 0; iIndex < m_vecObj.size(); ++iIndex)
	{
		if (PickingObjDot(vMouse, iIndex))
		{
			return iIndex;
		}
	}

	return -1;
}

bool CObj::PickingObjDot(const D3DXVECTOR3 & vPos, const int & iIndex)
{

	D3DXVECTOR3	vPoint[4] = {
		{ m_vecObj[iIndex]->vPos.x, m_vecObj[iIndex]->vPos.y + (TILECY / 2.f), 0.f },
		{ m_vecObj[iIndex]->vPos.x + (TILECX / 2.f), m_vecObj[iIndex]->vPos.y, 0.f },
		{ m_vecObj[iIndex]->vPos.x, m_vecObj[iIndex]->vPos.y - (TILECY / 2.f), 0.f },
		{ m_vecObj[iIndex]->vPos.x - (TILECX / 2.f), m_vecObj[iIndex]->vPos.y, 0.f }
	};

	// 시계 방향으로 향하는 네 방향벡터를 구하자

	D3DXVECTOR3	vDir[4] = {

		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};

	// 각 법선 벡터를 구하자
	// (x, y)에 대한 법선벡터는 (-y, x)
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
		// 예각인 경우 양수, 예각인 경우 사각형 바깥쪽을 피킹한 상황
		if (0.f < D3DXVec3Dot(&vNormal[i], &vMouseDir[i]))
			return false;
	}

	return true;
}

void CObj::Delete_Object(int iIndex)
{
	auto& iter = m_vecObj.begin() + iIndex;
	Safe_Delete(*iter);
	iter = m_vecObj.erase(iter);
}

void CObj::Initialize(void)
{
	if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Stage/Object/Object%d.png", TEX_MULTI, L"Object", L"Object", 8)))
	{
		AfxMessageBox(L"Object Image Insert Failed");
		return;
	}

	m_vecPos.reserve(TILEX * TILEY);



	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			float fX = (TILECX * j) + ((i % 2) * (TILECX / 2.f));
			float fY = (TILECY / 2.f) * i;

			D3DXVECTOR3*		pObj = new D3DXVECTOR3( fX , fY , 0.f );

			m_vecPos.push_back(pObj);

		}
	}

}

void CObj::Update(void)
{

	CMainFrame*		pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	if (!pMain)
		return;
	CMiniView*		pMiniView = dynamic_cast<CMiniView*>(pMain->m_SecondSplitter.GetPane(0, 0));
	//1,0 창의 정보가져옴 
	CMyForm*	pForm = dynamic_cast<CMyForm*>(pMain->m_SecondSplitter.GetPane(1, 0));
	int iObjSize = pForm->pDlg3->m_iObjSize;

	Set_Size(&matSize, iObjSize);
}

void CObj::Render(void)
{
	D3DXMATRIX		matWorld, matScale, matTrans, matRotZ;

	TCHAR		szBuf[MIN_STR] = L"";
	int			iIndex = 0;


	for (auto iter : m_vecObj)
	{
		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixScaling(&matScale, iter->vSize.x, iter->vSize.y, 1.f);
		D3DXMatrixTranslation(&matTrans, iter->vPos.x - m_pMainView->GetScrollPos(0),  // x는 0
			iter->vPos.y - m_pMainView->GetScrollPos(1),  // y는 1
			iter->vPos.z);
		D3DXMatrixRotationZ(&matRotZ, iter->fRadian);

		matWorld = matScale *matRotZ*matTrans;
		
		swprintf_s(szBuf, L"%d", iIndex);

		RECT	rc{};

		// GetClientRect : 현재 클라이언트의 렉트를 얻어옴
		// ::함수 : 전역 함수 호출 구문, 일반적인 클래스 멤버함수와 구분하기 위해 표기하기도 한다.
		::GetClientRect(m_pMainView->m_hWnd, &rc);

		float fCX = WINCX / float(rc.right - rc.left);
		float fCY = WINCY / float(rc.bottom - rc.top);

		Set_Ratio(&matWorld, fCX, fCY);
		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		const TEXINFO*		pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Object", L"Object", iter->byDrawID);

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

void CObj::Mini_Render(void)
{
	D3DXMATRIX		matWorld, matScale, matTrans, matRotZ;

	for (auto iter : m_vecObj)
	{
		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixScaling(&matScale, iter->vSize.x, iter->vSize.y, 1.f);
		D3DXMatrixTranslation(&matTrans, iter->vPos.x,
			iter->vPos.y,
			iter->vPos.z);

		D3DXMatrixRotationZ(&matRotZ, iter->fRadian);

		matWorld = matScale *matRotZ*matTrans;

		Set_Ratio(&matWorld, 0.3f, 0.3f);
		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		const TEXINFO*		pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Object", L"Object", iter->byDrawID);

		float		fX = pTexInfo->tImgInfo.Width / 2.f;
		float		fY = pTexInfo->tImgInfo.Height / 2.f;

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,
			&D3DXVECTOR3(fX, fY, 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CObj::Release(void)
{
	for_each(m_vecObj.begin(), m_vecObj.end(), CDeleteObj());
	m_vecObj.clear();

	for_each(m_vecPos.begin(), m_vecPos.end(), CDeleteObj());
	m_vecPos.clear();

	vector<OBJECT*>().swap(m_vecObj);
	vector<D3DXVECTOR3*>().swap(m_vecPos);
}

void CObj::Set_Ratio(D3DXMATRIX * pOut, float fRatioX, float fRatioY)
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

void CObj::Set_Size(D3DXMATRIX * pOut, int _iSize)
{
	pOut->_11 *= _iSize*0.1f;
	pOut->_21 *= _iSize*0.1f;
	pOut->_31 *= _iSize*0.1f;
	pOut->_41 *= _iSize*0.1f;
					  
	pOut->_12 *= _iSize*0.1f;
	pOut->_22 *= _iSize*0.1f;
	pOut->_32 *= _iSize*0.1f;
	pOut->_42 *= _iSize*0.1f;
					   
}
