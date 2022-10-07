#include "..\Public\CustomFont.h"

CCustomFont::CCustomFont(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)	
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CCustomFont::Initialize(const _tchar * pFontFilePath)
{
	m_pBatch = new SpriteBatch(m_pContext);
	m_pFont = new SpriteFont(m_pDevice, pFontFilePath);

	return S_OK;
}

HRESULT CCustomFont::Render(const _tchar* pString, const _float2& vPosition, _fvector vColor)
{
	if (nullptr == m_pBatch ||
		nullptr == m_pFont)
		return E_FAIL;

	m_pBatch->Begin();
	// void XM_CALLCONV DrawString(_In_ SpriteBatch* spriteBatch, _In_z_ wchar_t const* text, FXMVECTOR position, FXMVECTOR color, float rotation, FXMVECTOR origin, GXMVECTOR scale, SpriteEffects effects = SpriteEffects_None, float layerDepth = 0) const;
	//m_pFont->DrawString(m_pBatch, pString, vecPos, vColor, 0.f, XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 2.f, 1.f, 0.f));
	
	m_pFont->DrawString(m_pBatch, pString, vPosition, vColor);

	m_pBatch->End();

	return S_OK;
}

HRESULT CCustomFont::Render(const _tchar* pString, _fvector vPosition, _fvector vColor, float rotation, _fvector vOrigin, _vector vScale, SpriteEffects effects, float layerDepth)
{

	if (nullptr == m_pBatch ||
		nullptr == m_pFont)
		return E_FAIL;

	m_pBatch->Begin();
	//vOrigin : 폰트의 위치 오프셋
	m_pFont->DrawString(m_pBatch, pString, vPosition, vColor, rotation, vOrigin, vScale, effects, layerDepth);

	m_pBatch->End();

	return S_OK;
}

CCustomFont * CCustomFont::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pFontFilePath)
{
	CCustomFont*		pInstance = new CCustomFont(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pFontFilePath)))
	{
		MSG_BOX("Failed to Created : CCustomFont");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCustomFont::Free()
{
	Safe_Delete(m_pBatch);
	Safe_Delete(m_pFont);

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
