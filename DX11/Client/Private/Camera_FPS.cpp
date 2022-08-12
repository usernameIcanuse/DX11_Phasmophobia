#include "stdafx.h"
#include "Camera_FPS.h"

CCamera_FPS::CCamera_FPS(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCamera(pDevice,pContext)
{
}

CCamera_FPS::CCamera_FPS(const CCamera_FPS& rhs)
	:CCamera(rhs)
{
}

HRESULT CCamera_FPS::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_FPS::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCamera_FPS::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	_matrix matTarget = m_pTarget->Get_WorldMatrix();

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, matTarget.r[0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, matTarget.r[1]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, matTarget.r[2]);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, matTarget.r[3]);


	if (FAILED(Bind_PipeLine()))
		return;
}

void CCamera_FPS::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_FPS::Render()
{
	return S_OK;
}


CCamera_FPS* CCamera_FPS::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_FPS* pInstance = new CCamera_FPS(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_FPS");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_FPS::Clone(void* pArg)
{
	CCamera_FPS* pInstance = new CCamera_FPS(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_FPS");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_FPS::Free()
{
	__super::Free();
}
