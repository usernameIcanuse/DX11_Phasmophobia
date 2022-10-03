#include "..\Public\Camera_Renderer.h"
#include "GameInstance.h"
#include "RenderTarget.h"


CCamera_Renderer::CCamera_Renderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CRenderer(pDevice, pContext)
{
}

HRESULT CCamera_Renderer::Initialize(void* pArg)
{
	GAMEINSTANCE->Add_Renderer(*(CRenderer_Manager::RENDERER_TYPE*)pArg, this);

	return S_OK;
}

ID3D11ShaderResourceView* CCamera_Renderer::Get_SRV()
{
	return m_pCameraScreen->Get_SRV();
}

CCamera_Renderer * CCamera_Renderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Renderer*		pInstance = new CCamera_Renderer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Renderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCamera_Renderer::Clone(void * pArg)
{
	CCamera_Renderer* pInstance = new CCamera_Renderer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Renderer");
		Safe_Release(pInstance);
	}

	return pInstance;	
}

void CCamera_Renderer::Free()
{
	__super::Free();

}
