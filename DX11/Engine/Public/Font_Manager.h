#pragma once

#include "Base.h"

BEGIN(Engine)

class CFont_Manager final : public CBase
{
	DECLARE_SINGLETON(CFont_Manager)
private:
	CFont_Manager();
	virtual ~CFont_Manager() = default;

public:
	HRESULT Add_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontTag, const _tchar* pFontFilePath);
	HRESULT Render_Font(const _tchar* pFontTag, const _tchar* pString, const _float2& vPosition, _fvector vColor);
	HRESULT Render_Font(const _tchar* pFontTag, const _tchar* pString, _fvector vPosition, _fvector vColor, float rotation, _fvector vOrigin, _vector vScale);
private:
	map<const _tchar*, class CCustomFont*>			m_Fonts;
	typedef map<const _tchar*, class CCustomFont*>	FONTS;

private:
	class CCustomFont* Find_Font(const _tchar* pFontTag);

public:
	virtual void Free() override;
};

END