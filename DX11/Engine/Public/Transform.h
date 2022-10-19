#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	typedef struct tagTransformDesc
	{
		_float			fSpeedPerSec;
		_float			fRotationPerSec;
	}TRANSFORMDESC;

public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_TRANSLATION, STATE_END };
	enum DIR   {RIGHT,LEFT,FRONT,BACK};
public:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	_vector Get_State(STATE eState) {
		return XMLoadFloat4x4(&m_WorldMatrix).r[eState];
	}

	_matrix Get_WorldMatrix() {
		return XMLoadFloat4x4(&m_WorldMatrix);
	}

	void Set_WorldMatrix(_matrix WorldMatrix)
	{
		XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);
	}

	void Set_State(STATE eState, _fvector vState)
	{
		_float4		vData;
		XMStoreFloat4(&vData, vState);
		memcpy(&m_WorldMatrix.m[eState], &vData, sizeof(_float4));		
	}

public:
	void Set_TransformDesc(const TRANSFORMDESC* pTransformDesc) {
		m_TransformDesc = *pTransformDesc;
	}

	void Set_Scaled(_float3 vScale);
	_float3 Get_Scaled();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Set_ShaderResource(class CShader* pShader, const char* pConstantName);

public:
	HRESULT Go_Straight(_float fTimeDelta, class CNavigation* pNaviCom = nullptr);
	HRESULT Go_Backward(_float fTimeDelta, class CNavigation* pNaviCom = nullptr);
	HRESULT Go_Left(_float fTimeDelta, class CNavigation* pNaviCom = nullptr);
	HRESULT Go_Right(_float fTimeDelta, class CNavigation* pNaviCom = nullptr);

	void Add_Direction(DIR eState);
	void Reset_Direction()
	{
		m_vMoveDir = _float3(0.f, 0.f, 0.f);
	}

	HRESULT Move(_float fTimeDelta, class CNavigation* pNaviCom = nullptr);
	HRESULT Move(_vector vAccel, _float fTimeDelta, class CNavigation* pNaviCom);
	HRESULT Slide_Object(_float3 vContactDirection, CNavigation* pNaviCom);

	void Turn(_fvector vAxis, _float fTimeDelta);
	void Rotation(_fvector vAxis, _float fRadian);
	void LookAt(_fvector vTargetPos);
	void Chase(_fvector vTargetPos, _float fTimeDelta);

	_float3 Get_MoveDir()
	{
		return m_vMoveDir;
	}

private:
	_float4x4				m_WorldMatrix;
	TRANSFORMDESC			m_TransformDesc;

	_float3					m_vMoveDir = _float3(0.f, 0.f, 0.f);
public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END