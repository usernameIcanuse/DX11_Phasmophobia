#pragma once

#include "GameObject.h"


BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct CameraDesc
	{
		/* 
		1. 트랜스폼원형을 클라에서 생성했다. 
		2. 원형객체를 복제하여 생성한 카메라에 트랜스폼 컴포넌트를 엔진에서 추가하고싶다. 
		3. 엔진내에서 클라에서 추가한 트랜스폼 원형의 정보를 던져준다. 
		*/
		_float4		vEye;
		_float4		vAt;
		_float		fFovy;
		_float		fAspect;
		_float		fNear;
		_float		fFar;

		CTransform::TRANSFORMDESC		TransformDesc;

	}CAMERADESC;
protected:
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera(const CCamera& Prototype);
	virtual ~CCamera() = default;
	
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

protected:
	CAMERADESC						m_CameraDesc;

public:
	HRESULT Bind_PipeLine();	

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END