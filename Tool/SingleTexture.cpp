#include "stdafx.h"
#include "SingleTexture.h"
#include "Device.h"


CSingleTexture::CSingleTexture()
{
}


CSingleTexture::~CSingleTexture()
{
	Release();
}

HRESULT CSingleTexture::InsertTexture(const TCHAR* pFilePath, const TCHAR* pStateKey /*= L""*/, const int& iCount /*= 0*/)
{
	m_pTexInfo = new TEXINFO;
	ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

	// D3DXGetImageInfoFromFile : 지정한 이미지 파일에 관한 정보를 얻어와 구조체에 저장하는 함수
	if (FAILED(D3DXGetImageInfoFromFile(pFilePath, &(m_pTexInfo->tImgInfo))))
	{
		AfxMessageBox(pFilePath);
		Safe_Delete(m_pTexInfo);
		return E_FAIL;
	}
	
	// D3DXCreateTextureFromFileEx :  이미지 파일에 기반하여 텍스처 com객체 만들어주는 함수
	if (FAILED(D3DXCreateTextureFromFileEx(CDevice::Get_Instance()->Get_Device(),	// 디바이스 컴객체
		pFilePath,		// 파일이 경로
		m_pTexInfo->tImgInfo.Width,		// 파일 가로사이즈
		m_pTexInfo->tImgInfo.Height,	// 파일 세로사이즈
		m_pTexInfo->tImgInfo.MipLevels,	// 파일 밉 레벨
		0,								// 텍스처 생성 방식, 0인 경우 일반적인 텍스처(D3DUSAGE_RENDERTARGET : 화면 출력용 텍스처)
		m_pTexInfo->tImgInfo.Format,	// 텍스처 픽셀 포맷
		D3DPOOL_MANAGED,				// DX자원들을 어떤 메모리에 보관할 것인가
		D3DX_DEFAULT,					// 이미지 필터링 방식, 확대 축소 시 어떻게 채울 것인가	
		D3DX_DEFAULT,					// Mipfilter, 밉맵을 이용한 확대 축소 시 어떻게 채울 것인가
		0,								// colorkey : 제거할 색상, 0을 넣어주면 컬러키 무효화
		nullptr,						// 이미지 정보를 기록할 구조체 주소값	, D3DXGetImageInfoFromFile를 통해서 채워줘서 nullptr
		nullptr,						// 팔레트 이미지 정보, 8비트 시절의 argb를 표현하기 위한 구조체
		&(m_pTexInfo->pTexture))))
	{
		AfxMessageBox(L"Single Texture Load Failed");
		Safe_Delete(m_pTexInfo);
		return E_FAIL;
	}


	/*D3DPOOL_DEFAULT = 0,	// 가장 적합한 메모리(비디오 카드 메모리), 장치가 손실되었을 경우 복원이 어려움, 속도가 가장 빠름
	D3DPOOL_MANAGED = 1,	//  DX자원을 그래픽 메모리 저장하고 동시에 RAM에 백업, 가장 안정적
	D3DPOOL_SYSTEMMEM = 2,	// DX자원을 RAM에 저장
	D3DPOOL_SCRATCH = 3,	// RAM에 저장하지만 DX 장치가 접근할 수 없음*/

	return S_OK;
}

void CSingleTexture::Release(void)
{
	Safe_Release(m_pTexInfo->pTexture);
	Safe_Delete(m_pTexInfo);
}

