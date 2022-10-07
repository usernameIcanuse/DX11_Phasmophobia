#pragma once

/* ȭ�鿡 �׷������� ��ü���� �׸��� ������� ��Ƽ� �����ϳ�.  */
/* ������ �ִ� ��ü���� �����Լ��� ȣ�����ֳ�. */

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer  : public CComponent
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_TERRAIN,RENDER_NONALPHABLEND, RENDER_NONLIGHT, RENDER_ALPHABLEND, RENDER_UI, RENDER_END };
	/* Emissive�� ���� ����Ÿ�� �ϳ� �� ���� NONALPHABLEND���� ĥ�ϰ� BLEND���� ���� �����ֱ�*/
	/*
	* �ؽ�ó�� �׷��� �ϴ� ���, �ؽ�ó ũ�� 2048 2048�� ���� �ٵ� �ٸ� ����� ���� �� �����ϱ�
	* �⺻ �ؽ�ó ������ ���� ��, ����Ÿ�� ����� ����
	* ���� ���� ���� mainapp���� m_pRenderer->Render_On_Texture() <- �۾��� ����
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
	/*ī�޶�, ������ ī�޶� ���� ���� ����*/
	_float4x4					m_CamViewMat, m_CamProjMat;
	_float4x4					m_CamViewInv, m_CamProjInv;
	_float4						m_vCamPosition;
	/* ���������� ���� �����̴�. */
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
	virtual HRESULT Render_Blend(); /* Diffuse * Shade ����ۿ� �׸���. */ 
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