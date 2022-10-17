#include "stdafx.h"
#include "Ghost_SpawnPoint.h"
#include "Ghost_Status.h"
#include "GameInstance.h"
#include "Ghost.h"

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
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (pArg)
		m_pGhost = (CGhost*)pArg;

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(Setup_Component()))
		return E_FAIL;

	if (FAILED(Setup_GhostStatus()))
		return E_FAIL;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(80, 120);


	m_iAreaDefaultTemperature = dis(gen) % 7 + 3;
	if (m_bFreeze && m_iAreaDefaultTemperature - 4 > 0)
	{
		m_iAreaDefaultTemperature -=7;
	}


	m_lAnswerFrequency = dis(gen);

	return S_OK;
}

void CGhost_SpawnPoint::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	
	if (nullptr != m_pGhost)
	{
		CTransform* matGhostTransform = (CTransform*)m_pGhost->Get_Component(CGameObject::m_pTransformTag);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, matGhostTransform->Get_State(CTransform::STATE_TRANSLATION));
	}
	if (m_bIsInDots)
	{/*µµÆ® ÄðÅ¸ÀÓ*/
		m_fDotsProjecterTime -= fTimeDelta;
		if(0.f > m_fDotsProjecterCoolTime)
			m_fDotsProjecterCoolTime -= fTimeDelta;
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

void CGhost_SpawnPoint::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef _DEBUG
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
	m_pRendererCom->Add_DebugRenderGroup(m_pAreaCom); 
	m_pRendererCom->Add_DebugRenderGroup(m_pSpawnPointCom);

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

	m_pGhost_Status->Set_Enable(_bEnable);
}

void CGhost_SpawnPoint::DotsProjecter()
{
	if (0.f > m_fDotsProjecterCoolTime)
	{
		/*±Í½Å ·»´õ*/
		if (0.f > m_fDotsProjecterTime)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<int> dis(5, 30);

			m_fDotsProjecterCoolTime = dis(gen);
			m_fDotsProjecterTime = 1.f;

		}
	}
}

void CGhost_SpawnPoint::Add_Score(_int _iScoreIndex)
{
	m_pGhost_Status->Add_Score(_iScoreIndex);
}

_int CGhost_SpawnPoint::Get_Anger()
{
	return m_pGhost_Status->m_iScore;
}

_uint CGhost_SpawnPoint::Get_EMFLevel()
{
	return m_pGhost_Status->m_iEMF;
}

_int   CGhost_SpawnPoint::Get_SpawnPointTemperature()
{
	if (m_bCheckFreeze)
	{
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
			if (m_bCheckSpiritBox)
			{
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

HRESULT CGhost_SpawnPoint::Setup_GhostStatus()
{
	/*For. Ghost_Status*/
	if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Ghost_Status"), TEXT("Prototype_GameObject_Ghost_Status"), (CGameObject**)&m_pGhost_Status, this)))
		return E_FAIL;
	return S_OK;

}



void CGhost_SpawnPoint::On_Collision_Enter(CCollider* pCollider)
{
	if (COLLISION_TYPE::PLAYER == pCollider->Get_Type())
	{
		m_pGhost_Status->Add_Score(CGhost_Status::PLAYER_IN_AREA);
	}
	else if (COLLISION_TYPE::ITEM == pCollider->Get_Type())
	{
		m_pGhost_Status->Add_Score(CGhost_Status::ITEM_IN_AREA);
	}

	else if (COLLISION_TYPE::SPIRITBOX == pCollider->Get_Type())
	{
		m_pGhost_Status->Add_Score(CGhost_Status::SPIRITBOX);
	}
	else if (COLLISION_TYPE::DOTSPROJECTER == pCollider->Get_Type())
	{
		m_pGhost_Status->Add_Score(CGhost_Status::DOTSPROJECTER);
	}
	else if (COLLISION_TYPE::CAMERA == pCollider->Get_Type())
	{
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
				wsprintf(m_szWhispering, TEXT("À¯ÈÄ"));
				m_fWhisperingTime = 2.f;
#endif
				m_fWhisperCoolTime = 100.f;
			}
		}
	}
	else if (COLLISION_TYPE::DOTSPROJECTER == pCollider->Get_Type())
	{
		if (m_bDotsProjecter)
		{
			DotsProjecter();//ÀÏÁ¤ ½Ã°£µ¿¾È¸¸ ¿Ü°û¼± ·»´õ¸µ
			m_bIsInDots = true;
		}
	}

}

void CGhost_SpawnPoint::On_Collision_Exit(CCollider* pCollider)
{
	if (COLLISION_TYPE::PLAYER == pCollider->Get_Type())
	{
		m_pGhost_Status->Subtract_Score(CGhost_Status::PLAYER_IN_AREA);
	}
	else if (COLLISION_TYPE::ITEM == pCollider->Get_Type())
	{
		m_pGhost_Status->Subtract_Score(CGhost_Status::ITEM_IN_AREA);
	}

	else if (COLLISION_TYPE::SPIRITBOX == pCollider->Get_Type())
	{
		m_pGhost_Status->Subtract_Score(CGhost_Status::SPIRITBOX);
	}
	else if (COLLISION_TYPE::DOTSPROJECTER == pCollider->Get_Type())
	{
		m_pGhost_Status->Subtract_Score(CGhost_Status::DOTSPROJECTER);
	}
	else if (COLLISION_TYPE::CAMERA == pCollider->Get_Type())
	{
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
