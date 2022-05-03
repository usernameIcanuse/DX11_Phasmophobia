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
								const TCHAR* pObjKey,// �̹��� ���
								const TCHAR* pStateKey = L"",	// ��Ƽ �ؽ�ó�� ��� ���Ǵ� ���� Ű ��
								const int& iCount = 0);			// ��Ƽ �ؽ�ó�� ��� ���Ǵ� �̹��� ���

	 void		Release(void);

private:
	map<wstring, CTexture*>			m_mapTex;
};

