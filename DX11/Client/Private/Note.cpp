#include "stdafx.h"
#include "../Public/Note.h"
#include "GameInstance.h"
#include "Ghost.h"
CNote::CNote(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CItem(pDevice, pContext)
{
}

CNote::CNote(const CNote& rhs)
    :CItem(rhs)
{
}

HRESULT CNote::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CNote::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(nullptr)))
        return E_FAIL;

    if (nullptr != pArg)
    {
        m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4((_float4x4*)pArg));
    }

    if (FAILED(Setup_Component()))
        return E_FAIL;
    
    if (FAILED(Setup_TempModel()))
        return E_FAIL;

    m_vAdjustpos = _float3(1.f, 1.3f, 2.f);

    return S_OK;
}

void CNote::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
    m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
    if (m_bInstalled)
    {
        m_pNoteOpenModel->Play_Animation(fTimeDelta);
    }

}

void CNote::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    GAMEINSTANCE->Add_Object_For_Culling(this, CRenderer::RENDER_NONALPHABLEND);

#ifdef _DEBUG
   // m_pRendererCom->Add_DebugRenderGroup(m_pOBBCom);
#endif
}

HRESULT CNote::Render()
{
    CModel* pModelCom = nullptr;

    if (m_bInstalled)
        pModelCom = m_pNoteOpenModel;
    else
        pModelCom = m_pModelCom;

    _uint iNumMeshContainers = pModelCom->Get_NumMeshContainers();

    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        if (FAILED(pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
            return E_FAIL;
      /*  if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
            return E_FAIL;*/

        
        pModelCom->Render(i, m_pShaderCom,0);
    }

//#ifdef _DEBUG
//      m_pOBBCom->Render();
//#endif // _DEBUG

      m_pTempNoteModel->Set_Enable(false);

    return S_OK;
}


_bool CNote::Install(_float3 vPosition, COLLISION_TYPE eType, _float4 vLook, CItem* pConnectItem )
{
    if (eType == COLLISION_TYPE::OBJECT|| eType == COLLISION_TYPE::NAVIGATION)
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

        m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

        m_pNoteOpenModel->Set_CurrentAnimation(1);

        return true;
    }
    return false;
}

void CNote::Set_TempModel_Pos(_float3 vPosition, COLLISION_TYPE eType, _float4 vLook, CItem* pConnectItem)
{
    if (m_pTempNoteModel)
    {
        m_pTempNoteModel->Set_Enable(true);
        m_pTempNoteModel->Set_TempModel_Pos(vPosition, eType, vLook, pConnectItem);
    }
}

void CNote::Ghost_Writing()
{
    /**/
   /*
   * if( m_bFirst)
   * 그리는 모션
   */
}


void CNote::On_Collision_Enter(CCollider* pCollider)
{
    if (COLLISION_TYPE::GHOST == pCollider->Get_Type())
    {
        if (m_bInstalled)
        {
            CGhost* pGhost = (CGhost*)pCollider->Get_Owner();
            if(pGhost->Check_GhostWriting())
                Ghost_Writing();
        }
    }

}


void CNote::On_Collision_Stay(CCollider* pCollider)
{
}

void CNote::On_Collision_Exit(CCollider* pCollider)
{
}

HRESULT CNote::Setup_Component()
{
    if (FAILED(__super::Setup_Component()))
        return E_FAIL;


    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Note"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;

    /* For.Com_ModelOpen */
  /*  if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Note_Open"), TEXT("Com_ModelOpen"), (CComponent**)&m_pNoteOpenModel)))
        return E_FAIL;*/

    /* For.Com_OBB*/
    CCollider::COLLIDERDESC			ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

    ColliderDesc.vScale = _float3(1.4f, 2.2f, 0.2f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, 0.f, -0.1f);
    ColliderDesc.pOwner = this;
    ColliderDesc.m_eObjID = COLLISION_TYPE::ITEM;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
        return E_FAIL;

    return S_OK;
}


HRESULT CNote::Setup_TempModel()
{
    /*For.TempModel*/
    if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_TempModel"), TEXT("Prototype_GameObject_TempNote"), (CGameObject**)&m_pTempNoteModel)))
        return E_FAIL;

    m_pTempNoteModel->Set_Enable(false);

    return S_OK;
}

CNote* CNote::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CNote* pInstance = new CNote(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CNote");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CNote::Clone(void* pArg)
{
    CNote* pInstance = new CNote(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CNote");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CNote::Free()
{
    __super::Free();

    Safe_Release(m_pNoteOpenModel);
}
