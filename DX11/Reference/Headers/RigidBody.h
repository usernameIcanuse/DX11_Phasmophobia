#pragma once

#include "Component.h"

BEGIN(Engine)

class CTransform;
class CNavigation;

class ENGINE_DLL CRigidBody final : public CComponent
{
public:
	typedef struct RigidBodyDesc
	{
		_float fWeight;
		CTransform* pOwnerTransform;
	}RIGIDBODYDESC;
private:
	CRigidBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRigidBody(const CRigidBody& rhs);
	virtual ~CRigidBody() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	void Add_Power(_vector _vPower);
	void Update(_float fTimeDelta, CNavigation* pCurNavi);

private:
	void Compute_Power();
	void Gravity(_float ftimeDelta);
	void Friction();
	void Apply_Transform(_float fTimeDelta, CNavigation* pCurNavi);

private:
	RIGIDBODYDESC  m_RigidBodyDesc;

	_bool			m_bOnGround = true;
	_bool			m_bGetPower = false;

	_float4					m_vPower = _float4(0.f, 0.f, 0.f,0.f);
	_float4					m_vCurPower = _float4(0.f, 0.f, 0.f, 0.f);
	
public:
	static CRigidBody* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END