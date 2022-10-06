#include "..\Public\Target_Manager.h"
#include "RenderTarget.h"

IMPLEMENT_SINGLETON(CTarget_Manager)

CTarget_Manager::CTarget_Manager()
{
}

ID3D11ShaderResourceView * CTarget_Manager::Get_SRV(const _tchar * pTargetTag)
{
	CRenderTarget*		pRenderTarget = Find_RenderTarget(pTargetTag);

	if (nullptr == pRenderTarget)
		return nullptr;

	return pRenderTarget->Get_SRV();	
}

HRESULT CTarget_Manager::Add_RenderTarget(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor)
{
	if (nullptr != Find_RenderTarget(pTargetTag))
		return E_FAIL;

	CRenderTarget*		pRenderTarget = CRenderTarget::Create(pDevice, pContext, iWidth, iHeight, eFormat, vClearColor);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	m_RenderTargets.emplace(pTargetTag, pRenderTarget);

	return S_OK;
}

HRESULT CTarget_Manager::Add_MRT(const _tchar * pMRTTag, const _tchar * pTargetTag)
{
	CRenderTarget*		pRenderTarget = Find_RenderTarget(pTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	list<CRenderTarget*>*		pMRTList = Find_MRT(pMRTTag);

	if (nullptr == pMRTList)
	{
		list<CRenderTarget*>		MRTList;
		MRTList.push_back(pRenderTarget);

		m_MRTs.emplace(pMRTTag, MRTList);
	}
	else
	{
		pMRTList->push_back(pRenderTarget);
	}

	Safe_AddRef(pRenderTarget);

	return S_OK;
}

HRESULT CTarget_Manager::Begin_MRT(ID3D11DeviceContext* pContext, const _tchar * pMRTTag)
{
	list<CRenderTarget*>*		pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	if (pMRTList->size() >= 8)
		return E_FAIL;

	ID3D11RenderTargetView*		RTVs[8] = { nullptr };

	_uint		iNumRTVs = 0;

	for (auto& pRenderTarget : *pMRTList)	
	{
		//pRenderTarget->Clear();
		RTVs[iNumRTVs++] = pRenderTarget->Get_RTV();	
	}
	
	Store_BackBuffer(pContext);
	/* 렌더타겟들을 장치에 바인딩한다. */
	pContext->OMSetRenderTargets(iNumRTVs, RTVs, m_pDepthStencilView);

	return S_OK;
}

HRESULT CTarget_Manager::End_MRT(ID3D11DeviceContext * pContext)
{
	Set_BackBuffer(pContext);

	return S_OK;
}

HRESULT CTarget_Manager::Begin_MRT_For_Texture(ID3D11DeviceContext* pContext, const _tchar* pMRTTag)
{
	list<CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	if (pMRTList->size() >= 8)
		return E_FAIL;

	ID3D11RenderTargetView* RTVs[8] = { nullptr };

	_uint		iNumRTVs = 0;

	for (auto& pRenderTarget : *pMRTList)
	{
		//pRenderTarget->Clear();
		RTVs[iNumRTVs++] = pRenderTarget->Get_RTV();
	}

	/* 렌더타겟들을 장치에 바인딩한다. */
	pContext->OMSetRenderTargets(iNumRTVs, RTVs, m_pDepthStencilView);

	return S_OK;
}

HRESULT CTarget_Manager::End_MRT_For_Texture(ID3D11DeviceContext* pContext, CRenderTarget* pRenderTarget)
{
	if (nullptr == pRenderTarget)
		return E_FAIL;
	ID3D11RenderTargetView* RTVs = pRenderTarget->Get_RTV();
		
	/* 렌더타겟들을 장치에 바인딩한다. */
	pContext->OMSetRenderTargets(1, &RTVs, m_pDepthStencilView);

	return S_OK;
}

HRESULT CTarget_Manager::Clear_MRT(const _tchar* pMRTTag)
{
	list<CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Clear();
	}

	return S_OK;
}

void CTarget_Manager::Set_BackBuffer(ID3D11DeviceContext* pContext)
{
	pContext->OMSetRenderTargets(1, &m_pBackBufferView, m_pDepthStencilView);

	Safe_Release(m_pBackBufferView);
	Safe_Release(m_pDepthStencilView);
}

void CTarget_Manager::Store_BackBuffer(ID3D11DeviceContext* pContext)
{
	/* 기존에 바인딩되어있던(백버퍼 + 깊이스텐실버퍼)를 얻어온다. */
	pContext->OMGetRenderTargets(1, &m_pBackBufferView, &m_pDepthStencilView);


}

#ifdef _DEBUG

HRESULT CTarget_Manager::Ready_Debug(const _tchar * pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	CRenderTarget*		pRenderTarget = Find_RenderTarget(pTargetTag);

	if (nullptr == pRenderTarget)
		return E_FAIL;

	return pRenderTarget->Ready_Debug(fX, fY, fSizeX, fSizeY);
}

HRESULT CTarget_Manager::Render_Debug(const _tchar* pMRTTag, CShader * pShader, CVIBuffer_Rect * pVIBuffer)
{
	list<class CRenderTarget*>*		pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Render_Debug(pShader, pVIBuffer);
	}

	return S_OK;
}
#endif // _DEBUG

CRenderTarget * CTarget_Manager::Find_RenderTarget(const _tchar * pTargetTag)
{
	auto	iter = find_if(m_RenderTargets.begin(), m_RenderTargets.end(), CTag_Finder(pTargetTag));

	if (iter == m_RenderTargets.end())
		return nullptr;

	return iter->second;	
}

list<class CRenderTarget*>* CTarget_Manager::Find_MRT(const _tchar * pMRTTag)
{
	auto	iter = find_if(m_MRTs.begin(), m_MRTs.end(), CTag_Finder(pMRTTag));

	if (iter == m_MRTs.end())
		return nullptr;

	return &iter->second;	
}

void CTarget_Manager::Free()
{
	for (auto& Pair : m_MRTs)
	{
		for (auto& pRenderTarget : Pair.second)
		{
			Safe_Release(pRenderTarget);
		}
		Pair.second.clear();
	}
	m_MRTs.clear();

	for (auto& Pair : m_RenderTargets)
		Safe_Release(Pair.second);

	m_RenderTargets.clear();
}
