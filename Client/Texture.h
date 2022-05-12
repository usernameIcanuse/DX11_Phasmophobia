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
	virtual HRESULT		InsertTexture(const TCHAR* pFilePath,		// �̹��� ���
									const TCHAR* pStateKey = L"",	// ��Ƽ �ؽ�ó�� ��� ���Ǵ� ���� Ű ��
									const int& iCount = 0)PURE;			// ��Ƽ �ؽ�ó�� ��� ���Ǵ� �̹��� ���
	
	virtual void		Release(void)PURE;
};

