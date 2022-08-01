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
		_uint		iLevelIndex;
		_tchar*		pTransformPrototypeTag;

		_float3		vEye;
		_float3		vAt;
		_float		fFovy;
		_float		fAspect;
		_float		fNear;
		_float		fFar;

		// CTransform::TRANSFORMDESC		TransformDesc;

	}CAMERADESC;
protected:
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera(const CCamera& Prototype);
	virtual ~CCamera() = default;
	
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	class CTransform*				m_pTransformCom = nullptr;
	CAMERADESC						m_CameraDesc;

	/* 클라에서 추가한 트랜스폼이 아니라. 엔진에서 스스로 추가하는 트랜스폼이다.  */
	/* 클라내에서 자주 ㄱ덤색이 될 수 있기 때문에, 쉽게 클라에서도 트랜스폼 태그를 확인할 수 있또록 해주기위해. */
	static const _tchar*			m_pTransformComTag;

protected:
	HRESULT Bind_PipeLine();
	

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END