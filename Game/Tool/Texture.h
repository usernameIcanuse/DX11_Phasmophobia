#pragma once

#include "Include.h"

class CTexture
{
public:
	CTexture();
	virtual ~CTexture();

public:
	virtual const TEXINFO*		Get_Texture(const TCHAR* pStateKey = L"",
											const int& iCount = 0)PURE;
public:	
	virtual HRESULT		InsertTexture(const TCHAR* pFilePath,		// 이미지 경로
									const TCHAR* pStateKey = L"",	// 멀티 텍스처일 경우 사용되는 상태 키 값
									const int& iCount = 0)PURE;			// 멀티 텍스처일 경우 사용되는 이미지 재수
	
	virtual void		Release(void)PURE;
};

