#pragma once
#include "Item.h"
#include "Client_Defines.h"

BEGIN(Client)

class CEMF final : public CItem
{
private:
	CEMF(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEMF(const CEMF& rhs);
	virtual ~CEMF() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void OnEventMessage(const _tchar* pMessage);


public:
	virtual void Turn_Switch()
	{
		m_bSwitch = !m_bSwitch;
	}

	

public:
	virtual void On_Collision_Enter(CCollider* pCollider);
	virtual void On_Collision_Stay(CCollider* pCollider);
	virtual void On_Collision_Exit(CCollider* pCollider);

public:
	void		Get_Ghost_Anger(_uint _iLevel)
	{
		m_iEMFLevel = _iLevel;
	}
private:
	_uint		m_iEMFLevel = 1;
	_tchar		m_szDegree[MAX_PATH] = TEXT("");//임시 emf레벨 출력

private:
	virtual	HRESULT	Setup_Component() override;

public:
	static CEMF* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END