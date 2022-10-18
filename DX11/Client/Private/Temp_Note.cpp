#include "stdafx.h"
#include "../Public/Temp_Note.h"
#include "GameInstance.h"

CTempNote::CTempNote(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CItem(pDevice, pContext)
{
}

CTempNote::CTempNote(const CTempNote& rhs)
    :CItem(rhs)
{
}

HRESULT CTempNote::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTempNote::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Setup_Component()))
        return E_FAIL;



    return S_OK;
}

void CTempNote::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
 
}

void CTempNote::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);

}

HRESULT CTempNote::Render()
{

    _uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
      
        
        m_pModelCom->Render(i, m_pShaderCom,1);
    }



    return S_OK;
}

void CTempNote::Set_TempModel_Pos(_float3 vPosition, COLLISION_TYPE eType, _float4 vLook, CItem* pConnectItem)
{
    if (eType == COLLISION_TYPE::OBJECT|| eType == COLLISION_TYPE::NAVIGATION)
    {
        _float3 vScale = m_pTransformCom->Get_Scaled();
        _vector vecLook = XMVector3Normalize(XMLoadFloat4(&vLook));
        m_pTransformCom->Set_State(CTransform::STATE_LOOK, vecLook * vScale.z);
        _vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
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


HRESULT CTempNote::Setup_Component()
{
    if (FAILED(__super::Setup_Component()))
        return E_FAIL;


    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Note"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;


    return S_OK;
}


CTempNote* CTempNote::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTempNote* pInstance = new CTempNote(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CTempNote");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTempNote::Clone(void* pArg)
{
    CTempNote* pInstance = new CTempNote(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CTempNote");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTempNote::Free()
{
    __super::Free();
}
