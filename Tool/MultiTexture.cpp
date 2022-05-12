#include "stdafx.h"
#include "MultiTexture.h"
#include "Device.h"

CMultiTexture::CMultiTexture()
{
}

CMultiTexture::~CMultiTexture()
{
	Release();
}

const TEXINFO* CMultiTexture::Get_Texture(const TCHAR* pStateKey /*= L""*/, const int& iCount /*= 0*/)
{
	auto	iter = find_if(m_mapMultiTex.begin(), m_mapMultiTex.end(), [&](auto& Pair) 
	{
		if (pStateKey == Pair.first)
			return true;

		return false;
	});
	
	if (iter == m_mapMultiTex.end())
		return nullptr;

	return iter->second[iCount];
}

HRESULT CMultiTexture::InsertTexture(const TCHAR* pFilePath, 
									const TCHAR* pStateKey /*= L""*/, 
									const int& iCount /*= 0*/)
{
	// 520바이트, 경로의 길이는 절대 520바이트를 넘어선 안 된다.
	TCHAR		szFullPath[MAX_PATH] = L"";

	for (int i = 0; i < iCount; ++i)
	{
		swprintf_s(szFullPath, pFilePath, i);

		TEXINFO* pTexInfo = new TEXINFO;
		ZeroMemory(pTexInfo, sizeof(TEXINFO));

		if (FAILED(D3DXGetImageInfoFromFile(szFullPath, &(pTexInfo->tImgInfo))))
		{
			AfxMessageBox(pFilePath);
			Safe_Delete(pTexInfo);
			return E_FAIL;
		}

		// D3DXCreateTextureFromFileEx :  이미지 파일에 기반하여 텍스처 com객체 만들어주는 함수
		if (FAILED(D3DXCreateTextureFromFileEx(CDevice::Get_Instance()->Get_Device(),	// 디바이스 컴객체
			szFullPath,		// 파일이 경로
			pTexInfo->tImgInfo.Width,		// 파일 가로사이즈
			pTexInfo->tImgInfo.Height,	// 파일 세로사이즈
			pTexInfo->tImgInfo.MipLevels,	// 파일 밉 레벨
			0,								// 텍스처 생성 방식, 0인 경우 일반적인 텍스처(D3DUSAGE_RENDERTARGET : 화면 출력용 텍스처)
			pTexInfo->tImgInfo.Format,	// 텍스처 픽셀 포맷
			D3DPOOL_MANAGED,				// DX자원들을 어떤 메모리에 보관할 것인가
			D3DX_DEFAULT,					// 이미지 필터링 방식, 확대 축소 시 어떻게 채울 것인가	
			D3DX_DEFAULT,					// Mipfilter, 밉맵을 이용한 확대 축소 시 어떻게 채울 것인가
			0,								// colorkey : 제거할 색상, 0을 넣어주면 컬러키 무효화
			nullptr,						// 이미지 정보를 기록할 구조체 주소값	, D3DXGetImageInfoFromFile를 통해서 채워줘서 nullptr
			nullptr,						// 팔레트 이미지 정보, 8비트 시절의 argb를 표현하기 위한 구조체
			&(pTexInfo->pTexture))))
		{
			AfxMessageBox(L"Multi Texture Load Failed");
			Safe_Delete(pTexInfo);
			return E_FAIL;
		}

		m_mapMultiTex[pStateKey].push_back(pTexInfo);
	}

	return S_OK;
}

void CMultiTexture::Release(void)
{
	for_each(m_mapMultiTex.begin(), m_mapMultiTex.end(), [](auto& Pair)
	{
		for_each(Pair.second.begin(), Pair.second.end(), Safe_Delete<TEXINFO*>);
		Pair.second.clear();
	});
	m_mapMultiTex.clear();
}
