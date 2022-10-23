#pragma once

#include "Client_Defines.h"
#include "UIBackground.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CIcon;


class CLobby_Main final : public CUIBackground
{
private:
	CLobby_Main(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLobby_Main(const CLobby_Main& rhs);
	virtual ~CLobby_Main() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void Set_Enable(_bool _bEnable);
	virtual HRESULT SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix);


private:
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;		
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;

private:
	HRESULT SetUp_Components();

	HRESULT SetUp_Icon();

public:
	static CLobby_Main* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END