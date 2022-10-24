#include "stdafx.h"
#include "../Public/MenuScreen.h"
#include "GameInstance.h"
#include "Lobby_Main.h"
#include "Lobby_WaitingRoom.h"
#include "Lobby_Store.h"
#include "Lobby_AddItems.h"
#include "Level_Loading.h"
#include "Camera_Fixed.h"
#include "UIBackGround.h"

CMenuScreen::CMenuScreen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice,pContext)
{
}

CMenuScreen::CMenuScreen(const CMenuScreen& rhs)
    :CGameObject(rhs)
{
}

HRESULT CMenuScreen::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMenuScreen::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(nullptr)))
        return E_FAIL;

    if (nullptr != pArg)
    {
        SCREENDESC tDesc = *(SCREENDESC*)pArg;

        m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&tDesc.WorldMat));


        if (FAILED(Setup_Screen()))
            return E_FAIL;

        if (FAILED(Setup_Camera()))
            return E_FAIL;

        if (true == tDesc.bGamePlay)
        {
            m_bLock = false;
            m_pCurUI->Icon_Lock(m_bLock);
        }
    }

    if (FAILED(Setup_Component()))
        return E_FAIL;



    return S_OK;
}

void CMenuScreen::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
    m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

    if(false == m_bLock)
    {    
        if (GAMEINSTANCE->Is_KeyState(KEY::ESC, KEY_STATE::TAP))
        {
            if (FAILED(GAMEINSTANCE->Change_Camera(TEXT("Camera_Player"))))
                return;
            m_bLock = true;
            m_pCurUI->Icon_Lock(m_bLock);
        }
        //이게 맞냐
#pragma region Lobby_Main
	if (m_pMain->Get_Enable())//로비
	{
		_uint iSelectedMenu = static_cast<CUIBackground*>(m_pMain)->Selected_Menu();

		switch (iSelectedMenu)
		{
		case 1:
			SetWindowText(g_hWnd, TEXT("Level_Lobby_WaitingRoom. "));
            m_pMain->Set_Enable(false);
			m_pWaitingRoom->Set_Enable(true);
            m_pCurUI = (CUIBackground*)m_pWaitingRoom;
            m_pCurUI->Icon_Lock(m_bLock);
			break;

		case 2:
			SetWindowText(g_hWnd, TEXT("Level_Lobby_Store. "));
            m_pMain->Set_Enable(false);
			m_pStore->Set_Enable(true);
			break;

        case 4:/*종료*/
            DestroyWindow(g_hWnd);
            break;

		}
	}
#pragma endregion Lobby_Main

#pragma region  WaitingRoom
	else if (m_pWaitingRoom->Get_Enable())//대기실
	{
		_uint iSelectedMenu = static_cast<CUIBackground*>(m_pWaitingRoom)->Selected_Menu();

		switch (iSelectedMenu)
		{
		case 2://추가
			SetWindowText(g_hWnd, TEXT("Level_Lobby_AddItems. "));
			m_pWaitingRoom->Set_Enable(false);
			m_pAddItems->Set_Enable(true);
			break;

		case 3://구입
			SetWindowText(g_hWnd, TEXT("Level_Lobby_Store. "));
			m_pWaitingRoom->Set_Enable(false);
			m_pStore->Set_Enable(true);
			break;

		case 4://떠나기
			SetWindowText(g_hWnd, TEXT("Level_Lobby_Main. "));
            m_pMain->Set_Enable(true);
			m_pWaitingRoom->Set_Enable(false);
            m_pCurUI = (CUIBackground*)m_pMain;
            m_pCurUI->Icon_Lock(m_bLock);
			break;

		case 6://시작
			m_pWaitingRoom->Set_Enable(false);
			if (FAILED(CGameInstance::Get_Instance()->Add_ReserveLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY), LEVEL_GAMEPLAY)))
				return;
			break;
		}
	}
#pragma endregion  WaitingRoom
 
    }
}

void CMenuScreen::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
#ifdef _DEBUG
    m_pRendererCom->Add_DebugRenderGroup(m_pOBBCom);
#endif
}

HRESULT CMenuScreen::Render()
{
    _uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        _int        iPassIndex = 2;

        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
            continue;
        if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
        {
            iPassIndex = 4;
            if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_EmissiveTexture", i, aiTextureType_EMISSIVE)))
            {
                iPassIndex = 0;
            }
        }


        m_pModelCom->Render(i, m_pShaderCom, iPassIndex);
    }

    return S_OK;
}


HRESULT CMenuScreen::Setup_Component()
{
    /* For.Com_Shader*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;


    /* For.Com_Renderer*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
        return E_FAIL;

    /* For.Com_Model*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Screen"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;

    /* For.Com_OBB*/
    CCollider::COLLIDERDESC			ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

    ColliderDesc.vScale = _float3(5.f, 4.f, 1.f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, 0.f, ColliderDesc.vScale.z * 0.3f);
    ColliderDesc.pOwner = this;
    ColliderDesc.m_eObjID = COLLISION_TYPE::ITEM;

    if (FAILED(__super::Add_Component(LEVEL_LOBBY, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom,&ColliderDesc)))
        return E_FAIL;


    return S_OK;
}

HRESULT CMenuScreen::Setup_Screen()
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    _float4 WorldPos;
    XMStoreFloat4(&WorldPos, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));

    /* For.Lobby*/
    if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, TEXT("Layer_Screen"), TEXT("Prototype_GameObject_Main"), (CGameObject**)& m_pMain,&WorldPos)))
        return E_FAIL;

    m_pMain->Icon_Lock(m_bLock);
    //m_pMain->Set_Enable(false);

    /* For.WaitingRoom*/
    if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, TEXT("Layer_Screen"), TEXT("Prototype_GameObject_WaitingRoom"), (CGameObject**)&m_pWaitingRoom,&WorldPos)))
        return E_FAIL;
    m_pWaitingRoom->Icon_Lock(m_bLock);
    m_pWaitingRoom->Set_Enable(false);

    ///* For.Store*/
    //if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, TEXT("Layer_Screen"), TEXT("Prototype_GameObject_Store"), (CGameObject**)&m_pStore)))
    //    return E_FAIL;

    ///* For.AddItems*/
    //if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, TEXT("Layer_Screen"), TEXT("Prototype_GameObject_AddItems"), (CGameObject**)&m_pAddItems)))
    //    return E_FAIL;

    m_pCurUI = (CUIBackground*)m_pMain;

    RELEASE_INSTANCE(CGameInstance);
    return S_OK;
}

HRESULT CMenuScreen::Setup_Camera()
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    CCamera::CAMERADESC			CameraDesc;
    ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

    _float4		vPos, vAt;
    XMStoreFloat4(&vPos, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
    XMStoreFloat4(&vAt, m_pTransformCom->Get_State(CTransform::STATE_LOOK) + m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) * 0.5f);

    CameraDesc.vEye = vPos;
    CameraDesc.vAt = vAt;
    CameraDesc.TransformDesc.fSpeedPerSec = 5.f;
    CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

    CameraDesc.fFovy = XMConvertToRadians(65.0f);
    CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
    CameraDesc.fNear = 0.2f;
    CameraDesc.fFar = 300.f;

    CGameObject* pCamera = nullptr;

    if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_Camera"), TEXT("Prototype_GameObject_Camera_Fixed"), &pCamera, &CameraDesc)))
        return E_FAIL;

    static_cast<CCamera_Fixed*>(pCamera)->Set_Target(m_pTransformCom);

    if (FAILED(pGameInstance->Add_CameraObject(TEXT("Camera_Fixed"), (CCamera*)pCamera)))
        return E_FAIL;

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

void CMenuScreen::On_Collision_Enter(CCollider* pCollider)
{
   
}

void CMenuScreen::On_Collision_Stay(CCollider* pCollider)
{
    if (COLLISION_TYPE::SIGHT == pCollider->Get_Type())
    {
        if (GAMEINSTANCE->Is_KeyState(KEY::LBUTTON, KEY_STATE::TAP))
        {
            if (FAILED(GAMEINSTANCE->Change_Camera(TEXT("Camera_Fixed"))))
                return;
            m_bLock = false;
            m_pCurUI->Icon_Lock(m_bLock);

        }
    }
}

void CMenuScreen::On_Collision_Exit(CCollider* pCollider)
{
}

HRESULT CMenuScreen::SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix)
{
    if (nullptr == m_pShaderCom||
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

CMenuScreen* CMenuScreen::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMenuScreen* pInstance = new CMenuScreen(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CMenuScreen");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMenuScreen::Clone(void* pArg)
{
    CMenuScreen* pInstance = new CMenuScreen(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CMenuScreen");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMenuScreen::Free()
{
    __super::Free();
    
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pRendererCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pOBBCom);

   
    //해당 클래스에 있는 변수들은 항상 safe_release해주기
}
