#pragma once

#include "Base.h"

BEGIN(Engine)
class CRenderTarget;
class CTexture;

class ENGINE_DLL CLight final : public CBase
{
private:
	CLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLight() = default;

public:
	LIGHTDESC* Get_LightDesc() {
		return &m_LightDesc;
	}

	void	Set_DotsTexture(CTexture* pTexture)
	{
		m_pDotsTexture = pTexture;
	}

public:
	HRESULT Initialize(const LIGHTDESC& LightDesc);
	HRESULT Render(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);

private:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pContext = nullptr;
	LIGHTDESC					m_LightDesc;

	CTexture*					m_pDotsTexture = nullptr;
	//CRenderTarget*			 m_pDepthBuffer[6] = { nullptr };

public:
	static CLight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc);
	virtual void Free() override;
};

END