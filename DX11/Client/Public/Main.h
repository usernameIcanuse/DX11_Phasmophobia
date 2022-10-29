#pragma once

#include "Client_Defines.h"
#include "UIBackground.h"
BEGIN(Engine)
class CShader;
class CRenderer;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUIIcon;


class CMain final : public CUIBackground
{
private:
	CMain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMain(const CMain& rhs);
	virtual ~CMain() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void Set_Enable(_bool _bEnable);

public:
	_uint	Selected_Menu()
	{
		return m_iSelectedMenu;
	}
	
	void		Icon_Lock(_bool _bLock);
	HRESULT Setup_Icon();

private:
	CShader* m_pShaderCom = nullptr;
	CTexture* m_pTextureCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;

private:
	HRESULT Setup_Component();

	virtual HRESULT SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix);


public:
	static CMain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END