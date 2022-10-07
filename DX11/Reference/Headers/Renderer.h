#pragma once

/* 화면에 그려져야할 객체들을 그리는 순서대로 모아서 관리하낟.  */
/* 가지고 있는 객체들의 렌더함수를 호출해주낟. */

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer  : public CComponent
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_TERRAIN,RENDER_NONALPHABLEND, RENDER_NONLIGHT, RENDER_ALPHABLEND, RENDER_UI, RENDER_END };
	/* Emissive를 위한 렌더타겟 하나 더 만들어서 NONALPHABLEND에서 칠하고 BLEND에서 같이 섞어주기*/
	/*
	* 텍스처에 그려야 하는 경우, 텍스처 크기 2048 2048로 같음 근데 다른 사이즈가 있을 수 있으니까
	* 기본 텍스처 렌더러 만들 때, 렌더타겟 만들고 지움
	* 렌더 돌기 전에 mainapp에서 m_pRenderer->Render_On_Texture() <- 작업을 해줌
	* 
	*/
protected:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRenderer(const CRenderer& rhs);
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

public:
	void Draw_On_Texture(class CRenderTarget* pRenderTarget,class CTexture* pTexture, class CShader* pShader, _int iPassindex, const _tchar* pText, _float2 vRenderPos, const _tchar* pTexttag);
	void Draw_On_Texture(class CRenderTarget* pRenderTarget, class CTexture* pTexture[], class CShader* pShader, _int iPassindex, _float3 vRenderPos[]);

protected:
	list<class CGameObject*>				m_RenderObjects[RENDER_END];
	typedef list<class CGameObject*>		RENDEROBJECTS;

protected:
	class CTarget_Manager*					m_pTarget_Manager = nullptr;
	class CLight_Manager*					m_pLight_Manager = nullptr;
	/*카메라, 아이템 카메라 투영 위한 정보*/
	_float4x4					m_CamViewMat, m_CamProjMat;
	_float4x4					m_CamViewInv, m_CamProjInv;
	_float4						m_vCamPosition;
	/* 직교투영을 위한 정보이다. */
	_float4x4					m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;

	class CShader*				m_pShader = nullptr;
	class CVIBuffer_Rect*		m_pVIBuffer = nullptr;

#ifdef _DEBUG

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