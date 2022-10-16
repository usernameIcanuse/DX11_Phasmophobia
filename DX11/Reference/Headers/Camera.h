#pragma once

#include "GameObject.h"


BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct CameraDesc
	{
		/* 
		1. Ʈ������������ Ŭ�󿡼� �����ߴ�. 
		2. ������ü�� �����Ͽ� ������ ī�޶� Ʈ������ ������Ʈ�� �������� �߰��ϰ�ʹ�. 
		3. ���������� Ŭ�󿡼� �߰��� Ʈ������ ������ ������ �����ش�. 
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