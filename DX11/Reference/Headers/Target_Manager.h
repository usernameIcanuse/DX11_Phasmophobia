#pragma once

/* 렌더타겟들을 모아놓는다. */
#include "Base.h"

BEGIN(Engine)

class CTarget_Manager final : public CBase
{
	DECLARE_SINGLETON(CTarget_Manager)
public:
	CTarget_Manager();
	virtual ~CTarget_Manager() = default;

public:
	ID3D11ShaderResourceView* Get_SRV(const _tchar* pTargetTag);

public:
	HRESULT Add_RenderTarget(ID3D11Device*	pDevice, ID3D11DeviceContext* pContext, const _tchar* pTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor);
	HRESULT Add_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);

	HRESULT Begin_MRT(ID3D11DeviceContext* pContext, const _tchar* pMRTTag);
	HRESULT End_MRT(ID3D11DeviceContext* pContext);

	HRESULT Begin_MRT_For_Texture(ID3D11DeviceContext* pContext, const _tchar* pMRTTag);
	HRESULT End_MRT_For_Texture(ID3D11DeviceContext* pContext, class CRenderTarget* pRenderTarget);
	HRESULT Set_RenderTarget(ID3D11DeviceContext* pContext, class CRenderTarget* pRenderTarget);

	HRESULT	Clear_MRT(const _tchar* pMRTTag);
	
	void Set_BackBuffer(ID3D11DeviceContext* pContext);
	void Store_BackBuffer(ID3D11DeviceContext* pContext);

#ifdef _DEBUG
public:
	HRESULT Ready_Debug(const _tchar* pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug(const _tchar* pMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif // _DEBUG

private:
	/* 렌더타겟들을 모아놓는다. */
	map<const _tchar*, class CRenderTarget*>			m_RenderTargets;
	typedef map<const _tchar*, class CRenderTarget*>	RENDERTARGETS;

	/* 동시에 바인딩되어야하는 렌더타겟들을 그룹지어놓는다. */
	map<const _tchar*, list<class CRenderTarget*>>				m_MRTs;
	typedef map<const _tchar*, list<class CRenderTarget*>>		MRTS;

private:
	ID3D11RenderTargetView*					m_pBackBufferView = nullptr;
	ID3D11DepthStencilView*					m_pDepthStencilView = nullptr;

public:
	class CRenderTarget* Find_RenderTarget(const _tchar* pTargetTag);
	list<class CRenderTarget*>* Find_MRT(const _tchar* pMRTTag);


public:
	virtual void Free();
};

END