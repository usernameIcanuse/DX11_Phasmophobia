#include "stdafx.h"
#include "../Public/SpiritBox.h"
#include "GameInstance.h"
#include "Ghost_SpawnPoint.h"

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
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Setup_Component()))
        return E_FAIL;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(10, 90);

    m_fAnswerTimeLasting = dis(gen) % 5 + 30;

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

#ifdef _DEBUG
    m_pRendererCom->Add_DebugRenderGroup(m_pOBBCom);
#endif
}

HRESULT CSpiritBox::Render()
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

    if (m_bSwitch)
    {
        GAMEINSTANCE->Render_Font(TEXT("Font_Dream"), m_szDegree, _float2(0.f, 100.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
    }

    if (m_fAnswerTime > 0.f)
    {
        GAMEINSTANCE->Render_Font(TEXT("Font_Dream"), m_szAnswer, _float2(1100.f, 100.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));

    }
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

        wsprintf(m_szDegree, TEXT("Frequency : %03d"), dis(gen));
        m_fTimeAcc = 0.f;
    }
}

void CSpiritBox::Normal_Operation(_float fTimeDelta)
{
    if (m_bInGhostArea)
        m_fAnswerTimeLasting -= fTimeDelta;

    wsprintf(m_szDegree, TEXT("Frequency : %03d"), m_lFrequency);
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
}

HRESULT CSpiritBox::Setup_Component()
{
    if (FAILED(__super::Setup_Component()))
        return E_FAIL;




    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Model_SpiritBox"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;

    /* For.Com_OBB*/
    CCollider::COLLIDERDESC			ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

    ColliderDesc.vScale = _float3(0.75f, 1.4f, 0.25f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);
    ColliderDesc.pOwner = this;
    ColliderDesc.m_eObjID = COLLISION_TYPE::SPIRITBOX;

    if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
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
}
