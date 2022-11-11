#include "stdafx.h"
#include "../Public/SpiritBox.h"
#include "GameInstance.h"
#include "Ghost_SpawnPoint.h"
#include "RenderTarget.h"
#include "SoundMgr.h"

CSpiritBox::CSpiritBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CItem(pDevice, pContext)
{
}

CSpiritBox::CSpiritBox(const CSpiritBox& rhs)
    :CItem(rhs)
{
}

HRESULT CSpiritBox::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSpiritBox::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(nullptr)))
        return E_FAIL;

    if (nullptr != pArg)
    {
        m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4((_float4x4*)pArg));
    }

    if (FAILED(Setup_Component()))
        return E_FAIL;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(10, 90);

    m_fAnswerTimeLasting = dis(gen) % 5 + 5;

    GAMEINSTANCE->Add_EventObject(CGame_Manager::EVENT_ITEM, this);
    m_pEventFunc = std::bind(&CItem::Normal_Operation, std::placeholders::_1, std::placeholders::_2);


    return S_OK;
}

void CSpiritBox::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
    m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
    m_pRigidBodyCom->Update(fTimeDelta, m_pCurrNavigation);
    if (m_bSwitch)
    {
        m_fTimeAcc += fTimeDelta;
        
        CSoundMgr::Get_Instance()->PlaySoundDistance(TEXT("evp white noise loop.wav"), CSoundMgr::ITEM_SPIRITBOX, this,0.6f);

        m_fAnswerTime -= fTimeDelta;
        if (0.f > m_fAnswerTime)
        {
            if (false == m_bGhostIconAlpha)
                m_bGhostIconAlpha = true;
            else if (false == m_bXIconAlpha)
                m_bXIconAlpha = true;
        }

    }
}

void CSpiritBox::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
    
    GAMEINSTANCE->Add_Object_For_Culling(this, CRenderer::RENDER_NONALPHABLEND);

    CTexture* pTexture = m_pModelCom->Get_SRV(0, aiTextureType_DIFFUSE);
    CRenderer::RENDERFONT RenderFont;
    if (m_bSwitch)
    {
        RenderFont.pString = m_szDegree;
        RenderFont.vPosition = XMVectorSet(643.f, 515.f, 0.f, 0.f);
        RenderFont.vColor = XMVectorSet(0.f, 0.f, 0.f, 0.7f);
        RenderFont.rotation = 90.f;
        RenderFont.vOrigin = XMVectorSet(0.f, 0.f, 0.f, 0.f);
        RenderFont.vScale = XMVectorSet(0.9f, 1.7f, 1.f, 0.f);
    }
    else
    {
        RenderFont.pString = TEXT("");
        RenderFont.vPosition = XMVectorSet(643.f, 515.f, 0.f, 0.f);
        RenderFont.vColor = XMVectorSet(0.f, 0.f, 0.f, 1.f);
        RenderFont.rotation = 90.f;
        RenderFont.vOrigin = XMVectorSet(0.f, 0.f, 0.f, 0.f);
        RenderFont.vScale = XMVectorSet(0.9f, 1.7f, 1.f, 0.f);
    }
    
 
    m_pDiffuse->Clear();
    ID3D11DepthStencilView* DSV = m_pDiffuse->GeT_DepthStencilView();
    m_pContext->ClearDepthStencilView(DSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

    m_pRendererCom->Draw_On_Texture(m_pDiffuse, pTexture, m_pShaderTexCom, 0, RenderFont, TEXT("Font_Digital"));

    m_pShaderTexCom->Set_RawValue("bAlpha", &m_bXIconAlpha, sizeof(_bool));
    m_pRendererCom->Draw_On_Texture(m_pDiffuse, m_pXIcon, m_pShaderTexCom, 5, _float3(600.f, 640.f, 0.f), _float3(35.f, 20.f, 1.f));

    m_pShaderTexCom->Set_RawValue("bAlpha", &m_bGhostIconAlpha, sizeof(_bool));
    m_pRendererCom->Draw_On_Texture(m_pDiffuse, m_pGhostIcon, m_pShaderTexCom, 5, _float3(600.f, 670.f, 0.f), _float3(45.f, 25.f, 1.f));


    if (m_bSwitch)
    {
        m_pRendererCom->Draw_On_Texture(m_pDiffuse, pTexture, m_pShaderTexCom, 0, RenderFont, TEXT("Font_Digital"));

        pTexture = m_pModelCom->Get_SRV(0, aiTextureType_EMISSIVE);
        if (nullptr != pTexture)
        {
            m_pRendererCom->Draw_On_Texture(m_pEmissive, pTexture, m_pShaderTexCom, 0, RenderFont, TEXT("Font_Digital"));

            m_pShaderTexCom->Set_RawValue("bAlpha", &m_bXIconAlpha, sizeof(_bool));
            m_pRendererCom->Draw_On_Texture(m_pEmissive, m_pXIcon, m_pShaderTexCom, 5, _float3(600.f, 640.f, 0.f), _float3(35.f, 20.f, 1.f));

            m_pShaderTexCom->Set_RawValue("bAlpha", &m_bGhostIconAlpha, sizeof(_bool));
            m_pRendererCom->Draw_On_Texture(m_pEmissive, m_pGhostIcon, m_pShaderTexCom, 5, _float3(600.f, 670.f, 0.f), _float3(45.f, 25.f, 1.f));
        }
    }

#ifdef _DEBUG
   // m_pRendererCom->Add_DebugRenderGroup(m_pOBBCom);
#endif
}

HRESULT CSpiritBox::Render()
{

    _uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        _int iPassIndex = 2;

        //if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
        //    return E_FAIL;
         if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
              return E_FAIL;

         if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pDiffuse->Get_SRV())))
             return E_FAIL;

         if (m_bSwitch)
         {
             if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_EmissiveTexture", m_pEmissive->Get_SRV())))
                 return E_FAIL;
             iPassIndex = 3;
         }
    
        
        m_pModelCom->Render(i, m_pShaderCom, iPassIndex);
    }


#ifdef _DEBUG
    if (m_fAnswerTime > 0.f)
    {
       // GAMEINSTANCE->Render_Font(TEXT("Font_Dream"), m_szAnswer, _float2(1100.f, 100.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));

    }
#endif
//#ifdef _DEBUG
//      m_pOBBCom->Render();
//#endif // _DEBUG


    return S_OK;
}

void CSpiritBox::Turn_Switch()
{
    m_bSwitch = !m_bSwitch;
    if (false == m_bSwitch)
        CSoundMgr::Get_Instance()->StopSound(CSoundMgr::ITEM_SPIRITBOX);

}

void CSpiritBox::OnEventMessage(const _tchar* pMessage)
{
    __super::OnEventMessage(pMessage);

    if (0 == lstrcmp(TEXT("Answer"), pMessage))
    {
       /*Ghost Icon*/
#ifdef _DEBUG
        wsprintf(m_szAnswer, TEXT("응답"));
#endif
        m_fAnswerTime = 3.f;
        m_bGhostIconAlpha = false;
    }
    else if (0 ==lstrcmp(TEXT("Not_Respone"), pMessage))
    {
        /*X Icon*/
#ifdef _DEBUG
        wsprintf(m_szAnswer, TEXT("무응답"));
#endif
        m_fAnswerTime = 3.f;
        m_bXIconAlpha = false;
    }
}


void CSpiritBox::MalFunction(_float fTimeDelta)
{
    if (false == m_bIsInHouse)
        return;
    CSoundMgr::Get_Instance()->StopSound(CSoundMgr::GHOST_WHISPERING);
    /*랜덤한 값을 넣어줌*/
    if (0.1f <= m_fTimeAcc)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, 99999);

        _int iValue = dis(gen);

        wsprintf(m_szDegree, TEXT("%03d.%02d"), iValue/100, iValue%100);
        m_fTimeAcc = 0.f;
    }
}

void CSpiritBox::Normal_Operation(_float fTimeDelta)
{
    if (m_bInGhostArea)
        m_fAnswerTimeLasting -= fTimeDelta;

    m_fGhostAnswer -= fTimeDelta;
    if (0.f > m_fGhostAnswer)
        CSoundMgr::Get_Instance()->StopSound(CSoundMgr::GHOST_WHISPERING);

    wsprintf(m_szDegree, TEXT("%03d.%02d"), m_lFrequency/100, m_lFrequency % 100);
}

void CSpiritBox::Frequency_Control(_long lMouseMove)
{
    m_lFrequency += lMouseMove*50;
}

void CSpiritBox::Drop_Item(_vector vPower)
{
    _vector vLook = XMVectorSet(0.f, -1.f, 0.f, 0.f);
    _vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
    _vector vUp = XMVector3Cross(vLook, vRight);
    // vUp = XMVector3Cross(vLook, vRight);

    m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
    m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);
    m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);

//    m_pTransformCom->Rotation(vLook, XMConvertToRadians(180.f));

    Add_Power(vPower);
}

void CSpiritBox::On_Collision_Enter(CCollider* pCollider)
{
    if (m_bSwitch)
    {
        if (COLLISION_TYPE::GHOST_AREA == pCollider->Get_Type())
        {
            m_bInGhostArea = true;
        }
    }

    if (COLLISION_TYPE::HOUSE == pCollider->Get_Type())
    {
        m_pCurrNavigation = m_pNaviHouseCom;
        m_bIsInHouse = true;
    }
}

void CSpiritBox::On_Collision_Stay(CCollider* pCollider)
{
    if (m_bSwitch)
    {
        if (COLLISION_TYPE::GHOST_AREA == pCollider->Get_Type())
        {
           CGhost_SpawnPoint* pGhost = (CGhost_SpawnPoint*)pCollider->Get_Owner();

           if (pGhost->Get_Answer(m_lFrequency, m_fAnswerTimeLasting))
           {
               std::random_device rd;
               std::mt19937 gen(rd());
               std::uniform_int_distribution<int> dis(1, 100);

               _int iValue = dis(gen);

               if (50 > iValue)
               {
                   if(0 == iValue % 4)
                     CSoundMgr::Get_Instance()->PlaySound(TEXT("death.wav"), CSoundMgr::GHOST_DEATH, 0.7f);
                   else if( 1 == iValue%4)
                     CSoundMgr::Get_Instance()->PlaySound(TEXT("close.wav"), CSoundMgr::GHOST_CLOSE, 0.7f);
                   else if (2 == iValue % 4)
                       CSoundMgr::Get_Instance()->PlaySound(TEXT("die.wav"), CSoundMgr::GHOST_DIE, 0.7f);
                   else if (3 == iValue % 4)
                       CSoundMgr::Get_Instance()->PlaySound(TEXT("die male.wav"), CSoundMgr::GHOST_DIEMALE, 0.7f);

               }
   /*            else
               {
                   std::random_device rd;
                   std::mt19937 gen(rd());
                   std::uniform_int_distribution<int> dis(1, 100);

                   _int iValue = dis(gen)%5;

                   m_fGhostAnswer = iValue + 1;

                   if (1 == iValue % 2)
                   {
                       CSoundMgr::Get_Instance()->PlaySound(TEXT("whispering voices.wav"), CSoundMgr::GHOST_WHISPERING, 0.5f);
                   }
                   else
                       CSoundMgr::Get_Instance()->PlaySound(TEXT("female whispering.wav"), CSoundMgr::GHOST_WHISPERING, 0.5f);
               }*/


           }
        }
    }
        
}

void CSpiritBox::On_Collision_Exit(CCollider* pCollider)
{
    if (m_bSwitch)
    {
        if (COLLISION_TYPE::GHOST_AREA == pCollider->Get_Type())
        {
            m_bInGhostArea = false;
        }
    }
    if (COLLISION_TYPE::HOUSE == pCollider->Get_Type())
    {
        m_pCurrNavigation = m_pNaviOutSideCom;
        m_bIsInHouse = false;
    }
}

HRESULT CSpiritBox::Setup_Component()
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

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_SpiritBox"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;

    /* For.Com_TexShader*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_TexShader"), (CComponent**)&m_pShaderTexCom)))
        return E_FAIL;

    /*For.Com_XIcon*/
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_XIcon"), TEXT("Com_XIcon"), (CComponent**)&m_pXIcon)))
        return E_FAIL;

    /*For.Com_GhostIcon*/
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_GhostIcon"), TEXT("Com_GhostIcon"), (CComponent**)&m_pGhostIcon)))
        return E_FAIL;

    /* For.Com_OBB*/
    CCollider::COLLIDERDESC			ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

    ColliderDesc.vScale = _float3(0.75f, 1.4f, 0.25f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);
    ColliderDesc.pOwner = this;
    ColliderDesc.m_eObjID = COLLISION_TYPE::SPIRITBOX;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
        return E_FAIL;

    /*For.RenderTarget*/
    m_pDiffuse= CRenderTarget::Create(m_pDevice, m_pContext, 1280, 720, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f),true);
    if (nullptr == m_pDiffuse)
        return E_FAIL;
    m_pEmissive = CRenderTarget::Create(m_pDevice, m_pContext, 1280, 720, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f), true);
    if (nullptr == m_pEmissive)
        return E_FAIL;
    return S_OK;
}


CSpiritBox* CSpiritBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSpiritBox* pInstance = new CSpiritBox(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CSpiritBox");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSpiritBox::Clone(void* pArg)
{
    CSpiritBox* pInstance = new CSpiritBox(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CSpiritBox");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSpiritBox::Free()
{
    __super::Free();
    Safe_Release(m_pDiffuse);
    Safe_Release(m_pEmissive);
    Safe_Release(m_pShaderTexCom);

    Safe_Release(m_pNaviHouseCom);
    Safe_Release(m_pNaviOutSideCom);
    Safe_Release(m_pRigidBodyCom);

    Safe_Release(m_pXIcon);
    Safe_Release(m_pGhostIcon);
}
