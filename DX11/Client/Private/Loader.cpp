#include "stdafx.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"
#include "BackGround.h"
#include "Lobby_Main.h"
#include "UIIcon.h"
#include "Lobby_WaitingRoom.h"
#include "Lobby_AddItems.h"
#include "Lobby_Store.h"
#include "Object_Collider.h"
#include "Wall_Collider.h"
#include "Navigation_Mesh.h"

#include "Point.h"

#include "Ghost.h"
#include "Ghost_SpawnPoint.h"
#include "Ghost_Status.h"

#include "Temp_DotsProjecter.h"
#include "Temp_Note.h"
#include "Temp_TrailCam.h"
#include "Temp_VideoCam.h"

#include "Atmosphere.h"
#include "Truck.h"

#include "Camera_Free.h"
#include "Camera_FPS.h"
#include "Thermometer.h"
#include "TrailCam.h"
#include "Tripod.h"
#include "House.h"
#include "Door.h"
//#include "LightSwitch.h"
#include "EMF.h"
#include "Note.h"
#include "SpiritBox.h"
#include "Video_Camera.h"
#include "CameraScreen.h"
#include "Terrain.h"
#include "Player.h"
#include "Inventory.h"
#include "Sky.h"
#include "DotsProjecter.h"
#include "FlashLight.h"
#include "UVLight.h"

CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

unsigned int APIENTRY LoadingMain(void* pArg)
{
	CoInitializeEx(nullptr, 0);

	CLoader*		pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CS());

	HRESULT hr = 0;

	switch (pLoader->Get_NextLevelID())
	{
	case LEVEL_LOGO:
		hr = pLoader->Loading_ForLogoLevel();
		break;
	case LEVEL_LOBBY:
		hr = pLoader->Loading_ForLobbyLevel();
		break;
	case LEVEL_STAGE1:
		hr = pLoader->Loading_ForStage1Level();
		break;
	}	

	if (FAILED(hr))
		MSG_BOX("Failed to Loading");

	LeaveCriticalSection(&pLoader->Get_CS());

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevel)
{
	m_eNextLevel = eNextLevel;


	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_ForLogoLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	
#pragma region PROTOTYPE_GAMEOBJECT

	lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	///* For.Prototype_GameObject_Camera_Free*/
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
	//	CCamera_Free::Create(m_pGraphic_Device))))
	//	return E_FAIL;

#pragma endregion


	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중이비낟. "));
	/* For.Prototype_Component_Texture_Default */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Phasmophobia_Logo.dds")))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중이비낟. "));



	lstrcpy(m_szLoadingText, TEXT("로딩 끝 "));	

	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForLobbyLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

#pragma region PROTOTYPE_GAMEOBJECT

	lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));
	m_fLoadingRatio = 0.f;
	_float		fRatio = 1.f / 10.f;//로딩 퍼센테이지

	/*For, Prototype_GameObject_Lobby*/
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Lobby"),
		CLobby_Main::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	m_fLoadingRatio += fRatio;

	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_WaitingRoom"),
		CLobby_WaitingRoom::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	m_fLoadingRatio += fRatio;
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_Store"),
		CLobby_Store::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	m_fLoadingRatio += fRatio;
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_AddItems"),
		CLobby_AddItems::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	m_fLoadingRatio += fRatio;
	if (FAILED(CGameInstance::Get_Instance()->Add_Prototype(TEXT("Prototype_GameObject_LobbyIcon"),
		CUIIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	m_fLoadingRatio += fRatio;

#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중이비낟. "));
	/*For.Prototype_Component_Texture_Lobby*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Lobby/Lobby_Main.dds")))))
		return E_FAIL;
	m_fLoadingRatio += fRatio;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_OutLine"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Lobby/OutLine.dds")))))
		return E_FAIL;
	m_fLoadingRatio += fRatio;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_WaitingRoom"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/WaitingRoom/WaitingRoom.dds")))))
		return E_FAIL;
	m_fLoadingRatio += fRatio;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Large_outline"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/WaitingRoom/Large_outline.dds")))))
		return E_FAIL;
	m_fLoadingRatio += fRatio;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Small_outline"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/WaitingRoom/Small_outline.dds")))))
		return E_FAIL;
	m_fLoadingRatio += fRatio;


	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중이비낟. "));

	///* For.Prototype_Component_VIBuffer_Terrain */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
	//	CVIBuffer_Terrain::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
	//	return E_FAIL;

	///* For.Prototype_Component_VIBuffer_Cube*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
	//	CVIBuffer_Cube::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩 끝 "));

	m_isFinished = true;

	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CLoader::Loading_ForStage1Level()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

#pragma region PROTOTYPE_GAMEOBJECT

	lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));

	/* For.Prototype_GameObject_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_FPS*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_FPS"),
		CCamera_FPS::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	///* For.Prototype_GameObject_Player */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Inventory */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Inventory"),
		CInventory::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/*For. Prototype_GameObject_Navigation_Mesh*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Navigation_Mesh"),
		CNavigation_Mesh::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_Ghost*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Ghost"),
		CGhost::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Ghost_SpawnPoint*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Ghost_SpawnPoint"),
		CGhost_SpawnPoint::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Ghost_Status*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Ghost_Status"),
		CGhost_Status::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	/* For.Prototype_GameObject_Sky */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_DotsProjecter*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_DotsProjecter"),
		CDotsProjecter::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_FlashLight*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FlashLight"),
		CFlashLight::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_UVLight*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UVLight"),
		CUVLight::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_Thermometer*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Thermometer"),
		CThermometer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_EMF*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EMF"),
		CEMF::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Note*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Note"),
		CNote::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_TrailCam*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TrailCam"),
		CTrailCam::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Tripod*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tripod"),
		CTripod::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_SpiritBox*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SpiritBox"),
		CSpiritBox::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Video_Camera*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Video_Camera"),
		CVideo_Camera::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Collider*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Collider"),
		CObject_Collider::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Wall*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall"),
		CWall_Collider::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Door*/
	if(FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Door"),
		CDoor::Create(m_pDevice,m_pContext))))
		return E_FAIL;

	/* For. Prototype_GameObject_House*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_House"),
		CHouse::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For. Prototype_GameObject_Atmosphere*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Atmosphere"),
		CAtmosphere::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For. Prototype_GameObject_TempDotsProjecter*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TempDotsProjecter"),
		CTempDotsProjecter::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For. Prototype_GameObject_TempNote*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TempNote"),
		CTempNote::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For. Prototype_GameObject_TempVideoCam*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TempVideoCam"),
		CTempVideoCam::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For. Prototype_GameObject_TempTrailCam*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TempTrailCam"),
		CTempTrailCam::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For. Prototype_GameObject_Truck*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Truck"),
		CTruck::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	///* For.Prototype_GameObject_Effect */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect"),
	//	CEffect::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	/* For. Prototype_GameObject_CameraScreen*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CameraScreen"),
		CCamera_Screen::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#ifdef _DEBUG

	/* For.Prototype_GameObject_Point*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Point"),
		CPoint::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#endif

#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중이비낟. "));
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Grass_%d.dds"), 2))))
		return E_FAIL;

	///* For.Prototype_Component_Texture_Player */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Player/Player.jpg"), 1))))
	//	return E_FAIL;

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
		return E_FAIL;

	///* For.Prototype_Component_Texture_Monster */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Monster"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Snow/Snow.png"), 1))))
	//	return E_FAIL;

	///* For.Prototype_Component_Texture_Explosion */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Explosion"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Explosion/Explosion%d.png"), 90))))
	//	return E_FAIL;



	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중이비낟. "));

	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height2.bmp")))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cube*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Point_Instance*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_VIBuffer_Point_Instance"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, 200))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_NorRect*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_VIBuffer_NorRect"),
		CVIBuffer_NorRect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_CameraRenderer*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Camera_Renderer"),
		CCamera_Renderer::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	_matrix			TransformMatrix;

	//TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	///* For.Prototype_Component_Model_Lighter*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Lighter"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Lighter/", "Lighter.fbx", TransformMatrix))))
	//	return E_FAIL;

	TransformMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f)  * XMMatrixRotationX(XMConvertToRadians(270.f)) * XMMatrixRotationZ(XMConvertToRadians(90.f)) * XMMatrixTranslation(0.f, 0.5f, 0.f);

	/*For. Prototype_Component_Model_DotsProjecter*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_DotsProjecter"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/DotsProjecter/", "DotsProjecter.fbx", TransformMatrix))))
		return E_FAIL;

	TransformMatrix = XMMatrixScaling(0.09f, 0.09f, 0.09f)* XMMatrixTranslation(0.f, 0.35f, 0.f);

	/* For.Prototype_Component_Model_FlashLight*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_FlashLight"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/FlashLight/", "FlashLight.fbx", TransformMatrix))))
		return E_FAIL;

	TransformMatrix = XMMatrixScaling(0.09f, 0.09f, 0.07f) * XMMatrixTranslation(0.f, 0.35f, 0.f);

	/* For.Prototype_Component_Model_UVLight*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_UVLight"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/FlashLight/", "UVLight.fbx", TransformMatrix))))
		return E_FAIL;

	TransformMatrix = XMMatrixScaling(0.07f, 0.07f, 0.07f);


	/* For.Prototype_Component_Model_Thermometer*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Thermometer"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Thermometer/", "Thermometer.fbx", TransformMatrix))))
		return E_FAIL;

	TransformMatrix = XMMatrixScaling(0.06f, 0.06f, 0.06f)*XMMatrixTranslation(0.f,0.6f,0.f);

	/* For.Prototype_Component_Model_EMF*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_EMF"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/EMF/", "EMF.fbx", TransformMatrix))))
		return E_FAIL;

	TransformMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f) * XMMatrixTranslation(-0.7f, 0.f, 0.f)* XMMatrixRotationX(XMConvertToRadians(270.0f));
	/* For.Prototype_Component_Model_Note*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Note"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Note/", "Notepad.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Note_Open*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Note_Open"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Note/", "Notepad_Open.fbx", TransformMatrix))))
		return E_FAIL;

	TransformMatrix = XMMatrixScaling(0.08f, 0.08f, 0.08f) * XMMatrixRotationY(XMConvertToRadians(180.f))*XMMatrixTranslation(0.f,0.f,0.25f);

	/* For.Prototype_Component_Model_TrailCam*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_TrailCam"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/TrailCam/", "TrailCam.fbx", TransformMatrix))))
		return E_FAIL;

	TransformMatrix = XMMatrixScaling(0.09f, 0.09f, 0.09f);

	/* For.Prototype_Component_Model_SpiritBox*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_SpiritBox"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SpiritBox/", "SpiritBox.fbx", TransformMatrix))))
		return E_FAIL;

	TransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f));


	/* For.Prototype_Component_Model_Video_Camera*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Video_Camera"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Video_Camera/", "Video_Camera.fbx", TransformMatrix))))
		return E_FAIL;

	TransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	/* For.Prototype_Component_Model_Tripod*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Tripod"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Tripod/", "Tripod.fbx", TransformMatrix))))
		return E_FAIL;
	
	TransformMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f);
	/* For.Prototype_Component_Model_RoomDoor*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_RoomDoor"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/House/FurnishedCabin/", "RoomDoor.fbx", TransformMatrix))))
		return E_FAIL;
	TransformMatrix = XMMatrixScaling(0.07f, 0.05f, 0.05f) *XMMatrixTranslation(0.f,6.f,0.f);
	/* For.Prototype_Component_Model_MainDoor*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_MainDoor"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/House/FurnishedCabin/", "MainDoor.fbx", TransformMatrix))))
		return E_FAIL;



	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

	/* For.Prototype_Component_Model_MapleTree*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_MapleTree"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/MapleTree/", "Maple.fbx", TransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Truck*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Truck"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Truck/", "Truck.fbx", TransformMatrix))))
		return E_FAIL;
	
//	/* For.Prototype_Component_Model_AbandonedMarket*/
//	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_AbandonedMarket"),
//		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/House/AbandonedMarket/", "AbandonedMarket.fbx", TransformMatrix))))
//		return E_FAIL;
//	
	/* For.Prototype_Component_Model_FurnishedCabin*/
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_FurnishedCabin"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/House/FurnishedCabin/", "FurnishedCabin.fbx", TransformMatrix))))
			return E_FAIL;

	TransformMatrix = XMMatrixScaling(0.005f, 0.005f, 0.005f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

	/* For.Prototype_Component_Model_SuburbanHouse*/
	/*if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_SuburbanHouse"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/House/SuburbanHouse/", "SuburbanHouse1.fbx", TransformMatrix))))
		return E_FAIL;*/

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

//	/* For.Prototype_Component_Model_Garage 됨/중간 물건들 텍스처문제*/
//	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Garage"),
//		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/House/Garage/", "Garage.fbx", TransformMatrix))))
//		return E_FAIL;
	
	
	/* For.Prototype_Component_Model_Pier_house*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Pier_house"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/House/Pier_house/", "Pier_house.fbx", TransformMatrix))))
	//	return E_FAIL;
	//
	///* For.Prototype_Component_Model_Pier_house2*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Pier_house2"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/House/Pier_house2/", "Pier_house2.fbx", TransformMatrix))))
	//	return E_FAIL;
	//
	///* For.Prototype_Component_Model_RoofTop*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_RoofTop"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/House/RoofTop/", "RoofTop.fbx", TransformMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_RoofTop_Background1*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_RoofTop_Background1"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/House/RoofTop_Background/", "RoofTop_Background1.fbx", TransformMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_RoofTop_Background2*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_RoofTop_Background2"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/House/RoofTop_Background/", "RoofTop_Background2.fbx", TransformMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_RoofTop_Background3*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_RoofTop_Background3"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/House/RoofTop_Background/", "RoofTop_Background3.fbx", TransformMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_RoofTop_Background4*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_RoofTop_Background4"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/House/RoofTop_Background/", "RoofTop_Background1.fbx", TransformMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_RoofTop_Background5*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_RoofTop_Background5"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/House/RoofTop_Background/", "RoofTop_Background5.fbx", TransformMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_RoofTop_Background6*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_RoofTop_Background6"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/House/RoofTop_Background/", "RoofTop_Background6.fbx", TransformMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_RoofTop_Background7*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_RoofTop_Background7"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/House/RoofTop_Background/", "RoofTop_Background7.fbx", TransformMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_RoofTop_Background8*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_RoofTop_Background8"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/House/RoofTop_Background/", "RoofTop_Background8.fbx", TransformMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_RoofTop_Background9*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_RoofTop_Background9"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/House/RoofTop_Background/", "RoofTop_Background9.fbx", TransformMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_RoofTop_Background10*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_RoofTop_Background10"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/House/RoofTop_Background/", "RoofTop_Background10.fbx", TransformMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_RoofTop_Background11*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_RoofTop_Background11"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/House/RoofTop_Background/", "RoofTop_Background11.fbx", TransformMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_RoofTop_Background12*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_RoofTop_Background12"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/House/RoofTop_Background/", "RoofTop_Background12.fbx", TransformMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_RoofTop_Background13*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_RoofTop_Background13"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/House/RoofTop_Background/", "RoofTop_Background13.fbx", TransformMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_RoofTop_Background14*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_RoofTop_Background14"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/House/RoofTop_Background/", "RoofTop_Background14.fbx", TransformMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Shelter*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_Shelter"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/House/Shelter/", "Shelter.fbx", TransformMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_SlumHouse1*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_SlumHouse1"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/House/SlumHouse/", "SlumHouse1.fbx", TransformMatrix))))
	//	return E_FAIL;
	//
	//	/* For.Prototype_Component_Model_OldHouse*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Model_OldHouse"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/House/OldHouse/", "Well.fbx", TransformMatrix))))
	//	return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("콜라이더추가.  "));
	/* For.Prototype_Component_Collider_AABB */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_AABB"),
	//	CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
	//	return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE */	
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_Ray*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_Ray"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_RAY))))
		return E_FAIL;

	/* For. Prototype_Component_Navigation*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, "../Bin/Resources/Map/NormalHouse/Navigation4.dat"))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("셰이더를 로딩중이빈다. "));
	/* For.Prototype_Component_Shader_VtxNorTex */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Element, VTXNORTEX_DECLARATION::iNumElements))))
		return E_FAIL;


	/* For.Prototype_Component_Shader_VtxCubeTex */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Shader_VtxCubeTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCubeTex.hlsl"), VTXCUBETEX_DECLARATION::Element, VTXCUBETEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxModel */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Shader_VtxModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxModel.hlsl"), VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance*/
	if(FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1,TEXT("Prototype_Component_Shader_VtxPointInstance"),
		CShader::Create(m_pDevice,m_pContext,TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance.hlsl"),VTXPOINT_INSTANCE_DECLARATION::Element, VTXPOINT_INSTANCE_DECLARATION::iNumElement))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩 끝 "));

	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevel)
{
	CLoader*		pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	DeleteCriticalSection(&m_CriticalSection);
	CloseHandle(m_hThread);
}
