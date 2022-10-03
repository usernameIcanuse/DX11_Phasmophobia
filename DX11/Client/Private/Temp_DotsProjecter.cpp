#include "stdafx.h"
#include "../Public/Temp_DotsProjecter.h"
#include "GameInstance.h"

CTempDotsProjecter::CTempDotsProjecter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CItem(pDevice,pContext)
{
}

CTempDotsProjecter::CTempDotsProjecter(const CTempDotsProjecter& rhs)
    :CItem(rhs)
{
}

HRESULT CTempDotsProjecter::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTempDotsProjecter::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

   // m_pTransformCom->Set_Scaled(_float3(5.f, 5.f, 5.f));

    if (FAILED(Setup_Component()))
        return E_FAIL;



    return S_OK;
}

void CTempDotsProjecter::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);


}

void CTempDotsProjecter::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    GAMEINSTANCE->Add_Object_For_Culling(this, CRenderer::RENDER_ALPHABLEND);

}

HRESULT CTempDotsProjecter::Render()
{


    _uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
            return E_FAIL;
       /* if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
            return E_FAIL;*/

        
        m_pModelCom->Render(i, m_pShaderCom,1);
    }


    return S_OK;
}


void CTempDotsProjecter::Set_TempModel_Pos(_float3 vPosition, COLLISION_TYPE eType, _float4 vLook, CItem* pConnectItem)
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

        m_pTransformCom->Rotation(vUp, XMConvertToRadians(180.f));
    }
    else if (eType == COLLISION_TYPE::OBJECT)
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

        m_pTransformCom->Rotation(vUp, XMConvertToRadians(180.f));
        m_pTransformCom->Rotation(vecLook, XMConvertToRadians(180.f));
        
        
    }

}


HRESULT CTempDotsProjecter::Setup_Component()
{

    if (FAILED(__super::Setup_Component()))
        return E_FAIL;
 
    /* For.Com_Model */
     if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Model_DotsProjecter"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;


    return S_OK;
}


CTempDotsProjecter* CTempDotsProjecter::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTempDotsProjecter* pInstance = new CTempDotsProjecter(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CTempDotsProjecter");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTempDotsProjecter::Clone(void* pArg)
{
    CTempDotsProjecter* pInstance = new CTempDotsProjecter(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CTempDotsProjecter");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTempDotsProjecter::Free()
{
    __super::Free();

}
