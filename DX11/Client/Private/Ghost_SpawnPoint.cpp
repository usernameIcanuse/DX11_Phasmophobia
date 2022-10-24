#include "stdafx.h"
#include "Ghost_SpawnPoint.h"
#include "Ghost_Status.h"
#include "GameInstance.h"
#include "Ghost.h"
#include "Door.h"
#include "HandPrint.h"
#include "TrailCam.h"

CGhost_SpawnPoint::CGhost_SpawnPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice,pContext)
{
}

CGhost_SpawnPoint::CGhost_SpawnPoint(const CGhost_SpawnPoint& rhs)
	:CGameObject(rhs)
{
}

HRESULT CGhost_SpawnPoint::Initialize_Prototype()
{
	if (__super::Initialize_Prototype())
		return E_FAIL;

	return S_OK;
}

HRESULT CGhost_SpawnPoint::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		if (FAILED(Load_Point((_tchar*)pArg)))
			return E_FAIL;

		if (FAILED(Setup_Ghost()))
			return E_FAIL;

		if (FAILED(Setup_GhostStatus()))
			return E_FAIL;
	}
	if (FAILED(Setup_Component()))
		return E_FAIL;


	if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_HandPrint"), TEXT("Prototype_GameObject_HandPrint"), (CGameObject**)&m_pHandPrint)))
		return E_FAIL;

	m_pHandPrint->Set_Enable(false);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(80, 120);


	m_iAreaDefaultTemperature = dis(gen) % 7 + 3;
	if (m_bFreeze && m_iAreaDefaultTemperature - 4 > 0)
	{
		m_iAreaDefaultTemperature -=7;
	}


	m_lAnswerFrequency = dis(gen);

	m_fHandPrintCoolTime = 30.f;
	m_fTrailCamCoolTime = 30.f;
	m_fDotsProjecterCoolTime = 30.f;

	GAMEINSTANCE->Add_EventObject(CGame_Manager::EVENT_GHOST, this);
	m_EventFunc = std::bind(&CGhost_SpawnPoint::Normal_Operation, std::placeholders::_1, std::placeholders::_2);

	return S_OK;
}

void CGhost_SpawnPoint::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	
}

void CGhost_SpawnPoint::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef _DEBUG
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
	//m_pRendererCom->Add_DebugRenderGroup(m_pAreaCom); 
	//m_pRendererCom->Add_DebugRenderGroup(m_pSpawnPointCom);

#endif
}

HRESULT CGhost_SpawnPoint::Render()
{

#ifdef _DEBUG
	if (m_fWhisperingTime > 0.f)
	{
		GAMEINSTANCE->Render_Font(TEXT("Font_Dream"), m_szWhispering, _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));

	}
#endif // _DEBUG
	return S_OK;
}

void CGhost_SpawnPoint::Set_Enable(_bool _bEnable)
{
	__super::Set_Enable(_bEnable);

	if(nullptr != m_pGhost)
		m_pGhost->Set_Enable(_bEnable);
	if(nullptr != m_pGhost)
		m_pGhost_Status->Set_Enable(_bEnable);
}

void CGhost_SpawnPoint::OnEventMessage(const _tchar* pMessage)
{
	if (0 == lstrcmp(TEXT("Normal_Operation"), pMessage))
	{

	}
	else if (0 == lstrcmp(TEXT("Event"), pMessage) || 0 == lstrcmp(TEXT("Event"), pMessage))
	{
		if (nullptr != m_pGhost)
		{
			m_pGhost->Move_To_SpawnPoint(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
		}
	}
}

void CGhost_SpawnPoint::Call_EventFunc(_float fTimeDelta)
{
	m_EventFunc(this, fTimeDelta);
}

void CGhost_SpawnPoint::Light_Attack(_float fTimeDelta)
{
}

void CGhost_SpawnPoint::Attack(_float fTimeDelta)
{
}

void CGhost_SpawnPoint::Normal_Operation(_float fTimeDelta)
{
	if (m_bIsInDots)
	{/*도트 쿨타임*/
		
		m_fDotsProjecterCoolTime -= fTimeDelta;
		
		if(true == m_bPlayDots)
		{
			m_fDotsProjecterTime -= fTimeDelta;
			if (0.f > m_fDotsProjecterTime)
			{
				m_fDotsProjecterTime = 0.5f;
				m_bPlayDots = false;

			}
			else if(0.f < m_fDotsProjecterTime)
			{
				if (nullptr != m_pGhost)
				{
					m_pGhost->DotsProjecter(fTimeDelta);
				}
			}
		}

	}

	m_fHandPrintCoolTime -= fTimeDelta;

	m_fTrailCamCoolTime -= fTimeDelta;
	if (0.f > m_fTrailCamCoolTime)
	{
		m_fTrailCamTime -= fTimeDelta;
	}

	_matrix matWorld = m_pTransformCom->Get_WorldMatrix();
	m_pAreaCom->Update(matWorld);
	m_pSpawnPointCom->Update(matWorld);

	m_iAreaTemperature = m_iAreaDefaultTemperature - rand() % 4;

#ifdef _DEBUG
	m_fWhisperingTime -= fTimeDelta;
#endif
	m_fWhisperCoolTime -= fTimeDelta;

	m_bIsInDots = false;
}

void CGhost_SpawnPoint::Add_Score(_int _iScoreIndex)
{
	if(nullptr !=m_pGhost_Status)
		m_pGhost_Status->Add_Score(_iScoreIndex);
}

_int CGhost_SpawnPoint::Get_Anger()
{
	if(nullptr != m_pGhost_Status)
		return m_pGhost_Status->m_iScore;

	return -1;
}

_uint CGhost_SpawnPoint::Get_EMFLevel()
{
	if(nullptr != m_pGhost_Status)
		return m_pGhost_Status->m_iEMF;

	return 0;
}

_int   CGhost_SpawnPoint::Get_SpawnPointTemperature()
{
	if (m_bCheckFreeze)
	{
		if (nullptr != m_pGhost_Status)
			m_pGhost_Status->Add_Score(CGhost_Status::FIND_EVIDENCE);
		m_bCheckFreeze = false;
	}
	return m_iAreaTemperature - 4;
}

void CGhost_SpawnPoint::Get_Answer(_long _lFrequency, _float& _fTime)
{
	if (_lFrequency == m_lAnswerFrequency || DBL_EPSILON >= _fTime)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(80, 120);

		_fTime = dis(gen) % 5 + 30;

		if (m_bSpiritBox)
		{
			GAMEINSTANCE->Broadcast_Message(CGame_Manager::EVENT_ITEM, TEXT("Answer"));
			m_lAnswerFrequency = dis(gen);
			GAMEINSTANCE->Add_Desc(CEvent_Manager::INTERACTION, 1.f);
			if (m_bCheckSpiritBox)
			{
				if (nullptr != m_pGhost_Status)
					m_pGhost_Status->Add_Score(CGhost_Status::FIND_EVIDENCE);
				m_bCheckSpiritBox = false;
			}
		}
		else
			GAMEINSTANCE->Broadcast_Message(CGame_Manager::EVENT_ITEM, TEXT("Not_Respone"));

	}
	
}

HRESULT CGhost_SpawnPoint::Setup_Component()
{
	/* For.Com_Sphere*/
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(50.f, 50.f, 50.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.pOwner = this;
	ColliderDesc.m_eObjID = COLLISION_TYPE::GHOST_AREA;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_Area"), (CComponent**)&m_pAreaCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Sphere*/
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(15.f, 15.f, 15.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);
	ColliderDesc.pOwner = this;
	ColliderDesc.m_eObjID = COLLISION_TYPE::GHOST_SPAWNPOINT;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SpawnPoint"), (CComponent**)&m_pSpawnPointCom, &ColliderDesc)))
		return E_FAIL;
#ifdef _DEBUG
	/* For.Com_Renderer*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;
#endif
	return S_OK;
}

HRESULT CGhost_SpawnPoint::Setup_Ghost()
{
	/*For. Ghost*/
	if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Ghost"), TEXT("Prototype_GameObject_Ghost"), (CGameObject**)&m_pGhost, this)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGhost_SpawnPoint::Setup_GhostStatus()
{
	/*For. Ghost_Status*/
	if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Ghost"), TEXT("Prototype_GameObject_Ghost_Status"), (CGameObject**)&m_pGhost_Status, this)))
		return E_FAIL;
	return S_OK;

}

HRESULT CGhost_SpawnPoint::Load_Point(const _tchar* pFilePath)
{
	vector<OBJ_DATA>  listData;

	_tchar FilePath[255];
	lstrcpy(FilePath, pFilePath);
	lstrcat(FilePath, TEXT("Ghost_SpawnPoint"));

	HANDLE hFile = CreateFile(FilePath,
		GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed to load file");
		return E_FAIL;
	}
	DWORD dwByteHouse = 0;
	OBJ_DATA  tDataObj;
	ZeroMemory(&tDataObj, sizeof(OBJ_DATA));
	while (true)
	{
		if (TRUE == ReadFile(hFile, &tDataObj, sizeof(OBJ_DATA), &dwByteHouse, nullptr))
		{
			if (0 == dwByteHouse)
			{
				break;
			}


			listData.push_back(tDataObj);
			

		}
	}
	CloseHandle(hFile);


	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 50);

	_int iSize = listData.size();
	_int iIndex = dis(gen) % iSize;

	OBJ_DATA tSelected = listData[iIndex];

	m_pTransformCom->Set_WorldMatrix(tSelected.matWorld);

	return S_OK;
}



void CGhost_SpawnPoint::On_Collision_Enter(CCollider* pCollider)
{
	if (COLLISION_TYPE::PLAYER == pCollider->Get_Type())
	{
		if (nullptr != m_pGhost_Status)
			m_pGhost_Status->Add_Score(CGhost_Status::PLAYER_IN_AREA);
	}
	else if (COLLISION_TYPE::ITEM == pCollider->Get_Type())
	{
		if (nullptr != m_pGhost_Status)
			m_pGhost_Status->Add_Score(CGhost_Status::ITEM_IN_AREA);
	}

	else if (COLLISION_TYPE::SPIRITBOX == pCollider->Get_Type())
	{
		if (nullptr != m_pGhost_Status)
			m_pGhost_Status->Add_Score(CGhost_Status::SPIRITBOX);
	}
	else if (COLLISION_TYPE::DOTSPROJECTER == pCollider->Get_Type())
	{
		if (nullptr != m_pGhost_Status)
			m_pGhost_Status->Add_Score(CGhost_Status::DOTSPROJECTER);
	}
	else if (COLLISION_TYPE::CAMERA == pCollider->Get_Type())
	{
		if (nullptr != m_pGhost_Status)
			m_pGhost_Status->Add_Score(CGhost_Status::CAMERA);
	}
	
}

void CGhost_SpawnPoint::On_Collision_Stay(CCollider* pCollider)
{
	if (COLLISION_TYPE::PLAYER == pCollider->Get_Type())
	{
		if (0.f > m_fWhisperCoolTime)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<int> dis(0, 100);

			_int	iValue = dis(gen);

			if (iValue > 98)
			{
#ifdef _DEBUG
				wsprintf(m_szWhispering, TEXT("유후"));
				m_fWhisperingTime = 2.f;
#endif
				m_fWhisperCoolTime = 100.f;
			}
		}
	}
	else if (COLLISION_TYPE::DOOR == pCollider->Get_Type())
	{
		if (0.f > m_fHandPrintCoolTime)
		{
			CDoor* pDoor = (CDoor*)pCollider->Get_Owner();

			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<int> dis(10, 70);

			_int	iValue = dis(gen);

			pDoor->Open_Door((_float)iValue);
			if (m_bHandPrint)
			{
				GAMEINSTANCE->Add_Desc(CEvent_Manager::INTERACTION, 1.f);

				m_bCheckHandPrint = true;//증거 한 번만 찾도록

				m_pHandPrint->Set_Enable(true);

				CTransform* pObjectTransform = (CTransform*)pDoor->Get_Component(CGameObject::m_pTransformTag);

				m_pHandPrint->Set_Position(pObjectTransform, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));

			}
			m_fHandPrintCoolTime = 20.f;
		}
	}

	else if (COLLISION_TYPE::DOTSPROJECTER == pCollider->Get_Type())
	{
		if (m_bDotsProjecter)
		{
			m_bIsInDots = true;
			if (0.f > m_fDotsProjecterCoolTime)
			{
				GAMEINSTANCE->Add_Desc(CEvent_Manager::INTERACTION, 1.f);

				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<int> dis(5, 90);

				if (nullptr != m_pGhost)
				{
					m_pGhost->m_pModelCom->Set_CurrentAnimation(1);
					m_pGhost->Move_To_SpawnPoint(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
				}
				m_fDotsProjecterCoolTime = dis(gen);
				m_bPlayDots = true;
			}
		}
	}

	else if (COLLISION_TYPE::ITEM_AREA == pCollider->Get_Type())
	{
		if (0.f > m_fTrailCamCoolTime)
		{

			if (0.f < m_fTrailCamTime)
			{
				CTrailCam* pTrailCam = (CTrailCam*)pCollider->Get_Owner();
				pTrailCam->Sensor_Activating(true);
				
			}
			else if (0.f > m_fTrailCamTime)
			{
				CTrailCam* pTrailCam = (CTrailCam*)pCollider->Get_Owner();
				pTrailCam->Sensor_Activating(false);
				GAMEINSTANCE->Add_Desc(CEvent_Manager::INTERACTION, 1.f);

				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<int> dis(10, 100);

				m_fTrailCamCoolTime = dis(gen);
				m_fTrailCamTime = dis(gen) % 10 + 3;
			}
		}
	}
}

void CGhost_SpawnPoint::On_Collision_Exit(CCollider* pCollider)
{
	if (COLLISION_TYPE::PLAYER == pCollider->Get_Type())
	{
		if (nullptr != m_pGhost_Status)
			m_pGhost_Status->Subtract_Score(CGhost_Status::PLAYER_IN_AREA);
	}
	else if (COLLISION_TYPE::ITEM == pCollider->Get_Type())
	{
		if (nullptr != m_pGhost_Status)
			m_pGhost_Status->Subtract_Score(CGhost_Status::ITEM_IN_AREA);
	}

	else if (COLLISION_TYPE::SPIRITBOX == pCollider->Get_Type())
	{
		if (nullptr != m_pGhost_Status)
			m_pGhost_Status->Subtract_Score(CGhost_Status::SPIRITBOX);
	}
	else if (COLLISION_TYPE::DOTSPROJECTER == pCollider->Get_Type())
	{
		if (nullptr != m_pGhost_Status)
			m_pGhost_Status->Subtract_Score(CGhost_Status::DOTSPROJECTER);
	}
	else if (COLLISION_TYPE::CAMERA == pCollider->Get_Type())
	{
		if (nullptr != m_pGhost_Status)
			m_pGhost_Status->Subtract_Score(CGhost_Status::CAMERA);
	}
}

CGhost_SpawnPoint* CGhost_SpawnPoint::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGhost_SpawnPoint* pInstance = new CGhost_SpawnPoint(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGhost_SpawnPoint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGhost_SpawnPoint::Clone(void* pArg)
{
	CGhost_SpawnPoint* pInstance = new CGhost_SpawnPoint(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGhost_SpawnPoint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhost_SpawnPoint::Free()
{
	__super::Free();

	Safe_Release(m_pSpawnPointCom);
	Safe_Release(m_pAreaCom);
#ifdef _DEBUG
	Safe_Release(m_pRendererCom);
#endif
}
