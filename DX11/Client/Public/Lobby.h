#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END

BEGIN(Client)

class CLobby final : public CGameObject
{
protected:
	CLobby(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLobby(const CLobby& rhs);
	virtual ~CLobby() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual HRESULT SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix);


protected:
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CModel* m_pModelCom = nullptr;



protected:
	virtual	HRESULT	Setup_Component();

public:
	virtual void On_Collision_Enter(CCollider* pCollider);
	virtual void On_Collision_Stay(CCollider* pCollider);
	virtual void On_Collision_Exit(CCollider* pCollider);

public:
	static CLobby* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END