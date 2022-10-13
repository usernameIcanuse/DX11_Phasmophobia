#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
END

BEGIN(Client)

class CComputer final : public CGameObject
{
private:
	CComputer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CComputer(const CComputer& rhs);
	virtual ~CComputer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual HRESULT SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix);

private:
	CShader* m_pShaderCom = nullptr;
	CModel* m_pModelCom = nullptr;
	CCollider* m_pOBBCom = nullptr;

private:
	virtual	HRESULT	Setup_Component();


public:
	static CComputer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END