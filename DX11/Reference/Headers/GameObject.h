#pragma once

#include "Base.h"
#include "Transform.h"

BEGIN(Engine)
class CCollider;

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);	
	CGameObject(const CGameObject& Prototype);
	virtual ~CGameObject() = default;

public:
	class CComponent* Get_Component(const _tchar* pComponentTag);

	_float Get_CamDistance() {
		return m_fCamDistance;
	}

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render(); 
	

public:
	virtual void OnEventMessage(const _tchar* pMessage) {}
	virtual void Call_EventFunc(_float fTimeDelta = 0.f) {}
	virtual HRESULT SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix) { return S_OK; };
public:
	static const _tchar*		m_pTransformTag;

protected:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pContext = nullptr;

	class CTransform*			m_pTransformCom = nullptr;

	_float						m_fCamDistance = 0.f;

protected:
	map<const _tchar*, class CComponent*>			m_Components;
	typedef map<const _tchar*, class CComponent*>	COMPONENTS;

	
	_bool		m_bEnable = true;

protected:
	HRESULT Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, class CComponent** ppOut, void* pArg = nullptr);
public:

	virtual  void	Set_Enable(_bool _bEnable)
	{
		m_bEnable = _bEnable;
		for (auto& elem : m_Components)
		{
			elem.second->Set_Enable(m_bEnable);
		}
	}

	_bool Get_Enable()
	{
		return m_bEnable;
	}

public:
	virtual void On_Collision_Enter(CCollider* pCollider) {};
	virtual void On_Collision_Stay(CCollider* pCollider) {};
	virtual void On_Collision_Exit(CCollider* pCollider) {};


private:
	class CComponent* Find_Components(const _tchar* pComponentTag);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END