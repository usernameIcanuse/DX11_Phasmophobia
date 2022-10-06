#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CModel;
class CCollider;

END

BEGIN(Client)

class CLightSwitch final : public CGameObject
{
private:
	CLightSwitch(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLightSwitch(const CLightSwitch& rhs);
	virtual ~CLightSwitch() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();


public:
	HRESULT	SetUp_ModelCom(const _tchar* pPrototypeTag);

private:
	CShader* m_pShaderCom = nullptr;
	//CTexture* m_pTextureCom = nullptr;
	//CRenderer* m_pRendererCom = nullptr;
	CModel* m_pModelCom = nullptr;
	CCollider* m_pOBBCom = nullptr;

	_bool	m_bSwitch = false;

private:
	virtual	HRESULT	Setup_Component();
	virtual HRESULT SetUp_ShaderResource();

public:
	virtual void On_Collision_Enter(CCollider* pCollider);
	virtual void On_Collision_Stay(CCollider* pCollider);
	virtual void On_Collision_Exit(CCollider* pCollider);


public:
	static CLightSwitch* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END