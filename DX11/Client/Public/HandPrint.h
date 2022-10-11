#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Cube;
class CCollider;
END

BEGIN(Client)

class CHandPrint final : public CGameObject
{
private:
	CHandPrint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHandPrint(const CHandPrint& rhs);
	virtual ~CHandPrint() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual HRESULT SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix);

public:
	void Set_Position(CTransform* _pTransform, _vector vPlayerPos);


private:
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;		
	CVIBuffer_Cube*			m_pVIBufferCom = nullptr;
	CCollider* m_pOBBCom = nullptr;

	CTransform*				m_pObjectTransform = nullptr;

private:
	_float4x4		m_ProjMatrix;
	_float4x4		m_ViewMatrix;

	_float4			m_vPosOffSet = _float4(0.f, 0.f, 0.f, 1.f);

private:
	HRESULT SetUp_Components();


public:
	static CHandPrint* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END