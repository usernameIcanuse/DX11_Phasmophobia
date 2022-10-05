#pragma once

/* 화면에 그려져야할 객체들을 그리는 순서대로 모아서 관리하낟.  */
/* 가지고 있는 객체들의 렌더함수를 호출해주낟. */

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer  : public CComponent
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_NONALPHABLEND, RENDER_NONLIGHT, RENDER_ALPHABLEND, RENDER_UI, RENDER_END };

protected:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;
	 
public:
	HRESULT Add_RenderGroup(RENDERGROUP	eGroup, class CGameObject* pGameObject);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	virtual void	Clear_RenderTarget() {}

protected:
	virtual HRESULT Begin_RenderTarget(const _tchar* _pMRTTag);
	virtual HRESULT End_RenderTarget();

	virtual void Set_Environment();
	virtual void End_Environment() {};

public:
	virtual HRESULT Draw_RenderGroup();

protected:
	list<class CGameObject*>				m_RenderObjects[RENDER_END];
	typedef list<class CGameObject*>		RENDEROBJECTS;

protected:
	class CTarget_Manager*					m_pTarget_Manager = nullptr;
	class CLight_Manager*					m_pLight_Manager = nullptr;


#ifdef _DEBUG
/* 직교투영을 위한 정보이다. */
protected:
	_float4x4					m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;
	class CShader*				m_pShader = nullptr;
	class CVIBuffer_Rect*		m_pVIBuffer = nullptr;

	/*카메라, 아이템 카메라 투영 위한 정보*/
	_float4x4					m_CamViewMat, m_CamProjMat;
	_float4x4					m_CamViewInv, m_CamProjInv;
	_float4						m_vCamPosition;

protected:
	vector<class CComponent*>	m_DebugComponents;
#endif // _DEBUG

public:
	virtual HRESULT Render_Priority();
	virtual HRESULT Render_NonAlphaBlend();
	virtual HRESULT Render_Lights();
	virtual HRESULT Render_Blend(); /* Diffuse * Shade 백버퍼에 그린다. */ 
	virtual HRESULT Render_NonLight();
	virtual HRESULT Render_AlphaBlend();
	virtual HRESULT Render_UI();

#ifdef _DEBUG
public:
	HRESULT Render_Debug();
	HRESULT Add_DebugRenderGroup(class CComponent* pComponent);
#endif // _DEBUG


public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END