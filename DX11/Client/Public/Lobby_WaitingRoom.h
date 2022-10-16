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
class CUIIcon;


class CLobby_WaitingRoom final : public CUIBackground
{
private:
	CLobby_WaitingRoom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLobby_WaitingRoom(const CLobby_WaitingRoom& rhs);
	virtual ~CLobby_WaitingRoom() = default;

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
	static CLobby_WaitingRoom* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END