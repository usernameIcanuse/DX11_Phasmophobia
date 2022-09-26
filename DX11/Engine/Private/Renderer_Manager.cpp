#include "..\Public\Renderer_Manager.h"
#include "Renderer.h"

IMPLEMENT_SINGLETON(CRenderer_Manager)

CRenderer_Manager::CRenderer_Manager()
{

}

HRESULT CRenderer_Manager::Add_Renderer(_uint eListIndex, CRenderer* pRenderer)
{
	if (eListIndex >= RENDERER_END ||
		nullptr == pRenderer)
		return E_FAIL;

	m_RendererList[eListIndex].push_back(pRenderer);
	Safe_AddRef(pRenderer);

	return S_OK;
}

HRESULT CRenderer_Manager::Draw_RenderGroup()
{
	for (_uint i = 0; i < RENDERER_STATIC; ++i)
	{
		for (auto& elem : m_RendererList[i])
		{
			if(nullptr != elem)
				elem->Draw_RenderGroup();
			Safe_Release(elem);
		}

		m_RendererList[i].clear();
	}

	for (auto& elem : m_RendererList[RENDERER_STATIC])
	{
		if (nullptr != elem)
			elem->Draw_RenderGroup();
	}
	return S_OK;
}

void CRenderer_Manager::Clear_RendererIndex(_uint eListIndex)
{
	for (auto& elem : m_RendererList[eListIndex])
	{
		Safe_Release(elem);
	}
	m_RendererList[eListIndex].clear();
}



void CRenderer_Manager::Free()
{
	for (_uint i = 0; i < RENDERER_END; ++i)
	{
		for (auto& elem : m_RendererList[i])
			Safe_Release(elem);

		m_RendererList[i].clear();
	}
	
}
