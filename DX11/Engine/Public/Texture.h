#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
private:
	CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;

public:
	HRESULT Set_ShaderResourceView(class CShader* pShader, const char* pConstantName, _uint iIndex = 0);

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pTextureFilePath, _uint iNumTextures) ;
	virtual HRESULT Initialize(void* pArg) override;

private:
	vector<ID3D11ShaderResourceView*>			m_SRVs;
	typedef vector<ID3D11ShaderResourceView*>	SRVS;

public:
	static CTexture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTextureFilePath, _uint iNumTextures = 1);
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END