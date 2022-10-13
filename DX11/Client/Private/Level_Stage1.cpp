#include "stdafx.h"
#include "..\Public\Level_Stage1.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "House.h"
#include "Door.h"
#include "Object_Collider.h"



CLevel_Stage1::CLevel_Stage1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Stage1::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;
	/*메쉬 등 추가*/

	if (FAILED(Ready_Layer_SkyBox(TEXT("Layer_SkyBox"))))
		return E_FAIL;
	//임구이 안 쓸 때
	/*if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;*/
	/////////////////////////

	if(FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(GAMEINSTANCE->Add_Font(m_pDevice, m_pContext, TEXT("Font_Dream"), TEXT("../Bin/Resources/Fonts/128.spriteFont"))))
		return E_FAIL;

	if (FAILED(Load_TruckProps()))
		return E_FAIL;

	if(FAILED(Load_Stage()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Stage1::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		

	GAMEINSTANCE->Add_Light(m_pBaseLight);

}

HRESULT CLevel_Stage1::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("Level_Stage1. "));

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_SkyBox(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.Lobby*/
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1, pLayerTag, TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_Camera(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float4(0.0f, 20.f, -100.f, 1.f);
	CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.TransformDesc.fSpeedPerSec = 20.f;
	CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	CameraDesc.fFovy = XMConvertToRadians(65.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1, pLayerTag, TEXT("Prototype_GameObject_Camera_Free"), nullptr,&CameraDesc)))
		return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_Player(const _tchar* pLayertag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if(FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1, pLayertag, TEXT("Prototype_GameObject_Player"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage1::Load_Stage()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	char Filepath[255] = "../Bin/Resources/Map/NormalHouse/House"; 
	
	HANDLE hFile = CreateFileA(Filepath,
		GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed to load file");
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}
	DWORD dwByteHouse = 0;
	MAP_DATA tDataMap;
	ZeroMemory(&tDataMap, sizeof(MAP_DATA));
	while (true)
	{
		if (TRUE == ReadFile(hFile, &tDataMap, sizeof(MAP_DATA), &dwByteHouse, nullptr))
		{
			if (0 == dwByteHouse)
			{
				break;
			}

			CGameObject* pTemp = nullptr;
			MODEL_TAG	 iModelTag = tDataMap.tModelTag;

			if (FAILED(pGameInstance->Add_GameObject(
				LEVEL_STAGE1,
				TEXT("Layer_House"),
				TEXT("Prototype_GameObject_House"),
				&pTemp)))
			{
				MSG_BOX("Fail");
				RELEASE_INSTANCE(CGameInstance);
				return E_FAIL;
			}

			CTransform* pTransform = (CTransform*)pTemp->Get_Component(CGameObject::m_pTransformTag);
			pTransform->Set_WorldMatrix(tDataMap.matWorld);

			switch (iModelTag)
			{
			case MODEL_TAG::FURNISHEDCABIN:
				static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_FurnishedCabin"));

				break;
			}


		}
	}

	CloseHandle(hFile);
	//MSG_BOX("Loaded Map");

	strcpy_s(Filepath,"../Bin/Resources/Map/NormalHouse/Door");

	hFile = CreateFileA(Filepath,
		GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed to load file");
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}
	dwByteHouse = 0;
	ZeroMemory(&tDataMap, sizeof(MAP_DATA));
	while (true)
	{
		if (TRUE == ReadFile(hFile, &tDataMap, sizeof(MAP_DATA), &dwByteHouse, nullptr))
		{
			if (0 == dwByteHouse)
			{
				break;
			}

			CGameObject* pTemp = nullptr;
			MODEL_TAG	 iModelTag = tDataMap.tModelTag;

			if (FAILED(pGameInstance->Add_GameObject(
				LEVEL_STAGE1,
				TEXT("Layer_House"),
				TEXT("Prototype_GameObject_Door"),
				&pTemp)))
			{
				MSG_BOX("Fail");
				RELEASE_INSTANCE(CGameInstance);
				return E_FAIL;
			}
			CTransform* pTransform = (CTransform*)pTemp->Get_Component(CGameObject::m_pTransformTag);
			pTransform->Set_WorldMatrix(tDataMap.matWorld);

			switch (iModelTag)
			{
			case MODEL_TAG::ROOMDOOR:
				static_cast<CDoor*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoomDoor"));

				break;
			case MODEL_TAG::MAINDOOR:
				static_cast<CDoor*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_MainDoor"));
				static_cast<CDoor*>(pTemp)->Set_MainDoor(true);
				break;
			}


		}
	}

	CloseHandle(hFile);

	//Load Collider
	///////////////////////////////////////////////////////////////////////////////

	strcpy_s(Filepath, "../Bin/Resources/Map/NormalHouse/Object_Collider");
	hFile = CreateFileA(Filepath,
		GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed to load file");
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}
	
	dwByteHouse = 0;
	COLLIDER_DATA tDataCollider;
	ZeroMemory(&tDataCollider, sizeof(COLLIDER_DATA));
	while (true)
	{
		if (TRUE == ReadFile(hFile, &tDataCollider, sizeof(COLLIDER_DATA), &dwByteHouse, nullptr))
		{
			if (0 == dwByteHouse)
			{
				break;
			}

			CGameObject* pTemp = nullptr;

			if (FAILED(pGameInstance->Add_GameObject(
				LEVEL_STAGE1,
				TEXT("Layer_Collider"),
				TEXT("Prototype_GameObject_Collider"),
				&pTemp)))
			{
				MSG_BOX("Fail");
				RELEASE_INSTANCE(CGameInstance);
				return E_FAIL;
			}

			CTransform* pTransform = (CTransform*)pTemp->Get_Component(CGameObject::m_pTransformTag);
			pTransform->Set_WorldMatrix(tDataCollider.matWorld);

		}
	}

	CloseHandle(hFile);
	//MSG_BOX("Loaded Collider");

	strcpy_s(Filepath, "../Bin/Resources/Map/NormalHouse/Wall");
	hFile = CreateFileA(Filepath,
		GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed to load file");
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	dwByteHouse = 0;
	tDataCollider;
	ZeroMemory(&tDataCollider, sizeof(COLLIDER_DATA));
	while (true)
	{
		if (TRUE == ReadFile(hFile, &tDataCollider, sizeof(COLLIDER_DATA), &dwByteHouse, nullptr))
		{
			if (0 == dwByteHouse)
			{
				break;
			}

			CGameObject* pTemp = nullptr;

			if (FAILED(pGameInstance->Add_GameObject(
				LEVEL_STAGE1,
				TEXT("Layer_Collider"),
				TEXT("Prototype_GameObject_Wall"),
				&pTemp)))
			{
				MSG_BOX("Fail");
				RELEASE_INSTANCE(CGameInstance);
				return E_FAIL;
			}

			CTransform* pTransform = (CTransform*)pTemp->Get_Component(CGameObject::m_pTransformTag);
			pTransform->Set_WorldMatrix(tDataCollider.matWorld);
			
		}
	}

	CloseHandle(hFile);


	strcpy_s(Filepath, "../Bin/Resources/Map/NormalHouse/LightSwitch");
	
	hFile = CreateFileA(Filepath,
		GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed to load file");
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}
	dwByteHouse = 0;
	SWITCH_DATA tData;
	ZeroMemory(&tData, sizeof(SWITCH_DATA));
	while (true)
	{
		if (TRUE == ReadFile(hFile, &tData, sizeof(SWITCH_DATA), &dwByteHouse, nullptr))
		{
			if (0 == dwByteHouse)
			{
				break;
			}

			if (FAILED(pGameInstance->Add_GameObject(
				LEVEL_STAGE1,
				TEXT("Layer_Light"),
				TEXT("Prototype_GameObject_LightSwitch"),
				nullptr, &tData)))
			{
				MSG_BOX("Fail");
				RELEASE_INSTANCE(CGameInstance);
				return E_FAIL;
			}
		
		}
	}
	CloseHandle(hFile);
	//MSG_BOX("Loaded Items");

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Lights()
{
	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
	LightDesc.vSpecular = _float4(0.2f, 0.2f, 0.2f, 1.f);

	m_pBaseLight = CLight::Create(m_pDevice, m_pContext, LightDesc);
	if (nullptr == m_pBaseLight)
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_Terrain(const _tchar* pLayertag)
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1, pLayertag, TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;

	

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage1::Load_TruckProps()
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	char Filepath[255] = "../Bin/Resources/Map/NormalHouse/Truck_Position";
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
	_matrix	TruckWorldMat;
	while (true)
	{
		if (TRUE == ReadFile(hFile, &tDataObj, sizeof(OBJ_DATA), &dwByteHouse, nullptr))
		{
			if (0 == dwByteHouse)
			{
				break;
			}

			TruckWorldMat = tDataObj.matWorld;
		}
	}

	CloseHandle(hFile);
	/*트럭의 위치 받아오기 방벡은 모두 normalize한 후에 넘겨줌
	*/
	TruckWorldMat.r[0]=XMVector3Normalize(TruckWorldMat.r[0]);
	TruckWorldMat.r[1] = XMVector3Normalize(TruckWorldMat.r[1]);
	TruckWorldMat.r[2] = XMVector3Normalize(TruckWorldMat.r[2]);


	strcpy_s(Filepath, "../Bin/Resources/Map/Default/Truck_Default");
	hFile = CreateFileA(Filepath,
		GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed to load file");
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	dwByteHouse = 0;
	ZeroMemory(&tDataObj, sizeof(OBJ_DATA));

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
			XMStoreFloat4x4(&WorldMatrix, LocalMat*TruckWorldMat);

			if (pGameInstance->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Truck"), TEXT("Prototype_GameObject_Truck"), nullptr, &WorldMatrix))
				return E_FAIL;
		}
	}

	CloseHandle(hFile);

	strcpy_s(Filepath, "../Bin/Resources/Map/Default/Dots_Default");
	hFile = CreateFileA(Filepath,
		GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed to load file");
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	dwByteHouse = 0;
	ZeroMemory(&tDataObj, sizeof(OBJ_DATA));

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
			XMStoreFloat4x4(&WorldMatrix, LocalMat * TruckWorldMat);

			if (pGameInstance->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Item"), TEXT("Prototype_GameObject_DotsProjecter"), nullptr, &WorldMatrix))
				return E_FAIL;
		}
	}

	CloseHandle(hFile);

	strcpy_s(Filepath, "../Bin/Resources/Map/Default/EMF_Default");
	hFile = CreateFileA(Filepath,
		GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed to load file");
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	dwByteHouse = 0;
	ZeroMemory(&tDataObj, sizeof(OBJ_DATA));

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
			XMStoreFloat4x4(&WorldMatrix, LocalMat * TruckWorldMat);

			if (pGameInstance->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Item"), TEXT("Prototype_GameObject_EMF"), nullptr, &WorldMatrix))
				return E_FAIL;
		}
	}

	CloseHandle(hFile);

	strcpy_s(Filepath, "../Bin/Resources/Map/Default/FlashLight_Default");
	hFile = CreateFileA(Filepath,
		GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed to load file");
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	dwByteHouse = 0;
	ZeroMemory(&tDataObj, sizeof(OBJ_DATA));

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
			XMStoreFloat4x4(&WorldMatrix, LocalMat* TruckWorldMat);

			if (pGameInstance->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Item"), TEXT("Prototype_GameObject_FlashLight"), nullptr, &WorldMatrix))
				return E_FAIL;
		}
	}

	CloseHandle(hFile);

	strcpy_s(Filepath, "../Bin/Resources/Map/Default/Note_Default");
	hFile = CreateFileA(Filepath,
		GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed to load file");
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	dwByteHouse = 0;
	ZeroMemory(&tDataObj, sizeof(OBJ_DATA));

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
			XMStoreFloat4x4(&WorldMatrix, LocalMat* TruckWorldMat);

			if (pGameInstance->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Item"), TEXT("Prototype_GameObject_Note"), nullptr, &WorldMatrix))
				return E_FAIL;
		}
	}

	CloseHandle(hFile);

	strcpy_s(Filepath, "../Bin/Resources/Map/Default/SpiritBox_Default");
	hFile = CreateFileA(Filepath,
		GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed to load file");
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	dwByteHouse = 0;
	ZeroMemory(&tDataObj, sizeof(OBJ_DATA));

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
			XMStoreFloat4x4(&WorldMatrix, LocalMat* TruckWorldMat);

			if (pGameInstance->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Item"), TEXT("Prototype_GameObject_SpiritBox"), nullptr, &WorldMatrix))
				return E_FAIL;
		}
	}

	CloseHandle(hFile);

	//strcpy_s(Filepath, "../Bin/Resources/Map/Default/Photo_Default");//일단 비디오 카메라로
	//hFile = CreateFileA(Filepath,
	//	GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	//if (INVALID_HANDLE_VALUE == hFile)
	//{
	//	MSG_BOX("Failed to load file");
	//	RELEASE_INSTANCE(CGameInstance);
	//	return E_FAIL;
	//}

	//dwByteHouse = 0;
	//ZeroMemory(&tDataObj, sizeof(OBJ_DATA));

	//while (true)
	//{
	//	if (TRUE == ReadFile(hFile, &tDataObj, sizeof(OBJ_DATA), &dwByteHouse, nullptr))
	//	{
	//		if (0 == dwByteHouse)
	//		{
	//			break;
	//		}

	//		_matrix LocalMat = tDataObj.matWorld;
	//		_float4x4 WorldMatrix;
	//		XMStoreFloat4x4(&WorldMatrix, TruckWorldMat * LocalMat);

	//		if (pGameInstance->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Item"), TEXT("Prototype_GameObject_Photo_Camera"), nullptr, &WorldMatrix))
	//			return E_FAIL;
	//	}
	//}

	//CloseHandle(hFile);

	strcpy_s(Filepath, "../Bin/Resources/Map/Default/Thermometer_Default");
	hFile = CreateFileA(Filepath,
		GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed to load file");
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	dwByteHouse = 0;
	ZeroMemory(&tDataObj, sizeof(OBJ_DATA));

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
			XMStoreFloat4x4(&WorldMatrix, LocalMat* TruckWorldMat);

			if (pGameInstance->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Item"), TEXT("Prototype_GameObject_Thermometer"), nullptr, &WorldMatrix))
				return E_FAIL;
		}
	}

	CloseHandle(hFile);

	strcpy_s(Filepath, "../Bin/Resources/Map/Default/TrailCam_Default");
	hFile = CreateFileA(Filepath,
		GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed to load file");
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	dwByteHouse = 0;
	ZeroMemory(&tDataObj, sizeof(OBJ_DATA));

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
			XMStoreFloat4x4(&WorldMatrix, LocalMat* TruckWorldMat);

			if (pGameInstance->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Item"), TEXT("Prototype_GameObject_TrailCam"), nullptr, &WorldMatrix))
				return E_FAIL;
		}
	}

	CloseHandle(hFile);

	strcpy_s(Filepath, "../Bin/Resources/Map/Default/Tripod_Default");
	hFile = CreateFileA(Filepath,
		GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed to load file");
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	dwByteHouse = 0;
	ZeroMemory(&tDataObj, sizeof(OBJ_DATA));

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
			XMStoreFloat4x4(&WorldMatrix, LocalMat * TruckWorldMat);

			if (pGameInstance->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Item"), TEXT("Prototype_GameObject_Tripod"), nullptr, &WorldMatrix))
				return E_FAIL;
		}
	}

	CloseHandle(hFile);

	strcpy_s(Filepath, "../Bin/Resources/Map/Default/UVLight_Default");
	hFile = CreateFileA(Filepath,
		GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed to load file");
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	dwByteHouse = 0;
	ZeroMemory(&tDataObj, sizeof(OBJ_DATA));

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
			XMStoreFloat4x4(&WorldMatrix, LocalMat* TruckWorldMat);

			if (pGameInstance->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Item"), TEXT("Prototype_GameObject_UVLight"), nullptr, &WorldMatrix))
				return E_FAIL;
		}
	}

	CloseHandle(hFile);

	strcpy_s(Filepath, "../Bin/Resources/Map/Default/Video_Default");
	hFile = CreateFileA(Filepath,
		GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed to load file");
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	dwByteHouse = 0;
	ZeroMemory(&tDataObj, sizeof(OBJ_DATA));

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
			XMStoreFloat4x4(&WorldMatrix, LocalMat* TruckWorldMat);

			if (pGameInstance->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Item"), TEXT("Prototype_GameObject_Video_Camera"), nullptr, &WorldMatrix))
				return E_FAIL;
		}
	}

	CloseHandle(hFile);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



CLevel_Stage1 * CLevel_Stage1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Stage1*		pInstance = new CLevel_Stage1(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Stage1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Stage1::Free()
{
	__super::Free();

	Safe_Release(m_pBaseLight);

}

