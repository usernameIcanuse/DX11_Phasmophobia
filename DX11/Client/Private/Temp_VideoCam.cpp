#include "stdafx.h"
#include "../Public/Temp_VideoCam.h"
#include "GameInstance.h"
#include "Tripod.h"

CTempVideoCam::CTempVideoCam(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CItem(pDevice, pContext)
{
}

CTempVideoCam::CTempVideoCam(const CTempVideoCam& rhs)
    :CItem(rhs)
{
}

HRESULT CTempVideoCam::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTempVideoCam::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Setup_Component()))
        return E_FAIL;

    m_vAdjustpos = _float3(1.f, 1.f, 1.9f);


    return S_OK;
}

void CTempVideoCam::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

}

void CTempVideoCam::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);

}

HRESULT CTempVideoCam::Render()
{

    _uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {

        m_pModelCom->Render(i, m_pShaderCom,1);
    }


    return S_OK;
}

void CTempVideoCam::Set_TempModel_Pos(_float3 vPosition, COLLISION_TYPE eType, _float4 vLook, CItem* pConnectItem)
{
    if (eType == COLLISION_TYPE::OBJECT || eType == COLLISION_TYPE::NAVIGATION)
    {
        CTransform* pTransform = (CTransform*)pConnectItem->Get_Component(CGameObject::m_pTransformTag);

        _float3 vScale = m_pTransformCom->Get_Scaled();

        _vector vUp = XMLoadFloat4(&vLook);
        _vector vecLook = pTransform->Get_State(CTransform::STATE_LOOK) * -1.f;
        _vector vRight = XMVector3Cross(vUp, vecLook);
        vecLook = XMVector3Cross(vRight, vUp);

        vecLook = XMVector3Normalize(vecLook);
        vUp= XMVector3Normalize(vUp);
        vRight= XMVector3Normalize(vRight);

        m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight*vScale.x);
        m_pTransformCom->Set_State(CTransform::STATE_UP, vUp * vScale.y);
        m_pTransformCom->Set_State(CTransform::STATE_LOOK, vecLook*vScale.z);

        m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&vPosition), 1.f));

    }

   else if (eType == COLLISION_TYPE::TRIPOD)
    {
       CTransform* pTransform = (CTransform*)pConnectItem->Get_Component(CGameObject::m_pTransformTag);
       
       _float3 vScale = m_pTransformCom->Get_Scaled();

       _vector vUp = XMLoadFloat4(&vLook);
       _vector vecLook = pTransform->Get_State(CTransform::STATE_LOOK);
       _vector vRight = pTransform->Get_State(CTransform::STATE_RIGHT);
     

       vecLook = XMVector3Normalize(vecLook);
       vUp = XMVector3Normalize(vUp);
       vRight = XMVector3Normalize(vRight);

       m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
       m_pTransformCom->Set_State(CTransform::STATE_UP, vUp * vScale.y);
       m_pTransformCom->Set_State(CTransform::STATE_LOOK, vecLook * vScale.z);

       m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&vPosition), 1.f));

   }

}



HRESULT CTempVideoCam::Setup_Component()
{

    if (FAILED(__super::Setup_Component()))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Video_Camera"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;

 

    return S_OK;
}


CTempVideoCam* CTempVideoCam::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTempVideoCam* pInstance = new CTempVideoCam(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CTempVideoCam");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTempVideoCam::Clone(void* pArg)
{
    CTempVideoCam* pInstance = new CTempVideoCam(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CTempVideoCam");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTempVideoCam::Free()
{
    __super::Free();
}
