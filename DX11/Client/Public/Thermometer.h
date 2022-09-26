#pragma once
#include "Item.h"
#include "Client_Defines.h"

BEGIN(Client)

class CThermometer final : public CItem
{
private:
	CThermometer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CThermometer(const CThermometer& rhs);
	virtual ~CThermometer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	//virtual void OnEventMessage(const _tchar* pMessage);


public:
	virtual void Turn_Switch()
	{
		m_bSwitch = !m_bSwitch; 
	}

	virtual void MalFunction(_float fTimeDelta = 0.f);
	virtual void Normal_Operation(_float fTimeDelta = 0.f);

private:
	_int		m_iDegree = 0;
	_tchar		m_szDegree[MAX_PATH] = TEXT("");//온도 출력
	_float		m_fTimeAcc = 0.f;//온도 갱신

public:
	virtual void On_Collision_Enter(CCollider* pCollider);
	virtual void On_Collision_Stay(CCollider* pCollider);
	virtual void On_Collision_Exit(CCollider* pCollider);

private:
	virtual	HRESULT	Setup_Component() override;
	virtual HRESULT SetUp_ShaderResource() override;
public:
	static CThermometer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END