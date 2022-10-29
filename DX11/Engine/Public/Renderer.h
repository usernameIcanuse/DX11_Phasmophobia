#pragma once

/* ȭ�鿡 �׷������� ��ü���� �׸��� ������� ��Ƽ� �����ϳ�.  */
/* ������ �ִ� ��ü���� �����Լ��� ȣ�����ֳ�. */

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer  : public CComponent
{
public:
	typedef struct tagRenderFont
	{
		const _tchar* pString;
		_vector vPosition;
		_vector vColor;
		_float rotation;
		_vector vOrigin;
		_vector vScale;
	}RENDERFONT;

public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_TERRAIN, RENDER_NONALPHABLEND, RENDER_DECAL, RENDER_NONLIGHT, RENDER_ALPHABLEND, RENDER_UI, RENDER_END };

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
	virtual HRESULT Draw_RenderGroup(_float fTimeDelta);

public:
	void Draw_On_Texture(class CRenderTarget* pRenderTarget,class CTexture* pTexture, class CShader* pShader, _int iPassindex,RENDERFONT& RenderFont, const _tchar* pTexttag);
	void Draw_On_Texture(class CRenderTarget* pRenderTarget, class CTexture* pSourTex, class CShader* pShader, _int iPassindex, _float3 vRenderPos,_float3 vScale);

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
////////////////post processing////////////////////
	CShader* m_pShaderPostProcess = nullptr;

	CRenderTarget* m_pRenderScreen = nullptr;
	_float m_fTimeDelta = 0.f;

	_int	m_iPassIndex = 0;

#ifdef _DEBUG

protected:
	vector<class CComponent*>	m_DebugComponents;
#endif // _DEBUG

public:
	virtual HRESULT Render_Priority();
	virtual HRESULT Render_NonAlphaBlend();
	virtual HRESULT Render_Decal();
	virtual HRESULT Render_Lights();
	virtual HRESULT Render_Blend(); /* Diffuse * Shade ����ۿ� �׸���. */ 
	virtual HRESULT Render_NonLight();
	virtual HRESULT Render_AlphaBlend();
	virtual HRESULT Render_UI();
	virtual HRESULT Post_Processing(_float fTimeDelta) { return S_OK; }

public:
	ID3D11ShaderResourceView* Get_SRV(const _tchar* pTargetTag);


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