#include "stdafx.h"
#include "TextureMgr.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr()
{
}


CTextureMgr::~CTextureMgr()
{
	Release();
}

const TEXINFO * CTextureMgr::Get_Texture(const TCHAR * pObjKey, const TCHAR * pStateKey, const int & iCount)
{
	auto	iter = find_if(m_mapTex.begin(), m_mapTex.end(), [&](auto& Pair)->bool
	{
		if (pObjKey == Pair.first)
			return true;

		return false;
	});

	if (iter == m_mapTex.end())
		return nullptr;

	return iter->second->Get_Texture(pStateKey, iCount);
}

HRESULT CTextureMgr::InsertTexture(const TCHAR * pFilePath, TEXTYPE eType, const TCHAR * pObjKey, const TCHAR * pStateKey, const int & iCount)
{
	//auto	iter = find_if(m_mapTex.begin(), m_mapTex.end(), CTag_Finder(pObjKey));

	auto	iter = find_if(m_mapTex.begin(), m_mapTex.end(), [&](auto& Pair)->bool 
	{
		if (pObjKey == Pair.first)
			return true;

		return false;
	});

	if (iter == m_mapTex.end())
	{
		CTexture*		pTexture = nullptr;

		switch (eType)
		{
		case TEX_SINGLE:
			pTexture = new CSingleTexture;
			break;

		case TEX_MULTI:
			pTexture = new CMultiTexture;
			break;
		}

		if (FAILED(pTexture->InsertTexture(pFilePath, pStateKey, iCount)))
		{
			AfxMessageBox(pFilePath);
			return E_FAIL;
		}

		m_mapTex.emplace(pObjKey, pTexture);
	}


	return S_OK;
}

void CTextureMgr::Release(void)
{
	//for_each(m_mapTex.begin(), m_mapTex.end(), CDeleteMap());

	for_each(m_mapTex.begin(), m_mapTex.end(), [](auto& Pair)
	{
		Safe_Delete(Pair.second);
	});

	m_mapTex.clear();
}
