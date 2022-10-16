#include "..\Public\Camera.h"
#include "GameInstance.h"

CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CCamera::CCamera(const CCamera & Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	memcpy(&m_CameraDesc, pArg, sizeof(CAMERADESC));

	m_pTransformCom->Set_TransformDesc(&m_CameraDesc.TransformDesc);	
	/*_vector		vPos = XMLoadFloat3(&m_CameraDesc.vEye);
	vPos = XMVectorSetW(vPos, 1.f);*/
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_CameraDesc.vEye));
	m_pTransformCom->LookAt(XMLoadFloat4(&m_CameraDesc.vAt));

	return S_OK;
}


HRESULT CCamera::Bind_PipeLine()
{
	CPipeLine*			pPipeLine = GET_INSTANCE(CPipeLine);	

	_matrix		ViewMatrix = XMMatrixInverse(nullptr, m_pTransformCom->Get_WorldMatrix());
	pPipeLine->Set_Transform(CPipeLine::D3DTS_VIEW, ViewMatrix);

	_matrix		ProjMatrix = XMMatrixPerspectiveFovLH(m_CameraDesc.fFovy, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar);
	pPipeLine->Set_Transform(CPipeLine::D3DTS_PROJ, ProjMatrix);	

	RELEASE_INSTANCE(CPipeLine);

	return S_OK;
}

void CCamera::Free()
{
	__super::Free();


}
