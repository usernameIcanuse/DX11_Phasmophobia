#include "..\Public\Renderer.h"
#include "GameObject.h"


CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eGroup, CGameObject * pGameObject)
{
	if (eGroup >= RENDER_END ||
		nullptr == pGameObject)
		return E_FAIL;

	m_RenderObjects[eGroup].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRenderer::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CRenderer::Draw_RenderGroup()
{
	for (_uint i = 0; i < RENDER_END; ++i)
	{
		if (i == RENDER_ALPHABLEND)
		{
			m_RenderObjects[i].sort([](CGameObject* pSour, CGameObject* pDest)
			{
				return pSour->Get_CamDistance() > pDest->Get_CamDistance();
			});			
		}

		for (auto& pGameObject : m_RenderObjects[i])
		{
			if (nullptr != pGameObject)
				pGameObject->Render();

			Safe_Release(pGameObject);
		}
		m_RenderObjects[i].clear();
	}

	return S_OK;
}

CRenderer * CRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRenderer*		pInstance = new CRenderer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CRenderer::Clone(void * pArg)
{
	AddRef();

	return this;	
}

void CRenderer::Free()
{
	__super::Free();

	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for (auto& pGameObject : m_RenderObjects[i])
			Safe_Release(pGameObject);

		m_RenderObjects[i].clear();		
	}
}
