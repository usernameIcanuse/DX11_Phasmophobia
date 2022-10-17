#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CVIBuffer_Rect;
class CTexture;
class CRenderer;
END

BEGIN(Client)

class CScreen final : public CGameObject
{
private:
	CScreen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CScreen(const CScreen& rhs);
	virtual ~CScreen() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual HRESULT SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix);

private:
	CShader* m_pShaderCom = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;
	CTexture* m_pTextureCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;

	class CComputer* m_pComputer = nullptr;

private:
	virtual	HRESULT	Setup_Component();


public:
	static CScreen* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END