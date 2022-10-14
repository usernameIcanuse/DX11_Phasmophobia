#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CCollider;
class CModel;

END

BEGIN(Client)

class CTruck_Inside final : public CGameObject
{
private:
	CTruck_Inside(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTruck_Inside(const CTruck_Inside& rhs);
	virtual ~CTruck_Inside() = default;

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


private:
	virtual	HRESULT	Setup_Component();


public:
	static CTruck_Inside* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END