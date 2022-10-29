#include "stdafx.h"
#include "..\Public\Level_StreetHouse.h"
#include "GameInstance.h"
#include "House.h"
#include "Door.h"
#include "Object_Collider.h"
#include "Computer.h"



CLevel_StreetHouse::CLevel_StreetHouse(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_StreetHouse::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;
	/*메쉬 등 추가*/

	if (FAILED(Ready_Layer_SkyBox(TEXT("Layer_SkyBox"))))
		return E_FAIL;
	//임구이 안 쓸 때
	//if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;
	/////////////////////////

	if(FAILED(Ready_Lights()))
		return E_FAIL;

	//if (FAILED(GAMEINSTANCE->Add_Font(m_pDevice, m_pContext, TEXT("Font_Dream"), TEXT("../Bin/Resources/Fonts/128.spriteFont"))))
	//	return E_FAIL;

	if (FAILED(Load_TruckProps()))
		return E_FAIL;

	if(FAILED(Load_Stage()))
		return E_FAIL;

	if (FAILED(GAMEINSTANCE->Change_Camera(TEXT("Camera_Player"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_StreetHouse::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		

	GAMEINSTANCE->Add_Light(m_pBaseLight);
	GAMEINSTANCE->Add_Desc(CEvent_Manager::PLAYTIME, fTimeDelta);

}

HRESULT CLevel_StreetHouse::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("LEVEL_GAMEPLAY. "));

	return S_OK;
}

HRESULT CLevel_StreetHouse::Ready_Layer_SkyBox(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.Lobby*/
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_StreetHouse::Ready_Layer_Player(const _tchar* pLayertag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_float4 vPosition = _float4(59.3f, 7.f, 44.2f, 1.f);

	if(FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayertag, TEXT("Prototype_GameObject_Player"),nullptr,&vPosition)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_StreetHouse::Load_Stage()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	char Filepath[255] = "../Bin/Resources/Map/StreetHouse/House"; 
	
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

			MODEL_TAG	 iModelTag = tDataMap.tModelTag;
			_float4x4	 WorldMat;
			XMStoreFloat4x4(&WorldMat,tDataMap.matWorld);

			if (FAILED(pGameInstance->Add_GameObject(
				LEVEL_GAMEPLAY,
				TEXT("Layer_House"),
				TEXT("Prototype_GameObject_StreetHouse"),
				nullptr,&WorldMat)))
			{
				MSG_BOX("Fail");
				RELEASE_INSTANCE(CGameInstance);
				return E_FAIL;
			}

		}
	}

	CloseHandle(hFile);
	//MSG_BOX("Loaded Map");


	strcpy_s(Filepath,"../Bin/Resources/Map/StreetHouse/Door");

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
				LEVEL_GAMEPLAY,
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

	strcpy_s(Filepath, "../Bin/Resources/Map/StreetHouse/Object_Collider");
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
				LEVEL_GAMEPLAY,
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


	strcpy_s(Filepath, "../Bin/Resources/Map/StreetHouse/Wall");
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
				LEVEL_GAMEPLAY,
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


	strcpy_s(Filepath, "../Bin/Resources/Map/StreetHouse/Atmosphere");

	hFile = CreateFileA(Filepath,
		GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed to load file");
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}
	dwByteHouse = 0;
	OBJ_DATA tDataObj;
	ZeroMemory(&tDataObj, sizeof(OBJ_DATA));
	while (true)
	{
		if (TRUE == ReadFile(hFile, &tDataObj, sizeof(OBJ_DATA), &dwByteHouse, nullptr))
		{
			if (0 == dwByteHouse)
			{
				break;
			}


			_float4x4 WorldMat;
			XMStoreFloat4x4(&WorldMat, tDataObj.matWorld);


			if (FAILED(pGameInstance->Add_GameObject(
				LEVEL_GAMEPLAY,
				TEXT("Layer_House"),
				TEXT("Prototype_GameObject_Atmosphere"),
				nullptr, &WorldMat)))
			{
				MSG_BOX("Fail");
				RELEASE_INSTANCE(CGameInstance);
				return E_FAIL;
			}

		}
	}
	CloseHandle(hFile);


	strcpy_s(Filepath, "../Bin/Resources/Map/StreetHouse/LightSwitch");
	
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
				LEVEL_GAMEPLAY,
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



	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Ghost"), TEXT("Prototype_GameObject_Ghost_SpawnPoint"),
		nullptr, TEXT("../Bin/Resources/Map/StreetHouse/"))))
		return E_FAIL;



	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_StreetHouse::Ready_Lights()
{
	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(0.5f, 0.5f, 0.5f, 1.f);

	m_pBaseLight = CLight::Create(m_pDevice, m_pContext, LightDesc);
	if (nullptr == m_pBaseLight)
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_StreetHouse::Ready_Layer_Terrain(const _tchar* pLayertag)
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayertag, TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;

	

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_StreetHouse::Load_TruckProps()
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	char Filepath[255] = "../Bin/Resources/Map/StreetHouse/Truck_Position";
	HANDLE hFile = CreateFileA(Filepath,
		GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed to load file");
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}
	DWORD dwByteHouse = 0;
	MAP_DATA tTruckPos;
	ZeroMemory(&tTruckPos, sizeof(MAP_DATA));
	_matrix	TruckWorldMat;
	while (true)
	{
		if (TRUE == ReadFile(hFile, &tTruckPos, sizeof(MAP_DATA), &dwByteHouse, nullptr))
		{
			if (0 == dwByteHouse)
			{
				break;
			}

			TruckWorldMat = tTruckPos.matWorld;
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
			XMStoreFloat4x4(&WorldMatrix, LocalMat*TruckWorldMat);

			if (pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Truck"), TEXT("Prototype_GameObject_Truck"), nullptr, &WorldMatrix))
				return E_FAIL;
		}
	}

	CloseHandle(hFile);

	strcpy_s(Filepath, "../Bin/Resources/Map/Default/TruckInside_Default");
	hFile = CreateFileA(Filepath,
		GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed to load file");
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	dwByteHouse = 0;

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
			XMStoreFloat4x4(&WorldMatrix, LocalMat * TruckWorldMat);

			if (pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Truck"), TEXT("Prototype_GameObject_Truck_Inside"), nullptr, &WorldMatrix))
				return E_FAIL;
		}
	}

	CloseHandle(hFile);

	strcpy_s(Filepath, "../Bin/Resources/Map/Default/Computer_Default");
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

			CComputer::COMPUTERDESC ComputerDesc;
			ComputerDesc.iNumCamera = 3;
			XMStoreFloat4x4(&ComputerDesc.TruckMat, TruckWorldMat);
			XMStoreFloat4x4(&ComputerDesc.WorldMat, tData.matWorld * TruckWorldMat);


			if (pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Truck"), TEXT("Prototype_GameObject_Computer"), nullptr, &ComputerDesc))
				return E_FAIL;
		}
	}

	CloseHandle(hFile);


	strcpy_s(Filepath, "../Bin/Resources/Map/Default/KeyPad_Default");
	hFile = CreateFileA(Filepath,
		GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed to load file");
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	dwByteHouse = 0;

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
			XMStoreFloat4x4(&WorldMatrix, LocalMat * TruckWorldMat);

			if (pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Truck"), TEXT("Prototype_GameObject_KeyPad"), nullptr, &WorldMatrix))
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
	OBJ_DATA tDataObj;
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

			if (pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Item"), TEXT("Prototype_GameObject_DotsProjecter"), nullptr, &WorldMatrix))
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

			if (pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Item"), TEXT("Prototype_GameObject_EMF"), nullptr, &WorldMatrix))
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
	tDataObj;
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

			if (pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Item"), TEXT("Prototype_GameObject_FlashLight"), nullptr, &WorldMatrix))
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

			if (pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Item"), TEXT("Prototype_GameObject_Note"), nullptr, &WorldMatrix))
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

			if (pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Item"), TEXT("Prototype_GameObject_SpiritBox"), nullptr, &WorldMatrix))
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

	//		if (pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Item"), TEXT("Prototype_GameObject_Photo_Camera"), nullptr, &WorldMatrix))
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

			if (pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Item"), TEXT("Prototype_GameObject_Thermometer"), nullptr, &WorldMatrix))
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

			if (pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Item"), TEXT("Prototype_GameObject_TrailCam"), nullptr, &WorldMatrix))
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

			if (pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Item"), TEXT("Prototype_GameObject_Tripod"), nullptr, &WorldMatrix))
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

			if (pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Item"), TEXT("Prototype_GameObject_UVLight"), nullptr, &WorldMatrix))
				return E_FAIL;
		}
	}

	CloseHandle(hFile);
	
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



CLevel_StreetHouse * CLevel_StreetHouse::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_StreetHouse*		pInstance = new CLevel_StreetHouse(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_StreetHouse");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_StreetHouse::Free()
{
	__super::Free();

	Safe_Release(m_pBaseLight);

}

