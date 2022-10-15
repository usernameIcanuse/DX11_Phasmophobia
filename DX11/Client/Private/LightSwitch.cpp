#include "stdafx.h"
#include "../Public/LightSwitch.h"
#include "GameInstance.h"
#include "LightBulb.h"

CLightSwitch::CLightSwitch(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice,pContext)
{
}

CLightSwitch::CLightSwitch(const CLightSwitch& rhs)
    :CGameObject(rhs)
{
}

HRESULT CLightSwitch::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CLightSwitch::Initialize(void* pArg)//자신의 위치와 전구의 위치를 같이 보냄
{
    if (FAILED(__super::Initialize(nullptr)))
        return E_FAIL;

    if (nullptr != pArg)
    {
        SWITCH_DATA tData = *(SWITCH_DATA*)pArg;
        
        m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&tData.matSwitchWorld));
        if (FAILED(Setup_LightBulb(tData)))
            return E_FAIL;
    }

    if (FAILED(Setup_Component()))
        return E_FAIL;

    return S_OK;
}

void CLightSwitch::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
 
    m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CLightSwitch::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
 
    GAMEINSTANCE->Add_Object_For_Culling(this, CRenderer::RENDER_NONALPHABLEND);
    //m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

}

HRESULT CLightSwitch::Render()
{

    _uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        _int       iPassIndex = 2;

        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
            return E_FAIL;
        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
        {
            iPassIndex = 0;
        }

        m_pModelCom->Render(i, m_pShaderCom, iPassIndex);
    }


    return S_OK;
}

HRESULT CLightSwitch::SetUp_ModelCom(const _tchar* pPrototypeTag)
{
   

    return S_OK;
}


HRESULT CLightSwitch::Setup_Component()
{
    /* For.Com_Shader*/
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;


    /* For.Com_OBB*/
    CCollider::COLLIDERDESC  ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

    ColliderDesc.vScale = _float3(0.5f, 0.5f, 0.3f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, 0.f, ColliderDesc.vScale.z * 0.5f);
    ColliderDesc.pOwner = this;
    ColliderDesc.m_eObjID = COLLISION_TYPE::LIGHTSWITCH;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_LightSwitch"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;
   
    return S_OK;
}

HRESULT		CLightSwitch::Setup_LightBulb(SWITCH_DATA _tData)
{
    CLightBulb* pTemp;
    _int    iNum = _tData.iBulbNum;
    for (_int i = 0; i < iNum; ++i)
    {
        _float4x4 BulbWorldMat = _tData.matBulbWorld[i];
        if (GAMEINSTANCE->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Light"), TEXT("Prototype_GameObject_LightBulb"), (CGameObject**)&pTemp, &BulbWorldMat))
            return E_FAIL;

        m_LightBulbList.push_back(pTemp);
    }
    return S_OK;

}

void CLightSwitch::OnEventMessage(const _tchar* pMessage)
{
    /* turn off 기능만 있으면 전체 스위치를 끔*/
}

HRESULT CLightSwitch::SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix)
{
    if (nullptr == m_pShaderCom ||
        nullptr == m_pModelCom)
        return E_FAIL;

    if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", pViewMatrix, sizeof(_float4x4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", pProjMatrix, sizeof(_float4x4))))
        return E_FAIL;


    return S_OK;
}

void CLightSwitch::Turn_Switch()
{
    m_bSwitch = !m_bSwitch;
    for (auto& elem : m_LightBulbList)
    {
        elem->Turn_Switch(m_bSwitch);
   }
}

void CLightSwitch::On_Collision_Enter(CCollider* pCollider)
{
}

void CLightSwitch::On_Collision_Stay(CCollider* pCollider)
{
}

void CLightSwitch::On_Collision_Exit(CCollider* pCollider)
{
}

CLightSwitch* CLightSwitch::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CLightSwitch* pInstance = new CLightSwitch(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CLightSwitch");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CLightSwitch::Clone(void* pArg)
{
    CLightSwitch* pInstance = new CLightSwitch(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CLightSwitch");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLightSwitch::Free()
{
    __super::Free();
    
    Safe_Release(m_pShaderCom);  
    Safe_Release(m_pModelCom);
    Safe_Release(m_pOBBCom);
   
    //해당 클래스에 있는 변수들은 항상 safe_release해주기
}
