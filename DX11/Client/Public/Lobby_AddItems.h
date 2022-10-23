#pragma once

#include "Client_Defines.h"
#include "Engine_Defines.h"
#include "UIBackground.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CIcon;


class CLobby_AddItems final : public CUIBackground
{
private:
	CLobby_AddItems(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLobby_AddItems(const CLobby_AddItems& rhs);
	virtual ~CLobby_AddItems() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void Set_Enable(_bool _bEnable);


private:
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;		
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;


private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();
	HRESULT SetUp_Icon();

public:
	static CLobby_AddItems* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END