#pragma once
#include "GameObject.h"
#include "Client_Defines.h"


BEGIN(Client)

class CGhost_SpawnPoint;


class CGhost_Status final: public CGameObject
{
	friend class CGhost_SpawnPoint;
private:
	CGhost_Status(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGhost_Status(const CGhost_Status& rhs);
	virtual ~CGhost_Status() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void	Increase_BaseLine();
	void	Decrease_BaseLine();

private:
	_uint	   m_iAggression = 0;
	_uint	   m_iEMF = 1;

	_float 	    m_fAggressionWeight = 0.f;//올림
	_float	    m_fCalmWeight= 0.f;//내림 & 1-올림-내림 == 유지

	_float	    m_fEventWeight = 0.f;//이벤트 발생 & 1-이벤트 발생 == 안 발생
	_float		m_fAttackWeight = 0.f;//헌팅 & 1-헌팅 == 안 헌팅

	_uint		m_iAggressionLine = 0.f;
	_uint		m_iCalmLine = 0.f;

	_uint		m_iBaseLine = 0.f; //이벤트, 헌팅 기준

	_float		m_fTime = 0.f;

	_float		m_fEventCoolTime = 100.f;//이벤트와 헌팅
	_float		m_fEventTime = 10.f;
	_float		m_fAttackTime = 10.f;
	

public:
	static CGhost_Status* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;//SpawnPoint를 넘겨줌
	virtual void Free() override;

};

END
