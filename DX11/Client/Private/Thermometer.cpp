#include "stdafx.h"
#include "../Public/Thermometer.h"
#include "Ghost_SpawnPoint.h"
#include "Atmosphere.h"
#include "GameInstance.h"
#include "RenderTarget.h"


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
    if (FAILED(__super::Initialize(nullptr)))
        return E_FAIL;

    if (nullptr != pArg)
    {
        m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4((_float4x4*)pArg));
    }

    if (FAILED(Setup_Component()))
        return E_FAIL;

    GAMEINSTANCE->Add_EventObject(CGame_Manager::EVENT_ITEM, this);
    m_pEventFunc = std::bind(&CItem::Normal_Operation, std::placeholders::_1, std::placeholders::_2);


    return S_OK;
}

void CThermometer::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
    m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
    m_pRigidBodyCom->Update(fTimeDelta, m_pCurrNavigation);

    if(m_bSwitch)
        m_fTimeAcc += fTimeDelta;

}

void CThermometer::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    GAMEINSTANCE->Add_Object_For_Culling(this, CRenderer::RENDER_NONALPHABLEND);

    if (m_bSwitch)
    {
        CTexture* pTexture = m_pModelCom->Get_SRV(0, aiTextureType_DIFFUSE);
        CRenderer::RENDERFONT RenderFont;
        if (nullptr != pTexture)
        {
            
            RenderFont.pString = m_szDegree;
            RenderFont.vPosition = XMVectorSet(625, 150, 0.f, 0.f);
            RenderFont.vColor = XMVectorSet(0.f, 0.f, 0.f, 1.f);
            RenderFont.rotation = 0.f;
            RenderFont.vOrigin = XMVectorSet(0.f, 0.f, 0.f, 0.f);
            RenderFont.vScale = XMVectorSet(1.f, 1.f, 1.f, 0.f);//시작 점을 오른쪽 아래로 넘기기

            m_pRendererCom->Draw_On_Texture(m_pDiffuse, pTexture, m_pShaderTexCom, 0, RenderFont, TEXT("Font_Dream"));

        }
        pTexture = m_pModelCom->Get_SRV(0, aiTextureType_EMISSIVE);
        if (nullptr != pTexture)
        {
            m_pRendererCom->Draw_On_Texture(m_pEmissive, pTexture, m_pShaderTexCom, 0,RenderFont, TEXT("Font_Dream"));
        }
    }

#ifdef _DEBUG
  //  m_pRendererCom->Add_DebugRenderGroup(m_pOBBCom);
#endif
}

HRESULT CThermometer::Render()
{


    _uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        _int iPassIndex = 2;

   
        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
            return E_FAIL;

        if (m_bSwitch)
        {
            if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pDiffuse->Get_SRV())))
                return E_FAIL;

            if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_EmissiveTexture", m_pEmissive->Get_SRV())))
                return E_FAIL;
            iPassIndex = 3;
        }
        else
        {
            if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
                return E_FAIL;
            if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_EmissiveTexture", i, aiTextureType_EMISSIVE)))
                return E_FAIL;
        }

        m_pModelCom->Render(i, m_pShaderCom, iPassIndex);
    }

    // MakeSpriteFont "폰트이름" /FontSize:32 /FastPack /CharacterRegion:0x0020-0x00FF /CharacterRegion:0x3131-0x3163 /CharacterRegion:0xAC00-0xD800 /DefaultCharacter:0xAC00 출력파일이름.spritefont


    //if (m_bSwitch)
    //{
    //    //const _tchar* pString, _fvector vPosition, _fvector vColor, float rotation, _fvector vOrigin, _vector vScale
    //    GAMEINSTANCE->Render_Font(TEXT("Font_Dream"), m_szDegree, XMVectorSet(200.f, 200.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), XMConvertToRadians(90.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 0.f));
    //    /*
    //    * 스프라이트 폰트를 그릴 때는 현재 세팅 되어있는 1번 렌더타겟에 자동을 그림? 다른 렌더타겟에 그리게 할 수 있음?
    //    * 스프라이트 폰트는 셰이더를 통해서 효과를 줄 수 있음?
    //    */
    //}
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

        wsprintf(m_szDegree, TEXT("%2d"), dis(gen));
        m_fTimeAcc = 0.f;

    }
}

void CThermometer::Normal_Operation(_float fTimeDelta)
{
    if (m_fTimeAcc >= 1.5f)
    {
        wsprintf(m_szDegree, TEXT("%2d"), m_iDegree);
        m_fTimeAcc = 0.f;

    }
}

void CThermometer::Drop_Item(_vector vPower)
{
    _vector vLook = XMVectorSet(0.f, -1.f, 0.f, 0.f);
    _vector vUp = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
    _vector vRight = XMVector3Cross(vLook, vUp);
    vUp = XMVector3Cross(vLook, vRight);

    m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
    m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);
    m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);

    Add_Power(vPower);
}

void CThermometer::On_Collision_Enter(CCollider* pCollider)
{
    if (COLLISION_TYPE::HOUSE == pCollider->Get_Type())
    {
        m_pCurrNavigation = m_pNaviHouseCom;
    }
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
    if (COLLISION_TYPE::HOUSE == pCollider->Get_Type())
    {
        m_pCurrNavigation = m_pNaviOutSideCom;
    }
}

HRESULT CThermometer::Setup_Component()
{

    if (FAILED(__super::Setup_Component()))
        return E_FAIL;

    /* For.Com_Navigation*/
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
    CNavigation::NAVIDESC	NaviDesc;
    ZeroMemory(&NaviDesc, sizeof(CNavigation::NAVIDESC));
    NaviDesc.m_iCurrentIndex = 0;

    if (FAILED(__super::Add_Component(pGameInstance->Get_Next_Level(), TEXT("Prototype_Component_Navigation_House"), TEXT("Com_NaviHouse"), (CComponent**)&m_pNaviHouseCom, &NaviDesc)))
        return E_FAIL;

    ZeroMemory(&NaviDesc, sizeof(CNavigation::NAVIDESC));
    NaviDesc.m_iCurrentIndex = 0;

    if (FAILED(__super::Add_Component(pGameInstance->Get_Next_Level(), TEXT("Prototype_Component_Navigation_OutSide"), TEXT("Com_NaviOutSide"), (CComponent**)&m_pNaviOutSideCom, &NaviDesc)))
        return E_FAIL;

    RELEASE_INSTANCE(CGameInstance);
    m_pCurrNavigation = m_pNaviOutSideCom;

    /*For.Com_RigidBody*/
    CRigidBody::RIGIDBODYDESC RigidBodyDesc;
    ZeroMemory(&RigidBodyDesc, sizeof(CRigidBody::RIGIDBODYDESC));
    RigidBodyDesc.fWeight = 1.f;
    RigidBodyDesc.pOwnerTransform = m_pTransformCom;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBodyCom, &RigidBodyDesc)))
        return E_FAIL;

    /* For.Com_TexShader*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_TexShader"), (CComponent**)&m_pShaderTexCom)))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Thermometer"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;

    /* For.Com_OBB*/
    CCollider::COLLIDERDESC			ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

    ColliderDesc.vScale = _float3(0.45f, 1.4f, 0.7f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);
    ColliderDesc.pOwner = this;
    ColliderDesc.m_eObjID = COLLISION_TYPE::THERMOMETER;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
        return E_FAIL;

    /*For.RenderTarget*/
    m_pDiffuse= CRenderTarget::Create(m_pDevice, m_pContext, 1280, 720, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f), true);
    if (nullptr == m_pDiffuse)
        return E_FAIL;

    m_pEmissive = CRenderTarget::Create(m_pDevice, m_pContext, 1280, 720, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f), true);
    if (nullptr == m_pEmissive)
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

    Safe_Release(m_pDiffuse);
    Safe_Release(m_pEmissive);
    Safe_Release(m_pShaderTexCom);

    Safe_Release(m_pNaviHouseCom);
    Safe_Release(m_pNaviOutSideCom);
    Safe_Release(m_pRigidBodyCom);
}
