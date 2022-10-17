#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CFirstLoading final : public CGameObject
{
private:
	CFirstLoading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFirstLoading(const CFirstLoading& rhs);
	virtual ~CFirstLoading() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;		
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;

private:
	_float			m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4		m_ProjMatrix;

	_float			m_fTime = 0.f;
	_float			m_fFade = 0.f;
	_int			m_iTextureIndex = 0;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

public:
	static CFirstLoading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END