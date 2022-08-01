#include "..\Public\Camera.h"
#include "GameInstance.h"

const _tchar*			CCamera::m_pTransformComTag = TEXT("Com_Transform");

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

	///* 트랜스폼 컴포너늩를 복제한다. */
	///* 트랜스폼을 사용할 놈의 초당 속도, 초당회전속도를 전달. */
	//if (FAILED(__super::Add_Component(m_CameraDesc.iLevelIndex, m_CameraDesc.pTransformPrototypeTag, m_pTransformComTag, (CComponent**)&m_pTransformCom, &m_CameraDesc.TransformDesc)))
	//	return E_FAIL;

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_CameraDesc.vEye);
	//m_pTransformCom->LookAt(m_CameraDesc.vAt);

	return S_OK;
}

void CCamera::Tick(_float fTimeDelta)
{
}

void CCamera::LateTick(_float fTimeDelta)
{
}

HRESULT CCamera::Render()
{
	return S_OK;
}

HRESULT CCamera::Bind_PipeLine()
{
	//if (nullptr == m_pGraphic_Device)
	//	return E_FAIL;

	//_float4x4		WorldMatrix = m_pTransformCom->Get_WorldMatrix();
	//_float4x4		ViewMatrix;

	//D3DXMatrixInverse(&ViewMatrix, nullptr, &WorldMatrix);
	//
	//m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);

	//_float4x4		ProjMatrix;

	//D3DXMatrixPerspectiveFovLH(&ProjMatrix, m_CameraDesc.fFovy, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar);

	//m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &ProjMatrix);

	return S_OK;
}

void CCamera::Free()
{
	__super::Free();


}
