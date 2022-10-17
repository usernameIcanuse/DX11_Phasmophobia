#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CCollider;
class CModel;
#ifdef _DEBUG
class CRenderer;
#endif
END

BEGIN(Client)

class CTruck final : public CGameObject
{
private:
	CTruck(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTruck(const CTruck& rhs);
	virtual ~CTruck() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual HRESULT SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix);

private:
	CShader* m_pShaderCom = nullptr;
	CCollider* m_pOBBCom = nullptr;
	CModel* m_pModelCom = nullptr;

#ifdef _DEBUG
	CRenderer* m_pRendererCom = nullptr;
#endif


private:
	virtual	HRESULT	Setup_Component();


public:
	static CTruck* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END