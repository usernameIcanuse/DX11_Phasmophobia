#pragma once

#include "SingleTexture.h"
#include "MultiTexture.h"

class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)

public:
	CTextureMgr();
	~CTextureMgr();

public:
	const TEXINFO*		Get_Texture(const TCHAR* pObjKey,
									const TCHAR* pStateKey = L"",
									const int& iCount = 0);
public:
	HRESULT		InsertTexture(const TCHAR* pFilePath,	
								TEXTYPE	eType,
								const TCHAR* pObjKey,// 이미지 경로
								const TCHAR* pStateKey = L"",	// 멀티 텍스처일 경우 사용되는 상태 키 값
								const int& iCount = 0);			// 멀티 텍스처일 경우 사용되는 이미지 재수

	 void		Release(void);

private:
	map<wstring, CTexture*>			m_mapTex;
};

