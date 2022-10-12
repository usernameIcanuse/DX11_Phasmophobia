#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CLight;
class CModel;

#ifdef _DEBUG
class CCollider;
#endif

END

BEGIN(Client)

class CLightBulb final : public CGameObject
{
private:
	CLightBulb(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLightBulb(const CLightBulb& rhs);
	virtual ~CLightBulb() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void OnEventMessage(const _tchar* pMessage);
	virtual HRESULT SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix);

	virtual void Call_EventFunc(_float fTimeDelta = 0.f);
	virtual void MalFunction(_float fTimeDelta = 0.f);
	virtual void Normal_Operation(_float fTimeDelta = 0.f);

public:
	void	Turn_Switch(_bool _bSwitch)
	{
		m_bSwitch = _bSwitch;
	}

public:
	HRESULT	SetUp_ModelCom(const _tchar* pPrototypeTag);

private:
	CShader* m_pShaderCom = nullptr;
	CModel* m_pModelCom = nullptr;
	CLight* m_pLight = nullptr;

#ifdef _DEBUG
	CCollider* m_pOBBCom = nullptr;
#endif


	function<void(CLightBulb*, _float)> m_pEventFunc;

	_bool	m_bSwitch = false;

private:
	virtual	HRESULT	Setup_Component();
	HRESULT		Setup_Light();

public:
	virtual void On_Collision_Enter(CCollider* pCollider);
	virtual void On_Collision_Stay(CCollider* pCollider);
	virtual void On_Collision_Exit(CCollider* pCollider);


public:
	static CLightBulb* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END