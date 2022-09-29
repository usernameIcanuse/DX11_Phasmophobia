#pragma once
#include "Item.h"
#include "Client_Defines.h"

BEGIN(Client)

class CSpiritBox final : public CItem
{
private:
	CSpiritBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSpiritBox(const CSpiritBox& rhs);
	virtual ~CSpiritBox() = default;

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
	virtual void MalFunction(_float fTimeDelta = 0.f);
	virtual void Normal_Operation(_float fTimeDelta = 0.f);
	virtual void Frequency_Control(_long lMouseMove);


private:
	_long		m_lFrequency = 100;
	_tchar		m_szDegree[MAX_PATH] = TEXT("");//주파수 출력

#ifdef _DEBUG
	_tchar		m_szAnswer[MAX_PATH] = TEXT("");
	_float		m_fAnswerTime = 0.f;
#endif
	_float		m_fTimeAcc = 0.f;

	_float		m_fAnswerTimeLasting = 0.f;
	_bool		m_bInGhostArea = false;

public:
	virtual void On_Collision_Enter(CCollider* pCollider);
	virtual void On_Collision_Stay(CCollider* pCollider);
	virtual void On_Collision_Exit(CCollider* pCollider);

private:
	virtual	HRESULT	Setup_Component() override;

public:
	static CSpiritBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END