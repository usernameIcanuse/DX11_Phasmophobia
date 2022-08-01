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

	/* Ŭ�󿡼� �߰��� Ʈ�������� �ƴ϶�. �������� ������ �߰��ϴ� Ʈ�������̴�.  */
	/* Ŭ�󳻿��� ���� �������� �� �� �ֱ� ������, ���� Ŭ�󿡼��� Ʈ������ �±׸� Ȯ���� �� �ֶǷ� ���ֱ�����. */
	static const _tchar*			m_pTransformComTag;

protected:
	HRESULT Bind_PipeLine();
	

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END