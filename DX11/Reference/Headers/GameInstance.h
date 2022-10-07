#pragma once

#include "Graphic_Device.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Input_Manager.h"
#include "Timer_Manager.h"
#include "PipeLine.h"
#include "Math_Utility.h"
#include "Light_Manager.h"
#include "Frustum.h"
#include "Collision_Manager.h"
#include "Font_Manager.h"
#include "Game_Manager.h"
#include "Renderer_Manager.h"
#include "Target_Manager.h"

/* 1. 게임내에 필요한 객체(매니져등)들을 모아서 보관한다. */
/* 2. 클라이언트 개발자가 접근하기좋은 루트를 제공해준다. 나. */
/* 3. 진짜 필요한 함수만 클라잉언트개발자에게 ㅇ오픈해주낟. */

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
public:
	CGameInstance();
	virtual ~CGameInstance() = default;

public: /* 엔진 라이브럴를 사용할때 필요한 초기화 과정으르 거칠거야. */
	HRESULT Initialize_Engine(HINSTANCE hInst, _uint iNumLevels, const GRAPHICDESC& GraphicDesc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut);
	HRESULT Tick_Engine(_float fTimeDelta);
	HRESULT Render_Engine();
	HRESULT Clear(_uint iLevelID);

public: /* For.Graphic_Device */	
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_DepthStencil_View();
	HRESULT Present();
	GRAPHICDESC		Get_GraphicDesc();
	ID3D11Device* Get_Device();
	ID3D11DeviceContext* Get_Context();


public: /* For.Input_Manager*/
	_byte Get_DIMouseKeyState(MOUSEBUTTON eMouseButtonID);
	_long Get_DIMouseMoveState(MOUSEMOVE eMouseMove);
	bool Is_KeyState(KEY _Key, KEY_STATE _KeyState);

public: /* For.Level_Manager */
	HRESULT Open_Level(_uint iLevelID, class CLevel* pLevel);
	_uint	Get_Current_Level();
	void	Set_Current_Level(_uint _iCurrentLevelID);

public: /* For.Object_Manager */
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, CGameObject** pOut = nullptr,void* pArg = nullptr);
	class CComponent* Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex = 0);
	void	Clear_Layer(_uint iLevelIndex, const _tchar* pLayerTag);

public: /* For.Component_Mananger */
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);

	
public: /* For.Timer_Manager */
	HRESULT Add_Timer(const _tchar* pTimerTag);
	_float Compute_Timer(const _tchar* pTimerTag);

public: /* For.PipeLine*/
	void Set_Transform(CPipeLine::TRANSFORMSTATE eState, _fmatrix TransformState);
	_matrix Get_Transform(CPipeLine::TRANSFORMSTATE eState);
	const _float4x4* Get_Transform_float4x4(CPipeLine::TRANSFORMSTATE eState);
	_float4x4* Get_Transform_TP(CPipeLine::TRANSFORMSTATE eState);
	_float4 Get_CamPosition();

public: /* For.Light_Manager */
	HRESULT Add_Light(CLight* pLight);
	LIGHTDESC* Get_LightDesc(_uint iIndex);
	void	Clear_Light();

public:/* For.Frustum*/
	void Add_ItemFrustum(_uint eFrustumType, CRenderer* _pRenderer, CTransform* _pTransform);
	void Add_Object_For_Culling(CGameObject* pGameObject, CRenderer::RENDERGROUP eRenderGroup);
	_bool isIn_Frustum_InLocalSpace(_fvector vLocalPoint, _float fRange, CRenderer::RENDERGROUP eRenderGroup, CGameObject* pGameObject);
	void Transform_ToLocalSpace(_fmatrix WorldMatrixInv);

public:/* For.Collision_Manager*/
	void Add_Collider(CCollider* pCollider);
	void Erase_Collider(CCollider* pCollider);

public:/* For.Font_Manager*/
	HRESULT Add_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontTag, const _tchar* pFontFilePath);
	HRESULT Render_Font(const _tchar* pFontTag, const _tchar* pString, const _float2& vPosition, _fvector vColor);
	HRESULT Render_Font(const _tchar* pFontTag, const _tchar* pString, _fvector vPosition, _fvector vColor, float rotation, _fvector vOrigin, _vector vScale);
public:/*For.Game_Manager*/
	HRESULT Add_EventObject(_int iIndex, class CGameObject* pObject);
	void Broadcast_Message(_int iIndex, const _tchar* pMessage);
	void Clear_Layer(_int iIndex);
	void Clear_List();

public:/* For.Renderer_Manager*/
	HRESULT Add_Renderer(_uint eListIndex, CRenderer* pRenderer);
	HRESULT Draw_RenderGroup();
	void Clear_RendererIndex(_uint eListIndex);


private:
	CGraphic_Device*				m_pGraphic_Device = nullptr;
	CLevel_Manager*					m_pLevel_Manager = nullptr;
	CObject_Manager*				m_pObject_Manager = nullptr;
	CComponent_Manager*				m_pComponent_Manager = nullptr;
	CTimer_Manager*					m_pTimer_Manager = nullptr;
	CInput_Manager*					m_pInput_Manager = nullptr;
	CPipeLine*						m_pPipeLine = nullptr;
	CLight_Manager*					m_pLight_Manager = nullptr;
	CFrustum*						m_pFrustum = nullptr;
	CFont_Manager*					m_pFont_Manager = nullptr;
	CCollision_Manager*				m_pCollision_Manager = nullptr;
	CGame_Manager*					m_pGame_Manager = nullptr;
	CRenderer_Manager*				m_pRenderer_Manager = nullptr;
	CTarget_Manager*				m_pTarget_Manager = nullptr;


	GRAPHICDESC						m_tagGraphicDesc;

public:
	static void Release_Engine();
	virtual void Free() override;
};

END