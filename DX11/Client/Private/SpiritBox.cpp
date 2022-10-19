#include "stdafx.h"
#include "../Public/SpiritBox.h"
#include "GameInstance.h"
#include "Ghost_SpawnPoint.h"
#include "RenderTarget.h"

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

    m_fAnswerTimeLasting = dis(gen) % 5 + 30;

    GAMEINSTANCE->Add_EventObject(CGame_Manager::EVENT_ITEM, this);
    m_pEventFunc = std::bind(&CItem::Normal_Operation, std::placeholders::_1, std::placeholders::_2);


    return S_OK;
}

void CSpiritBox::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
    m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

    if (m_bSwitch)
    {
        m_fTimeAcc += fTimeDelta;
    
#ifdef _DEBUG
        m_fAnswerTime -= fTimeDelta;
#endif
    }
}

void CSpiritBox::LateTick(_float fTimeDelta)
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
            RenderFont.vPosition = XMVectorSet(650.f, 560.f, 0.f, 0.f);
            RenderFont.vColor = XMVectorSet(0.f, 0.f, 0.f, 1.f);
            RenderFont.rotation = 90.f;
            RenderFont.vOrigin = XMVectorSet(0.f, 0.f, 0.f, 0.f);
            RenderFont.vScale = XMVectorSet(1.f, 1.f, 1.f, 0.f);

            m_pRendererCom->Draw_On_Texture(m_pDiffuse, pTexture, m_pShaderTexCom, 0, RenderFont, TEXT("Font_Dream"));

        }
        pTexture = m_pModelCom->Get_SRV(0, aiTextureType_EMISSIVE);
        if (nullptr != pTexture)
        {
            m_pRendererCom->Draw_On_Texture(m_pEmissive, pTexture, m_pShaderTexCom, 0, RenderFont, TEXT("Font_Dream"));
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


#ifdef _DEBUG
    if (m_fAnswerTime > 0.f)
    {
        GAMEINSTANCE->Render_Font(TEXT("Font_Dream"), m_szAnswer, _float2(1100.f, 100.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));

    }
#endif
//#ifdef _DEBUG
//      m_pOBBCom->Render();
//#endif // _DEBUG


    return S_OK;
}

void CSpiritBox::OnEventMessage(const _tchar* pMessage)
{
    __super::OnEventMessage(pMessage);

    if (0 == lstrcmp(TEXT("Answer"), pMessage))
    {
       /*Ghost Icon*/
#ifdef _DEBUG
        wsprintf(m_szAnswer, TEXT("응답"));
        m_fAnswerTime = 3.f;
#endif
    }
    else if (0 ==lstrcmp(TEXT("Not_Respone"), pMessage))
    {
        /*X Icon*/
#ifdef _DEBUG
        wsprintf(m_szAnswer, TEXT("무응답"));
        m_fAnswerTime = 3.f;
#endif
    }
}


void CSpiritBox::MalFunction(_float fTimeDelta)
{
    /*랜덤한 값을 넣어줌*/
    if (0.1f <= m_fTimeAcc)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(10, 90);

        wsprintf(m_szDegree, TEXT("%03d"), dis(gen));
        m_fTimeAcc = 0.f;
    }
}

void CSpiritBox::Normal_Operation(_float fTimeDelta)
{
    if (m_bInGhostArea)
        m_fAnswerTimeLasting -= fTimeDelta;

    wsprintf(m_szDegree, TEXT("%03d"), m_lFrequency);
}

void CSpiritBox::Frequency_Control(_long lMouseMove)
{
    m_lFrequency += lMouseMove;
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
    }
}

void CSpiritBox::On_Collision_Stay(CCollider* pCollider)
{
    if (m_bSwitch)
    {
        if (COLLISION_TYPE::GHOST_AREA == pCollider->Get_Type())
        {
           CGhost_SpawnPoint* pGhost = (CGhost_SpawnPoint*)pCollider->Get_Owner();
           pGhost->Get_Answer(m_lFrequency, m_fAnswerTimeLasting);
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

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_SpiritBox"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;

    /* For.Com_TexShader*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_TexShader"), (CComponent**)&m_pShaderTexCom)))
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
}
