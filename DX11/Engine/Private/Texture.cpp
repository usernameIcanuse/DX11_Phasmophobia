#include "..\Public\Texture.h"
#include "Shader.h"

CTexture::CTexture(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{

}

CTexture::CTexture(const CTexture & rhs)
	: CComponent(rhs)
	, m_SRVs(rhs.m_SRVs)
{
	for (auto& pSRV : m_SRVs)
		Safe_AddRef(pSRV);
}

HRESULT CTexture::Set_ShaderResourceView(CShader * pShader, const char* pConstantName, _uint iIndex)
{
	if (iIndex >= m_SRVs.size())
		return E_FAIL;

	return pShader->Set_ShaderResourceView(pConstantName, m_SRVs[iIndex]);
}

HRESULT CTexture::Initialize_Prototype(const _tchar * pTextureFilePath, _uint iNumTextures)
{
	_tchar		szTextureFilePath[MAX_PATH] = TEXT("");

	m_SRVs.reserve(iNumTextures);

	for (_uint i = 0; i < iNumTextures; ++i)
	{
		ID3D11ShaderResourceView*			pSRV = nullptr;

		wsprintf(szTextureFilePath, pTextureFilePath, i);

		_tchar			szExt[MAX_PATH] = TEXT("");

		_wsplitpath_s(szTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);

		HRESULT		hr = 0;

		if (!lstrcmp(szExt, TEXT(".dds")))		
			hr = DirectX::CreateDDSTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);
		
		else		
			hr = DirectX::CreateWICTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);

		if (FAILED(hr))
		{
			return E_FAIL;
		}
		m_SRVs.push_back(pSRV);
		
	}

	return S_OK;
}

HRESULT CTexture::Initialize_RTV(const _tchar* pTextureFilePath, _uint iNumTextures)
{
	return E_NOTIMPL;
}

HRESULT CTexture::Initialize(void * pArg)
{//m_RTV¸¦ ³Ñ°ÜÁÜ

	if (nullptr != pArg)
	{
		
	}

	return S_OK;
}


CTexture* CTexture::Create_RTV(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTextureFilePath, _uint iNumTextures)
{
	CTexture* pInstance = new CTexture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_RTV(pTextureFilePath, iNumTextures)))
	{
		MSG_BOX("Failed to Created : CTexture_RTV");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CTexture * CTexture::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pTextureFilePath, _uint iNumTextures)
{
	CTexture*		pInstance = new CTexture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pTextureFilePath, iNumTextures)))
	{
		MSG_BOX("Failed to Created : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTexture::Clone(void * pArg)
{
	CTexture*		pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for (auto& pSRV : m_SRVs)
		Safe_Release(pSRV);

	m_SRVs.clear();
}
