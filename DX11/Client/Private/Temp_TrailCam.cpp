#include "stdafx.h"
#include "../Public/Temp_TrailCam.h"
#include "GameInstance.h"

CTempTrailCam::CTempTrailCam(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CItem(pDevice, pContext)
{
}

CTempTrailCam::CTempTrailCam(const CTempTrailCam& rhs)
    :CItem(rhs)
{
}

HRESULT CTempTrailCam::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTempTrailCam::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Setup_Component()))
        return E_FAIL;



    return S_OK;
}

void CTempTrailCam::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

}

void CTempTrailCam::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);




    m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);

}

HRESULT CTempTrailCam::Render()
{

    _uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {

        
        m_pModelCom->Render(i, m_pShaderCom,1);
    }



    return S_OK;
}


HRESULT CTempTrailCam::Setup_Component()
{
    if (FAILED(__super::Setup_Component()))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_TrailCam"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;

   
    return S_OK;
}


void CTempTrailCam::Set_TempModel_Pos(_float3 vPosition, COLLISION_TYPE eType, _float4 vLook, CItem* pConnectItem)
{
    if (eType == COLLISION_TYPE::WALL)
    {
        _float3 vScale = m_pTransformCom->Get_Scaled();
        _vector vecLook = XMVector3Normalize(XMLoadFloat4(&vLook));
        m_pTransformCom->Set_State(CTransform::STATE_LOOK, vecLook * vScale.z);
        _vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
        _vector vRight = XMVector3Cross(vUp, XMLoadFloat4(&vLook));

        vUp = XMVector3Cross(XMLoadFloat4(&vLook), vRight);


        vRight = XMVector3Normalize(vRight);
        vUp = XMVector3Normalize(vUp);

        m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
        m_pTransformCom->Set_State(CTransform::STATE_UP, vUp * vScale.y);
        m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&vPosition), 1.f));

      //  m_pTransformCom->Rotation(vUp, XMConvertToRadians(180.f));

       
    }
    else if (eType == COLLISION_TYPE::OBJECT || eType == COLLISION_TYPE::NAVIGATION)
    {
        _float3 vScale = m_pTransformCom->Get_Scaled();
        _vector vecLook = XMVector3Normalize(XMLoadFloat4(&vLook));
        m_pTransformCom->Set_State(CTransform::STATE_LOOK, vecLook * vScale.z);
        _vector vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f);
        _vector vUp = XMVector3Cross(XMLoadFloat4(&vLook), vRight);


        vRight = XMVector3Normalize(vRight);
        vUp = XMVector3Normalize(vUp);

        m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
        m_pTransformCom->Set_State(CTransform::STATE_UP, vUp * vScale.y);
        m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&vPosition), 1.f));

       // m_pTransformCom->Rotation(vUp, XMConvertToRadians(180.f));
        m_pTransformCom->Rotation(vecLook, XMConvertToRadians(180.f));


    }
}

CTempTrailCam* CTempTrailCam::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTempTrailCam* pInstance = new CTempTrailCam(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CTempTrailCam");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTempTrailCam::Clone(void* pArg)
{
    CTempTrailCam* pInstance = new CTempTrailCam(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CTempTrailCam");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTempTrailCam::Free()
{
    __super::Free();

}
