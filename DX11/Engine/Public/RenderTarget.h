#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CRenderTarget final : public CBase
{
public:
	typedef struct tagRenderTargetDesc
	{
		_uint iWidth;
		_uint iHeight;
		DXGI_FORMAT eFormat;
		_float4 vClearColor;
	}RENDERTARGETDESC;

private:
	CRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderTarget() = default;

public:
	void Get_Desc(D3D11_TEXTURE2D_DESC*	TextureDesc)
	{
		m_pTexture->GetDesc(TextureDesc);
	}

	ID3D11ShaderResourceView* Get_SRV() const {
		return m_pSRV;
	}

	ID3D11RenderTargetView* Get_RTV() const {
		return m_pRTV;
	}

public:
	/* 1. ID3D11Texture2D 라는 객체를 선 생성. */
	/* 2. ID3D11RenderTargetView를 생성. */
	/* 3. ID3D11ShaderResourceView를 생성. */
	HRESULT Initialize(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor);
	HRESULT Clear();

#ifdef _DEBUG
	HRESULT Ready_Debug(_float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif // _DEBUG

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;

	_float4					m_vClearColor;

#ifdef _DEBUG
private:
	_float4x4				m_WorldMatrix;
	
#endif // _DEBUG

private:
	ID3D11Texture2D*					m_pTexture = nullptr;
	/* 렌더타겟으로 바인딩 할 수 있는 객체 .*/
	ID3D11RenderTargetView*				m_pRTV = nullptr;

	/* 셰이더 전역변수로 바인딩 될 수 있는 객체. */
	ID3D11ShaderResourceView*			m_pSRV = nullptr;

public:
	static CRenderTarget* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor);	
	virtual void Free() override;
};

END