#include "stdafx.h"
#include "../Public/Computer.h"
#include "GameInstance.h"
#include "Video_Camera.h"
#include "Screen.h"
#include "Mouse.h"
#include "KeyBoard.h"

CComputer::CComputer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice,pContext)
{
}

CComputer::CComputer(const CComputer& rhs)
    :CGameObject(rhs)
{
}

HRESULT CComputer::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CComputer::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(nullptr)))
        return E_FAIL;

    if (nullptr != pArg)
    {
        COMPUTERDESC ComputerDesc = *(COMPUTERDESC*)pArg;
        m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&ComputerDesc.WorldMat));

        if(FAILED(Connect_Camera(ComputerDesc.iNumCamera,ComputerDesc.TruckMat)))
            return E_FAIL;

        if (FAILED(Setup_Computer(ComputerDesc.TruckMat)))
            return E_FAIL;
    }

    if (FAILED(Setup_Component()))
        return E_FAIL;

    if (FAILED(Setup_Screen()))
        return E_FAIL;



    return S_OK;
}

void CComputer::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
 
    for (auto& iter = m_listCameraOff.begin(); iter != m_listCameraOff.end();)
    {
        if (true == (*iter)->Get_Switch())
        {
            m_listCameraOn.push_back(*iter);
            iter = m_listCameraOff.erase(iter);
        }
        else
            ++iter;
    }

    for (auto& iter = m_listCameraOn.begin(); iter != m_listCameraOn.end();)
    {
        if (false == (*iter)->Get_Switch())
        {
            m_listCameraOff.push_back(*iter);
            iter = m_listCameraOn.erase(iter);
        }
        else
            ++iter;
    }

    if (m_iCurrCamIndex >0 && m_iCurrCamIndex >= m_listCameraOn.size())
    {
        m_iCurrCamIndex = m_listCameraOn.size()-1;
    }

  
}

void CComputer::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    if (true == m_pKeyBoard->Is_Clicked())// 키보드 눌림
    {
        ++m_iCurrCamIndex;
        if (m_iCurrCamIndex == m_listCameraOn.size())
            m_iCurrCamIndex = 0;
    }


    GAMEINSTANCE->Add_Object_For_Culling(this,CRenderer::RENDER_NONALPHABLEND);

}

HRESULT CComputer::Render()
{
    _uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        _int    iPassIndex = 2;
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



HRESULT CComputer::Setup_Component()
{
    /* For.Com_Shader*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    /* For.Com_Model*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Computer"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
        return E_FAIL;
   
    return S_OK;
}

HRESULT	CComputer::Setup_Screen()
{
    if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Truck"), TEXT("Prototype_GameObject_ComputerScreen"), nullptr, this)))
        return E_FAIL;


    return S_OK;
}

HRESULT CComputer::Setup_Computer(_float4x4 _TruckMat)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    char Filepath[255] =  "../Bin/Resources/Map/Default/KeyBoard_Default";
    HANDLE hFile = CreateFileA(Filepath,
        GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (INVALID_HANDLE_VALUE == hFile)
    {
        MSG_BOX("Failed to load file");
        RELEASE_INSTANCE(CGameInstance);
        return E_FAIL;
    }

    DWORD dwByteHouse = 0;
    MAP_DATA tData;
    ZeroMemory(&tData, sizeof(MAP_DATA));

    while (true)
    {
        if (TRUE == ReadFile(hFile, &tData, sizeof(MAP_DATA), &dwByteHouse, nullptr))
        {
            if (0 == dwByteHouse)
            {
                break;
            }

            _matrix LocalMat = tData.matWorld;
            _float4x4 WorldMatrix;
            XMStoreFloat4x4(&WorldMatrix, LocalMat * XMLoadFloat4x4(&_TruckMat));

            if (pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Truck"), TEXT("Prototype_GameObject_KeyBoard"), (CGameObject**)&m_pKeyBoard, &WorldMatrix))
                return E_FAIL;
            Safe_AddRef(m_pKeyBoard);
        }
    }

    CloseHandle(hFile);

    strcpy_s(Filepath, "../Bin/Resources/Map/Default/Mouse_Default");
    hFile = CreateFileA(Filepath,
        GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (INVALID_HANDLE_VALUE == hFile)
    {
        MSG_BOX("Failed to load file");
        RELEASE_INSTANCE(CGameInstance);
        return E_FAIL;
    }

    dwByteHouse = 0;
    //MAP_DATA tData;
    ZeroMemory(&tData, sizeof(MAP_DATA));

    while (true)
    {
        if (TRUE == ReadFile(hFile, &tData, sizeof(MAP_DATA), &dwByteHouse, nullptr))
        {
            if (0 == dwByteHouse)
            {
                break;
            }

            _matrix LocalMat = tData.matWorld;
            _float4x4 WorldMatrix;
            XMStoreFloat4x4(&WorldMatrix, LocalMat * XMLoadFloat4x4(&_TruckMat));

            if (pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Truck"), TEXT("Prototype_GameObject_Mouse"), (CGameObject**)&m_pMouse, &WorldMatrix))
                return E_FAIL;
            Safe_AddRef(m_pMouse);
        }
    }

    CloseHandle(hFile);

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}


HRESULT	CComputer::Connect_Camera(_int _iNumCamera, _float4x4 _TruckMat)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    char Filepath[255]= "../Bin/Resources/Map/Default/Video_Default";
    HANDLE hFile = CreateFileA(Filepath,
        GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (INVALID_HANDLE_VALUE == hFile)
    {
        MSG_BOX("Failed to load file");
        RELEASE_INSTANCE(CGameInstance);
        return E_FAIL;
    }

    DWORD dwByteHouse = 0;
    OBJ_DATA tDataObj;
    ZeroMemory(&tDataObj, sizeof(OBJ_DATA));

    CVideo_Camera* pTemp = nullptr;

    while (true)
    {
        if (TRUE == ReadFile(hFile, &tDataObj, sizeof(OBJ_DATA), &dwByteHouse, nullptr))
        {
            if (0 == dwByteHouse)
            {
                break;
            }

            _matrix LocalMat = tDataObj.matWorld;
            _float4x4 WorldMatrix;
            XMStoreFloat4x4(&WorldMatrix, LocalMat * XMLoadFloat4x4(&_TruckMat));

            if (pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Item"), TEXT("Prototype_GameObject_Video_Camera"), (CGameObject**) & pTemp, &WorldMatrix))
                return E_FAIL;

            m_listCameraOff.push_back(pTemp);
            Safe_AddRef(pTemp);
        }
    }

    CloseHandle(hFile);

    RELEASE_INSTANCE(CGameInstance);

    return S_OK;
}

HRESULT CComputer::SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix)
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

HRESULT CComputer::Set_Screen_RSV(CShader* _pShaderCom)
{
    if (m_listCameraOn.empty())
        return E_FAIL;

    auto& iter = m_listCameraOn.begin();
    for (_int i = 0; i < m_iCurrCamIndex; ++i)
        ++iter;

    if(FAILED(_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", (*iter)->Get_CameraScreen_SRV())))
        return E_FAIL;

    return S_OK;
}

CComputer* CComputer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CComputer* pInstance = new CComputer(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CComputer");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CComputer::Clone(void* pArg)
{
    CComputer* pInstance = new CComputer(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CComputer");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CComputer::Free()
{
    __super::Free();
    
    for (auto& elem : m_listCameraOn)
        Safe_Release(elem);

    for (auto& elem : m_listCameraOff)
        Safe_Release(elem);

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pOBBCom);
    Safe_Release(m_pModelCom);

    Safe_Release(m_pMouse);
    Safe_Release(m_pKeyBoard);

   
    //해당 클래스에 있는 변수들은 항상 safe_release해주기
}
