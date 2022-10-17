#include "stdafx.h"
#include "Ghost.h"
#include "GameInstance.h"
#include "Ghost_SpawnPoint.h"
#include "Ghost_Status.h"
#include "HandPrint.h"
#include "Ghost_Behavior.h"
#include "Door.h"

CGhost::CGhost(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice,pContext)
{
}

CGhost::CGhost(const CGhost& rhs)
	:CGameObject(rhs)
{
}

HRESULT CGhost::Initialize_Prototype()
{
	if (__super::Initialize_Prototype())
		return E_FAIL;

	return S_OK;
}

HRESULT CGhost::Initialize(void* pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	
	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;
	
	if (FAILED(Setup_Component()))
		return E_FAIL;

	if (FAILED(Setup_SpawnPoint()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		CHARACTERDATA tagData = *(CHARACTERDATA*)pArg;
		m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&tagData.matWorld));

		if (FAILED(Setup_Bahavior(tagData.iCurrentIndex)))
			return E_FAIL;

		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_HandPrint"), TEXT("Prototype_GameObject_HandPrint"), (CGameObject**)&m_pHandPrint)))
			return E_FAIL;

		m_pHandPrint->Set_Enable(false);
	}
	m_pModelCom->Set_CurrentAnimation(1);


	GAMEINSTANCE->Add_EventObject(CGame_Manager::EVENT_GHOST, this);
	GAMEINSTANCE->Broadcast_Message(CGame_Manager::EVENT_GHOST, TEXT("Normal_Operation"));
	m_fUpdatePointTime = 5.f;

	return S_OK;
}

void CGhost::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fTime += fTimeDelta;
	m_fUpdatePointTime -= fTimeDelta;

	

	if (0.f > m_fUpdatePointTime)
	{
		Stop_Updating_SpawnPoint();
	}
	_matrix matWorld = m_pTransformCom->Get_WorldMatrix();

	

	m_pOBBCom->Update(matWorld);
	m_pGhostCom->Update(matWorld);


}

void CGhost::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


#ifdef _DEBUG
	//m_pRendererCom->Add_DebugRenderGroup(m_pOBBCom);
	//m_pRendererCom->Add_DebugRenderGroup(m_pGhostCom);


#endif // _DEBUG
}

HRESULT CGhost::Render()
{

	_uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		_int	iPassIndex = 1;

		if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
		{
			iPassIndex = 0;
		}



		m_pModelCom->Render(i, m_pShaderCom, iPassIndex, "g_Bones");
	}

#ifdef _DEBUG
	GAMEINSTANCE->Render_Font(TEXT("Font_Dream"), m_szEvent, _float2(0.f, 200.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
#endif

	return S_OK;
}

HRESULT CGhost::SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix)
{
	if (nullptr == m_pShaderCom ||
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


void CGhost::Set_Enable(_bool _bEnable)
{
	__super::Set_Enable(_bEnable);

	m_pSpawnPoint->Set_Enable(_bEnable);
}

void CGhost::OnEventMessage(const _tchar* pMessage)
{
	if (0 == lstrcmp(TEXT("Event"), pMessage))
	{
		_vector vSpawnPos = m_pSpawnPoint->Get_SpawnPoint();
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vSpawnPos);
		
		m_fEventTime = 10.f + m_pSpawnPoint->Get_Anger() / 6;
		m_EventFunc = std::bind(&CGhost::Light_Attack, std::placeholders::_1, std::placeholders::_2);
	
		m_pModelCom->Set_CurrentAnimation(0);

	}
	 
	else if (0 == lstrcmp(TEXT("Attack"), pMessage))
	{
		_vector vSpawnPos = m_pSpawnPoint->Get_SpawnPoint();
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vSpawnPos);

		m_pOBBCom->Set_Enable(true);
		m_fAttackTime = 10.f + m_pSpawnPoint->Get_Anger() / 5;
		m_EventFunc = std::bind(&CGhost::Attack, std::placeholders::_1, std::placeholders::_2);
	
		m_pModelCom->Set_CurrentAnimation(0);
	}
	else if (0 == lstrcmp(TEXT("Normal_Operation"), pMessage))
	{
		m_EventFunc = std::bind(&CGhost::Normal_Operation, std::placeholders::_1, std::placeholders::_2);
		m_pModelCom->Set_CurrentAnimation(1);
		m_fEventTime = m_fAttackTime = 0.f;
	}

}

void CGhost::Call_EventFunc(_float fTimeDelta)
{
	m_EventFunc(this,fTimeDelta);
}

_bool CGhost::Check_GhostWriting()
{
	if (m_bCheckGhostWriting)
	{
		m_pSpawnPoint->Add_Score(CGhost_Status::FIND_EVIDENCE);
		m_bCheckGhostWriting = false;
	}


	return m_bGhostWriting;
}


void CGhost::Stop_Updating_SpawnPoint()
{
	m_pSpawnPoint->Set_Ghost(nullptr);
	m_pBehavior->Setup_SpawnPointIndex();
}

void CGhost::Light_Attack(_float fTimeDelta)
{
	GAMEINSTANCE->Add_Object_For_Culling(this, CRenderer::RENDER_NONALPHABLEND);
	m_pModelCom->Play_Animation(fTimeDelta);

	/*ºÒºû ±ôºý°Å¸², ±Í½Å ¸ðµ¨ ·»´õ¸µ, ÀüÀÚ Àåºñµé °íÀå*/
#ifdef _DEBUG
	wsprintf(m_szEvent, TEXT("±ð²á~"));
#endif
	m_fEventTime -= fTimeDelta;
	if (0.f > m_fEventTime)
	{
		GAMEINSTANCE->Broadcast_Message(CGame_Manager::EVENT_ITEM, TEXT("Normal_Operation"));
		GAMEINSTANCE->Broadcast_Message(CGame_Manager::EVENT_GHOST, TEXT("Normal_Operation"));
	}
	
}

void CGhost::Attack(_float fTimeDelta)
{
	/*Ãâ±¸ ´ÝÈû&Àá±è, ±Í½Å attack collider set enable, ÀüÀÚ Àåºñµé °íÀå*/

	GAMEINSTANCE->Add_Object_For_Culling(this, CRenderer::RENDER_NONALPHABLEND);
#ifdef _DEBUG
	wsprintf(m_szEvent, TEXT("µ¼È²Ã­"));
#endif
	m_fIdleTime -= fTimeDelta;
	if (0.f > m_fIdleTime)
	{
		m_pModelCom->Set_CurrentAnimation(1);
	}
	else
		m_pModelCom->Set_CurrentAnimation(0);

	m_pModelCom->Play_Animation(fTimeDelta*1.3f);

	m_fAttackTime -= fTimeDelta;
	if (0.f > m_fAttackTime)
	{
		m_pOBBCom->Set_Enable(false);
		GAMEINSTANCE->Broadcast_Message(CGame_Manager::EVENT_ITEM, TEXT("Normal_Operation"));
		GAMEINSTANCE->Broadcast_Message(CGame_Manager::EVENT_GHOST, TEXT("Normal_Operation"));
	}
}

void CGhost::Normal_Operation(_float fTimeDelta)
{
	GAMEINSTANCE->Add_Object_For_Culling(this, CRenderer::RENDER_NONALPHABLEND);
	m_pModelCom->Play_Animation(fTimeDelta);

	_vector vSpawnPos = m_pSpawnPoint->Get_SpawnPoint();
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

	if (35.f < XMVectorGetX(XMVector3Length(vSpawnPos - vPosition)))
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vSpawnPos);
		m_pBehavior->Move_To_SpawnPoint();
	}

	if (m_bInDots)
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vSpawnPos);
		m_pBehavior->Move_To_SpawnPoint();

		m_fDotsTime -= fTimeDelta;
		m_pModelCom->Play_Animation(fTimeDelta*2.f);
		GAMEINSTANCE->Add_Object_For_Culling(this, CRenderer::RENDER_NONALPHABLEND);
		if (0.f > m_fDotsTime)
		{
			m_bInDots = false;
		}

	}

	if (m_bHandPrint)
	{
		m_fHandPrintCoolTime -= fTimeDelta;
	}

}


HRESULT CGhost::Setup_Component()
{
	/* For.Com_OBB*/
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(4.f, 10.f, 4.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);
	ColliderDesc.pOwner = this;
	ColliderDesc.m_eObjID = COLLISION_TYPE::GHOST_ATTACK;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	m_pOBBCom->Set_Enable(false);

	/* For.Com_Ghost*/
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(3.f, 10.f, 3.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);
	ColliderDesc.pOwner = this;
	ColliderDesc.m_eObjID = COLLISION_TYPE::GHOST;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_Ghost"), (CComponent**)&m_pGhostCom, &ColliderDesc)))
		return E_FAIL;

	///* For.Com_Renderer*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Ghost*/
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Ghost_Girl"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_Shader*/
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CGhost::Setup_SpawnPoint()
{
	if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Ghost_SpawnPoint"), TEXT("Prototype_GameObject_Ghost_SpawnPoint"), (CGameObject**)&m_pSpawnPoint, this)))
		return E_FAIL;
	return S_OK;
}

HRESULT CGhost::Setup_Bahavior(_int iNaviIndex)
{
	
	if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Ghost"), TEXT("Prototype_GameObject_Ghost_Behavior"), (CGameObject**)&m_pBehavior, &iNaviIndex)))
		return E_FAIL;

	m_pBehavior->Set_Owner(m_pTransformCom);

	return S_OK;
}/**/

void CGhost::On_Collision_Enter(CCollider* pCollider)
{
	if (COLLISION_TYPE::DOOR == pCollider->Get_Type())
	{ 
		if (0.f > m_fHandPrintCoolTime)
		{
			CDoor* pDoor = (CDoor*)pCollider->Get_Owner();

			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<int> dis(30, 70);

			_int	iValue = dis(gen);

			pDoor->Open_Door((_float)iValue);
			if (m_bHandPrint)
			{
				m_bCheckHandPrint = true;//Áõ°Å ÇÑ ¹ø¸¸ Ã£µµ·Ï

				m_pHandPrint->Set_Enable(true);				

				CTransform* pObjectTransform = (CTransform*)pDoor->Get_Component(CGameObject::m_pTransformTag);

				m_pHandPrint->Set_Position(pObjectTransform, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));

			}
			m_fHandPrintCoolTime = 20.f;
		}
	}

	if (COLLISION_TYPE::DOTSPROJECTER == pCollider->Get_Type())
	{
		if (m_bDotsProjecter)
		{
			m_bInDots = true;
			m_fDotsTime = 1.f;
		}
	}
}

void CGhost::On_Collision_Stay(CCollider* pCollider)
{
	
}

void CGhost::On_Collision_Exit(CCollider* pCollider)
{
}

CGhost* CGhost::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGhost* pInstance = new CGhost(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGhost");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGhost::Clone(void* pArg)
{
	CGhost* pInstance = new CGhost(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGhost");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhost::Free()
{
	__super::Free();

	Safe_Release(m_pOBBCom);
	Safe_Release(m_pGhostCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);

}
