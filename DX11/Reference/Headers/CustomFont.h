#pragma once

#include "Base.h"

BEGIN(Engine)

class CCustomFont final : public CBase
{
private:
	CCustomFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCustomFont() = default;

public:
	HRESULT Initialize(const _tchar* pFontFilePath);
	HRESULT Render(const _tchar* pString, const _float2& vPosition, _fvector vColor);
	HRESULT Render(const _tchar* pString, _fvector vPosition, _fvector vColor, float rotation, _fvector vOrigin, _vector vScale, SpriteEffects effects = SpriteEffects_None, float layerDepth = 0);
private:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pContext = nullptr;

private:
	SpriteBatch*				m_pBatch = nullptr;
	SpriteFont*					m_pFont = nullptr;

public:
	static CCustomFont* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontFilePath);
	virtual void Free() override;
};

END