#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CCollider;
class CRenderer;
class CModel;

END

BEGIN(Client)

class CHouse : public CGameObject
{
protected:
	CHouse(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHouse(const CHouse& rhs);
	virtual ~CHouse() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual HRESULT SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix);

public:
	HRESULT	SetUp_ModelCom(const _tchar* pPrototypeTag, _uint iLevelIndex = LEVEL_GAMEPLAY );

protected:
	CShader* m_pShaderCom = nullptr;
#ifdef _DEBUG
	CRenderer* m_pRendererCom = nullptr;
#endif
	CModel* m_pModelCom = nullptr;
	CCollider* m_pOBBCom = nullptr;
	CTexture* m_pBlackTex = nullptr;

protected:
	virtual	HRESULT	Setup_Component();

public:
	virtual void On_Collision_Enter(CCollider* pCollider);
	virtual void On_Collision_Stay(CCollider* pCollider);
	virtual void On_Collision_Exit(CCollider* pCollider);

public:
	static CHouse* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END