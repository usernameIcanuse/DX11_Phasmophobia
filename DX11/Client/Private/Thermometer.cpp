#include "stdafx.h"
#include "../Public/Thermometer.h"
#include "Ghost_SpawnPoint.h"
#include "Atmosphere.h"
#include "GameInstance.h"


CThermometer::CThermometer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CItem(pDevice,pContext)
{
}

CThermometer::CThermometer(const CThermometer& rhs)
    :CItem(rhs)
{
}

HRESULT CThermometer::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CThermometer::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Setup_Component()))
        return E_FAIL;

    return S_OK;
}

void CThermometer::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
    m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

    if(m_bSwitch)
        m_fTimeAcc += fTimeDelta;

}

void CThermometer::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    GAMEINSTANCE->Add_Object_For_Culling(this, CRenderer::RENDER_NONALPHABLEND);

#ifdef _DEBUG
    m_pRendererCom->Add_DebugRenderGroup(m_pOBBCom);
#endif
}

HRESULT CThermometer::Render()
{


    _uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
            return E_FAIL;
        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
            return E_FAIL;


        m_pModelCom->Render(i, m_pShaderCom,2);
    }

    // MakeSpriteFont "폰트이름" /FontSize:32 /FastPack /CharacterRegion:0x0020-0x00FF /CharacterRegion:0x3131-0x3163 /CharacterRegion:0xAC00-0xD800 /DefaultCharacter:0xAC00 출력파일이름.spritefont


    if (m_bSwitch)
    {
        GAMEINSTANCE->Render_Font(TEXT("Font_Dream"), m_szDegree, _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
    }
//#ifdef _DEBUG
//      m_pOBBCom->Render();
//#endif // _DEBUG


    return S_OK;
}


void CThermometer::MalFunction(_float fTimeDelta)
{
    if (m_fTimeAcc >= 0.1f)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(10, 99);

        wsprintf(m_szDegree, TEXT("방온도 : %d"), dis(gen));
        m_fTimeAcc = 0.f;

    }
}

void CThermometer::Normal_Operation(_float fTimeDelta)
{
    if (m_fTimeAcc >= 1.5f)
    {
        wsprintf(m_szDegree, TEXT("방온도 : %d"), m_iDegree);
        m_fTimeAcc = 0.f;

    }
}

void CThermometer::On_Collision_Enter(CCollider* pCollider)
{
    __super::On_Collision_Enter(pCollider);
}

void CThermometer::On_Collision_Stay(CCollider* pCollider)
{
    if (COLLISION_TYPE::ATMOSPHERE == pCollider->Get_Type())
    {
       
        m_iDegree = static_cast<CAtmosphere*>(pCollider->Get_Owner())->Get_Temperature();
        
    }
    else if (COLLISION_TYPE::GHOST_AREA == pCollider->Get_Type())
    {
       
        m_iDegree = static_cast<CGhost_SpawnPoint*>(pCollider->Get_Owner())->Get_AreaTemperature();
         
    }
    else if (COLLISION_TYPE::GHOST_SPAWNPOINT == pCollider->Get_Type())
    {
        
        m_iDegree = static_cast<CGhost_SpawnPoint*>(pCollider->Get_Owner())->Get_SpawnPointTemperature();
       
    }
}

void CThermometer::On_Collision_Exit(CCollider* pCollider)
{
}

HRESULT CThermometer::Setup_Component()
{

    if (FAILED(__super::Setup_Component()))
        return E_FAIL;


    
    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Thermometer"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;

    /* For.Com_OBB*/
    CCollider::COLLIDERDESC			ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

    ColliderDesc.vScale = _float3(0.45f, 1.4f, 0.7f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);
    ColliderDesc.pOwner = this;
    ColliderDesc.m_eObjID = COLLISION_TYPE::THERMOMETER;

    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
        return E_FAIL;

    return S_OK;
}


CThermometer* CThermometer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CThermometer* pInstance = new CThermometer(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CThermometer");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CThermometer::Clone(void* pArg)
{
    CThermometer* pInstance = new CThermometer(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CThermometer");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CThermometer::Free()
{
    __super::Free();
}
