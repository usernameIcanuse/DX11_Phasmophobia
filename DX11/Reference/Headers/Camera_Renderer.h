#pragma once

/* ȭ�鿡 �׷������� ��ü���� �׸��� ������� ��Ƽ� �����ϳ�.  */
/* ������ �ִ� ��ü���� �����Լ��� ȣ�����ֳ�. */

#include "Renderer.h"

BEGIN(Engine)

class CRenderTarget;

class ENGINE_DLL CCamera_Renderer final : public CRenderer
{
private:
	CCamera_Renderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Renderer(const CCamera_Renderer& rhs);
	virtual ~CCamera_Renderer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
public:
	virtual		HRESULT Render_Priority();
	virtual		HRESULT Render_NonAlphaBlend();
	virtual		HRESULT Render_Lights();
	virtual		HRESULT Render_Blend(); /* Diffuse * Shade ����ۿ� �׸���. */
	virtual		HRESULT Render_NonLight();
	virtual		HRESULT Render_AlphaBlend();
	virtual		HRESULT Render_UI();
	virtual HRESULT Draw_RenderGroup();
public:
	ID3D11ShaderResourceView* Get_SRV();

	virtual void	Clear_RenderTarget() override;


private:
	virtual HRESULT Begin_RenderTarget(const _tchar* _pMRTTag) override;
	virtual HRESULT End_RenderTarget() override;

	virtual void Set_Environment();
	virtual void End_Environment();

private:
	CRenderTarget* m_pCameraScreen = nullptr;
	class CTransform* m_pOwnerTransform = nullptr;

public:
	static CCamera_Renderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END