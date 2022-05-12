#pragma once
#include "Texture.h"
class CMultiTexture :
	public CTexture
{
public:
	CMultiTexture();
	virtual ~CMultiTexture();

public:
	virtual const TEXINFO* Get_Texture(const TCHAR* pStateKey = L"", const int& iCount = 0) override;
public:
	virtual HRESULT InsertTexture(const TCHAR* pFilePath, const TCHAR* pStateKey = L"", const int& iCount = 0) override;
	virtual void Release(void) override;

private:
	map<wstring, vector<TEXINFO*>>		m_mapMultiTex;

};

