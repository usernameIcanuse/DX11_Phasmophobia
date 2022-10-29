#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
#ifdef _DEBUG
class CRenderer;
#endif

END

BEGIN(Client)

class CKeyPad final : public CGameObject
{
private:
	CKeyPad(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CKeyPad(const CKeyPad& rhs);
	virtual ~CKeyPad() = default;

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
	CTexture* m_pTextureCom = nullptr;
#ifdef _DEBUG
	CRenderer* m_pRendererCom = nullptr;
#endif

	_bool	m_bClicked = false;
	_float	m_fTimeAcc = 0.f;

private:
	virtual	HRESULT	Setup_Component();

public:
	virtual void On_Collision_Enter(CCollider* pCollider);
	virtual void On_Collision_Stay(CCollider* pCollider);
	virtual void On_Collision_Exit(CCollider* pCollider);


public:
	static CKeyPad* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END