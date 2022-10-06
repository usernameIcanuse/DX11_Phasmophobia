#pragma once

#include "Client_Defines.h"
#include "Item.h"

BEGIN(Engine)
class CShader;
class CCamera_Renderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CCamera_Screen final : public CItem
{
private:
	CCamera_Screen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Screen(const CCamera_Screen& rhs);
	virtual ~CCamera_Screen() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual HRESULT SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix);

public:
	virtual void Turn_Switch();

public:
	void Set_Position();

private:
	CShader*				m_pShaderCom = nullptr;
	CCamera_Renderer*		m_pCameraRenderer = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

	CTransform* m_pCameraTransform = nullptr;

private:
	_float			m_fSizeX, m_fSizeY;
	_float4x4		m_ProjMatrix;

private:
	virtual	HRESULT	Setup_Component();

public:
	static CCamera_Screen* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END