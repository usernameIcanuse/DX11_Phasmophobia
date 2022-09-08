#include "stdafx.h"
#include "ImguiMgr.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "GameObject.h"
#include "House.h"


IMPLEMENT_SINGLETON(CImguiMgr)

CImguiMgr::CImguiMgr()
{
}

void CImguiMgr::Free()
{

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();


}

void CImguiMgr::Init(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	m_pDevice = _pDevice;
	Safe_AddRef(m_pDevice);
	m_pContext = _pContext;
	Safe_AddRef(m_pContext);

	m_vSelectedOffSet = XMVectorSet(0.f, 0.f, 0.f, 0.f);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	

	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(m_pDevice, m_pContext);
}

void CImguiMgr::Tick(_float fTimeDelta)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::Checkbox("Map Tool", &show_Map_Tool);
		ImGui::Checkbox("Object Tool", &show_Object_Tool);
		ImGui::Checkbox("Collider Tool", &show_Collider_Tool);


		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	if (CURRENT_LEVEL == LEVEL_STAGE1)
	{
		Set_Prototype();
		if (show_Map_Tool && !show_Object_Tool && !show_Collider_Tool)
			Tool_Map();
		else if (!show_Map_Tool && show_Object_Tool && !show_Collider_Tool)
			Tool_Object();
		else if (!show_Map_Tool && !show_Object_Tool && show_Collider_Tool)
			Tool_Collider();

		Picking_Object();
	}
}

void CImguiMgr::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


}

void CImguiMgr::Set_Prototype()
{
	static _bool	bFirst = false;
	if (!bFirst)
	{
		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_Player"), &m_pPlayer)))
			return;

		m_pRayCom = (CCollider*)m_pPlayer->Get_Component(TEXT("Com_Ray"));
		
		bFirst = true;
		m_iSelectedIndex = -1;

		CGameObject* pTerrain = nullptr;
		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Terrain"), TEXT("Prototype_GameObject_Terrain"), &pTerrain)))
			return;

		m_pTerrainTransform = (CTransform*)pTerrain->Get_Component(CGameObject::m_pTransformTag);
		m_pTerrainVIBuffer = (CVIBuffer_Terrain*)pTerrain->Get_Component(TEXT("Com_VIBuffer"));
		
		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_Collider"), &m_ColliderPrototype)))
			return;
		m_ColliderPrototype->Set_Enable(false);

		
		/* Object */
		CGameObject* pTemp = nullptr;
		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_DotsProjecter"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		m_vecPrototypeObject.push_back(pTemp);

		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_FlashLight"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		m_vecPrototypeObject.push_back(pTemp);

		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_Thermometer"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		m_vecPrototypeObject.push_back(pTemp);

		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_EMF"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		m_vecPrototypeObject.push_back(pTemp);

		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_Note"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		m_vecPrototypeObject.push_back(pTemp);
		
		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_SpiritBox"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		m_vecPrototypeObject.push_back(pTemp);
		
		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_Video_Camera"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		m_vecPrototypeObject.push_back(pTemp);

		/* House */
		/*if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_House"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_Truck"));
		m_vecPrototypeHouse.push_back(pTemp);

		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_House"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_FurnishedCabin"));
		m_vecPrototypeHouse.push_back(pTemp);*/

		//if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_House"), &pTemp)))
		//	return;
		//pTemp->Set_Enable(false);
		//static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_Garage"));
		//m_vecPrototypeHouse.push_back(pTemp);

		/*if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_House"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_Pier_house"));
		m_vecPrototypeHouse.push_back(pTemp);

		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_House"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_Pier_house2"));
		m_vecPrototypeHouse.push_back(pTemp);

		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_House"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop"));
		m_vecPrototypeHouse.push_back(pTemp);

		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_House"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background1"));
		m_vecPrototypeHouse.push_back(pTemp);

		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_House"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background2"));
		m_vecPrototypeHouse.push_back(pTemp);

		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_House"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background3"));
		m_vecPrototypeHouse.push_back(pTemp);

		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_House"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background4"));
		m_vecPrototypeHouse.push_back(pTemp);

		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_House"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background5"));
		m_vecPrototypeHouse.push_back(pTemp);

		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_House"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background6"));
		m_vecPrototypeHouse.push_back(pTemp);

		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_House"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background7"));
		m_vecPrototypeHouse.push_back(pTemp);

		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_House"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background8"));
		m_vecPrototypeHouse.push_back(pTemp);

		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_House"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background9"));
		m_vecPrototypeHouse.push_back(pTemp);

		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_House"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background10"));
		m_vecPrototypeHouse.push_back(pTemp);

		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_House"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background11"));
		m_vecPrototypeHouse.push_back(pTemp);

		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_House"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background12"));
		m_vecPrototypeHouse.push_back(pTemp);

		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_House"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background13"));
		m_vecPrototypeHouse.push_back(pTemp);

		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_House"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background14"));
		m_vecPrototypeHouse.push_back(pTemp);

		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_House"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_SlumHouse1"));
		m_vecPrototypeHouse.push_back(pTemp);

		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_House"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_OldHouse"));
		m_vecPrototypeHouse.push_back(pTemp);

		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_House"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_Shelter"));
		m_vecPrototypeHouse.push_back(pTemp);*/


	}
}


void CImguiMgr::Tool_Map()
{
	ImGui::Begin("Tool_Map");

	static int item_current_idx = -1; // Here we store our selection data as an index.

	if (ImGui::Button("Close Me"))
	{
		show_Map_Tool = false;
		m_iSelectedIndex = item_current_idx = -1;
		m_pSelectedObject = nullptr;
		m_pSelectedTransform = nullptr;
	}
	/* Save/Load Map*/
	static char Stage[256] = "";
	ImGui::InputText("Stage Name", Stage, IM_ARRAYSIZE(Stage));

	static char str0[256] = "";
	ImGui::InputText("File Name", str0, IM_ARRAYSIZE(str0));
	
	
	if (ImGui::Button("Save"))                            
	{
		Save_Map(Stage,str0);
	}
	
	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		Load_Map(Stage,str0);
	}

	if (ImGui::Button("Clear"))
	{
		GAMEINSTANCE->Clear_Layer(LEVEL_STAGE1,TEXT("Layer_House"));
		for(int i=0;i<(_uint)MODEL_TAG::MODEL_END;++i)
			m_vecCollocatedHouse[i].clear();

	}


	const char* items[] = { "Truck","FurnishedCabin",/*"Garage",*/
							"Pier_house","Pier_house2","RoofTop","RoofTop_Background1",
							"RoofTop_Background2","RoofTop_Background3",
		"RoofTop_Background4","RoofTop_Background5","RoofTop_Background6",
		"RoofTop_Background7","RoofTop_Background8","RoofTop_Background9",
		"RoofTop_Background10","RoofTop_Background11","RoofTop_Background12",
		"RoofTop_Background13","RoofTop_Background14","SlumHouse1",
							"OldHouse","Shelter"};


	if (GAMEINSTANCE->Is_KeyState(KEY::DELETEKEY, KEY_STATE::TAP))
	{
		m_iSelectedIndex = item_current_idx = -1; 
		m_pSelectedObject =  nullptr;
		m_pSelectedTransform = nullptr;
	}

	if (ImGui::BeginListBox("House"))
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			m_vecPrototypeHouse[n]->Set_Enable(false);
			const bool is_selected = (item_current_idx == n);
			if (ImGui::Selectable(items[n], is_selected))
				m_iSelectedIndex = item_current_idx = n;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
				
			}
		}

		ImGui::EndListBox();
	}

	if (-1 < m_iSelectedIndex)
	{
		m_pSelectedObject = m_vecPrototypeHouse[m_iSelectedIndex];
		m_pSelectedObject->Set_Enable(true);
		m_pSelectedTransform = (CTransform*)m_pSelectedObject->Get_Component(CGameObject::m_pTransformTag);

	}

	Translation();
	Rotation();
	Scaling();

	ImGui::End();

}

void CImguiMgr::Tool_Object()
{
	ImGui::Begin("Tool_Object");

	static int Object_current_idx = -1; // Here we store our selection data as an index.

	if (ImGui::Button("Close Me"))
	{
		show_Object_Tool = false;
		m_iSelectedIndex = Object_current_idx = -1;
		m_pSelectedObject = nullptr;
		m_pSelectedTransform = nullptr;
	}
	/* Save/Load Map*/
	static char Stage[256] = "";
	ImGui::InputText("Stage Name", Stage, IM_ARRAYSIZE(Stage));

	static char str0[256] = "";
	ImGui::InputText("File Name", str0, IM_ARRAYSIZE(str0));


	if (ImGui::Button("Save"))
	{
		Save_Object(Stage, str0);
	}

	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		Load_Object(Stage, str0);
	}

	if (ImGui::Button("Clear"))
	{
		GAMEINSTANCE->Clear_Layer(LEVEL_STAGE1, TEXT("Layer_Object"));//일단 맵 만들 때는 레이어 하나로만
		for (int i = 0; i < (_uint)LAYER::LAYER_END; ++i)
		{
			m_vecObjectTag[i].clear();
			m_vecCollocatedObject[i].clear();
		}

	}


	const char* items[] = {"DotsProjecter", "FlashLight", "Thermometer","EMF",
							"Note","SpiritBox","Video Camera"};


	if (GAMEINSTANCE->Is_KeyState(KEY::DELETEKEY, KEY_STATE::TAP))
	{
		m_iSelectedIndex = Object_current_idx = -1;
		m_pSelectedObject = nullptr;
		m_pSelectedTransform = nullptr;
	}

	if (ImGui::BeginListBox("Object"))
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			m_vecPrototypeObject[n]->Set_Enable(false);
			const bool is_selected = (Object_current_idx == n);
			if (ImGui::Selectable(items[n], is_selected))
				m_iSelectedIndex = Object_current_idx = n;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();

			}
		}

		ImGui::EndListBox();
	}

	if (-1 < m_iSelectedIndex)
	{
		m_pSelectedObject = m_vecPrototypeObject[m_iSelectedIndex];
		m_pSelectedObject->Set_Enable(true);
		m_pSelectedTransform = (CTransform*)m_pSelectedObject->Get_Component(CGameObject::m_pTransformTag);
	}

	Translation();
	Rotation();
	Scaling();

	ImGui::End();
}

void CImguiMgr::Tool_Collider()
{
	ImGui::Begin("Tool_Collider");

	if (ImGui::Button("Close Me"))
	{
		show_Collider_Tool = false;
		m_ColliderPrototype->Set_Enable(false);
		m_pSelectedObject = nullptr;
		m_pSelectedTransform = nullptr;
	}
	/* Save/Load Map*/
	static char Stage[256] = "";
	ImGui::InputText("Stage Name", Stage, IM_ARRAYSIZE(Stage));

	static char str0[256] = "";
	ImGui::InputText("File Name", str0, IM_ARRAYSIZE(str0));


	if (ImGui::Button("Save"))
	{
		Save_Collider(Stage, str0);
	}

	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		Load_Collider(Stage, str0);
	}

	if (ImGui::Button("Clear"))
	{
		GAMEINSTANCE->Clear_Layer(LEVEL_STAGE1, TEXT("Layer_Collider"));//일단 맵 만들 때는 레이어 하나로만
		m_vecCollider.clear();

	}

	if (GAMEINSTANCE->Is_KeyState(KEY::DELETEKEY, KEY_STATE::TAP))
	{
		m_pSelectedTransform->Set_State(CTransform::STATE_RIGHT, XMVectorSet(1.f,0.f,0.f,0.f));
		m_pSelectedTransform->Set_State(CTransform::STATE_UP, XMVectorSet(0.f, 1.f, 0.f, 0.f));
		m_pSelectedTransform->Set_State(CTransform::STATE_LOOK, XMVectorSet(0.f, 0.f, 1.f, 0.f));
		m_pSelectedTransform->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));

		m_ColliderPrototype->Set_Enable(false);
		m_pSelectedObject = nullptr;
		m_pSelectedTransform = nullptr;
	}

	if (ImGui::Button("Collider"))
	{
		m_ColliderPrototype->Set_Enable(true);
		m_pSelectedObject = m_ColliderPrototype;
		m_pSelectedTransform = (CTransform*)m_ColliderPrototype->Get_Component(CGameObject::m_pTransformTag);

	}


	Translation();
	Rotation();

	static float Scale[4] = { 1.f, 1.f, 1.f, 0.44f };
	ImGui::InputFloat3("input float3", Scale);

	if (m_pSelectedObject)
	{
		m_pSelectedTransform->Set_Scaled(_float3(Scale[0], Scale[1], Scale[2]));
	}

	ImGui::End();

}

void CImguiMgr::Picking_Object()
{
	
	_float4 fPosition;
	if (-1 < m_iSelectedIndex || m_pSelectedObject)
	{
		if (CMath_Utility::Picking(m_pTerrainVIBuffer, m_pTerrainTransform, &fPosition));
		{
			//_float3 fScale = m_pSelectedTransform->Get_Scaled();
			
			
			XMStoreFloat4(&fPosition, XMLoadFloat4(&fPosition) + m_vSelectedOffSet);

			
			for (int i = 0; i < (_uint)LAYER::LAYER_END; ++i)
			{
				for (auto& elem : m_vecCollocatedObject[i])
				{
					CCollider* pObjCollider =   (CCollider*)elem->Get_Component(TEXT("Com_AABB"));
					if (m_pRayCom->Collision(pObjCollider))
					{

						XMStoreFloat4(&fPosition, XMVectorSetW(XMLoadFloat3( &m_pRayCom->Get_CollidePos()),1.f));

					}
				}
			}
			m_pSelectedTransform->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&fPosition));

			if (show_Map_Tool && !show_Object_Tool && !show_Collider_Tool)
				CollocateHouse();
			else if (!show_Map_Tool && show_Object_Tool && !show_Collider_Tool)
			{
				if (-1 < m_iSelectedIndex)
					CollocateObject();
				else
					MoveObject(fPosition);
			}
			else if (!show_Map_Tool && !show_Object_Tool && show_Collider_Tool)
				CollocateCollider();
		}

	}
	
	else
	{
		CCollider* pCollider = nullptr;
		if (!show_Map_Tool && !show_Object_Tool && show_Collider_Tool)
		{
			if (GAMEINSTANCE->Is_KeyState(KEY::LBUTTON, KEY_STATE::TAP))
			{
				_float fDist = -1.f;
				RAY		tRay = CMath_Utility::Get_MouseRayInWorldSpace();
				for (int i = 0; i < (_uint)LAYER::LAYER_END; ++i)
				{
					for (auto& elem : m_vecCollocatedObject[i])
					{
						pCollider = (CCollider*)elem->Get_Component(TEXT("Com_OBB"));
						if (pCollider->Collision(tRay, fDist))
						{

							m_pSelectedObject = elem;
							m_pSelectedTransform = (CTransform*)elem->Get_Component(CGameObject::m_pTransformTag);

						}
					}
				}
			}
		}
	}
}
void CImguiMgr::Translation()
{
	ImGui::Text("[ P ] : y+");
	ImGui::Text("[ O ] : y-");

	if (m_pSelectedObject)
	{
	
		if (GAMEINSTANCE->Is_KeyState(KEY::P, KEY_STATE::TAP))
		{
			m_vSelectedOffSet += XMVectorSet(0.f,0.05f,0.f,0.f);
		}
		else if (GAMEINSTANCE->Is_KeyState(KEY::O, KEY_STATE::TAP))
		{
			m_vSelectedOffSet -= XMVectorSet(0.f, 0.05f, 0.f, 0.f);
		}
	
	}
}

void CImguiMgr::Rotation()
{ // Sliders
	static ImGuiSliderFlags flags_i = ImGuiSliderFlags_None;

	static int slider_i = 0;
	
	ImGui::Text("Underlying int value: %d", slider_i);
	ImGui::SliderInt("Rotation", &slider_i, 0, 360, "%d", flags_i);
	
	if (m_pSelectedObject)
	{
		m_pSelectedTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(slider_i));
	}
}

void CImguiMgr::Scaling()
{
	static ImGuiSliderFlags flags_f = ImGuiSliderFlags_None;

	static float slider_f = 1.f;

	ImGui::Text("Underlying float value: %f", slider_f);
	ImGui::SliderFloat("Scaling", &slider_f, 0.5f, 10.0f, "%.3f", flags_f);
	
	if (m_pSelectedObject)
	{
		m_pSelectedTransform->Set_Scaled(_float3(slider_f, slider_f, slider_f));
	}
}

void CImguiMgr::MoveObject(_float4 _fPosition)
{
	if (GAMEINSTANCE->Is_KeyState(KEY::LBUTTON, KEY_STATE::TAP))
	{
		m_pSelectedObject = nullptr;
		m_pSelectedTransform = nullptr;
	}
}
void CImguiMgr::CollocateHouse()
{
	if (GAMEINSTANCE->Is_KeyState(KEY::SPACE, KEY_STATE::TAP))
	{
	
		CGameObject* pTemp = nullptr;
		MODEL_TAG	tIndex = MODEL_TAG::MODEL_END;
	
		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_House"), TEXT("Prototype_GameObject_House"), &pTemp)))
			return;

		switch (m_iSelectedIndex)
		{
		
		case 0:
			static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_Truck"));
			tIndex = MODEL_TAG::TRUCK;
			break;
		case 1:
			static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_FurnishedCabin"));
			tIndex = MODEL_TAG::FURNISHEDCABIN;
			break;
		case 2:
			static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_Garage"));
			tIndex = MODEL_TAG::GARAGE;
			break;
		case 3:
			static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_Pier_house"));
			tIndex = MODEL_TAG::PIER_HOUSE;
			break;
		case 4:
			static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_Pier_house2"));
			tIndex = MODEL_TAG::PIER_HOUSE2;
			break;
		case 5:
			static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop"));
			tIndex = MODEL_TAG::ROOFTOP;
			break;
		case 6:
			static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background1"));
			tIndex = MODEL_TAG::ROOFTOP_BACKGROUND1;
			break;

		case 7:
			static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background2"));
			tIndex = MODEL_TAG::ROOFTOP_BACKGROUND2;
			break;

		case 8:
			static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background3"));
			tIndex = MODEL_TAG::ROOFTOP_BACKGROUND3;
			break;

		case 9:
			static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background4"));
			tIndex = MODEL_TAG::ROOFTOP_BACKGROUND4;
			break;

		case 10:
			static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background5"));
			tIndex = MODEL_TAG::ROOFTOP_BACKGROUND5;
			break;

		case 11:
			static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background6"));
			tIndex = MODEL_TAG::ROOFTOP_BACKGROUND6;
			break;

		case 12:
			static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background7"));
			tIndex = MODEL_TAG::ROOFTOP_BACKGROUND7;
			break;

		case 13:
			static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background8"));
			tIndex = MODEL_TAG::ROOFTOP_BACKGROUND8;
			break;

		case 14:
			static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background9"));
			tIndex = MODEL_TAG::ROOFTOP_BACKGROUND9;
			break;

		case 15:
			static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background10"));
			tIndex = MODEL_TAG::ROOFTOP_BACKGROUND10;
			break;

		case 16:
			static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background11"));
			tIndex = MODEL_TAG::ROOFTOP_BACKGROUND11;
			break;

		case 17:
			static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background12"));
			tIndex = MODEL_TAG::ROOFTOP_BACKGROUND12;
			break;

		case 18:
			static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background13"));
			tIndex = MODEL_TAG::ROOFTOP_BACKGROUND13;
			break;

		case 19:
			static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background14"));
			tIndex = MODEL_TAG::ROOFTOP_BACKGROUND14;
			break;
		case 20:
			static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_SlumHouse1"));
			tIndex = MODEL_TAG::SLUMHOUSE1;
			break;
		case 21:
			static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_OldHouse"));
			tIndex = MODEL_TAG::OLDHOUSE;
			break;
		case 22:
			static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_Shelter"));
			tIndex = MODEL_TAG::SHELTER;
			break;
		}

		CTransform* pTempTransform = (CTransform*)pTemp->Get_Component(CGameObject::m_pTransformTag);
		
		pTempTransform->Set_State(CTransform::STATE_RIGHT,m_pSelectedTransform->Get_State(CTransform::STATE_RIGHT));
		pTempTransform->Set_State(CTransform::STATE_UP, m_pSelectedTransform->Get_State(CTransform::STATE_UP));
		pTempTransform->Set_State(CTransform::STATE_LOOK, m_pSelectedTransform->Get_State(CTransform::STATE_LOOK));
		pTempTransform->Set_State(CTransform::STATE_TRANSLATION,m_pSelectedTransform->Get_State(CTransform::STATE_TRANSLATION));

		m_vecCollocatedHouse[(_uint)tIndex].push_back(pTemp);

	}
}

void CImguiMgr::CollocateObject()
{
	if (GAMEINSTANCE->Is_KeyState(KEY::SPACE, KEY_STATE::TAP))
	{

		CGameObject* pTemp = nullptr;
		LAYER tLayerIndex = LAYER::LAYER_END;
		OBJ_TAG tObjIndex = OBJ_TAG::OBJ_END;

		switch (m_iSelectedIndex)//오브젝트
		{
		case 0:
			tLayerIndex = LAYER::OBJECT;
			tObjIndex = OBJ_TAG::DOTSPROJECTER;
			if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Object"), TEXT("Prototype_GameObject_DotsProjecter"), &pTemp)))
				return;
			break;

		case 1:
			tLayerIndex = LAYER::OBJECT;
			tObjIndex = OBJ_TAG::FLASHLIGHT;
			if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Object"), TEXT("Prototype_GameObject_FlashLight"), &pTemp)))
				return;
			break;

		case 2:
			tLayerIndex = LAYER::OBJECT;
			tObjIndex = OBJ_TAG::THERMOMETER;
			if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Object"), TEXT("Prototype_GameObject_Thermometer"), &pTemp)))
				return;
			break;

		case 3:
			tLayerIndex = LAYER::OBJECT;
			tObjIndex = OBJ_TAG::EMF;
			if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Object"), TEXT("Prototype_GameObject_EMF"), &pTemp)))
				return;
			break;

		case 4:
			tLayerIndex = LAYER::OBJECT;
			tObjIndex = OBJ_TAG::NOTE;
			if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Object"), TEXT("Prototype_GameObject_Note"), &pTemp)))
				return;
			break;

		case 5:
			tLayerIndex = LAYER::OBJECT;
			tObjIndex = OBJ_TAG::SPIRITBOX;
			if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Object"), TEXT("Prototype_GameObject_SpiritBox"), &pTemp)))
				return;
			break;

		case 6:
			tLayerIndex = LAYER::OBJECT;
			tObjIndex = OBJ_TAG::VIDEOCAMERA;
			if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Object"), TEXT("Prototype_GameObject_Video_Camera"), &pTemp)))
				return;
			break;
		}

		CTransform* pTempTransform = (CTransform*)pTemp->Get_Component(CGameObject::m_pTransformTag);

		pTempTransform->Set_State(CTransform::STATE_RIGHT, m_pSelectedTransform->Get_State(CTransform::STATE_RIGHT));
		pTempTransform->Set_State(CTransform::STATE_UP, m_pSelectedTransform->Get_State(CTransform::STATE_UP));
		pTempTransform->Set_State(CTransform::STATE_LOOK, m_pSelectedTransform->Get_State(CTransform::STATE_LOOK));
		pTempTransform->Set_State(CTransform::STATE_TRANSLATION, m_pSelectedTransform->Get_State(CTransform::STATE_TRANSLATION));


		m_vecCollocatedObject[(_uint)tLayerIndex].push_back(pTemp);
		m_vecObjectTag[(_uint)tLayerIndex].push_back(tObjIndex);

	}
}

void CImguiMgr::CollocateCollider()
{
	if (GAMEINSTANCE->Is_KeyState(KEY::SPACE, KEY_STATE::TAP))
	{

		CGameObject* pTemp = nullptr;

		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Collider"), TEXT("Prototype_GameObject_Collider"), &pTemp)))
			return;

		CTransform* pTempTransform = (CTransform*)pTemp->Get_Component(CGameObject::m_pTransformTag);

		pTempTransform->Set_State(CTransform::STATE_RIGHT, m_pSelectedTransform->Get_State(CTransform::STATE_RIGHT));
		pTempTransform->Set_State(CTransform::STATE_UP, m_pSelectedTransform->Get_State(CTransform::STATE_UP));
		pTempTransform->Set_State(CTransform::STATE_LOOK, m_pSelectedTransform->Get_State(CTransform::STATE_LOOK));
		pTempTransform->Set_State(CTransform::STATE_TRANSLATION, m_pSelectedTransform->Get_State(CTransform::STATE_TRANSLATION));

		m_vecCollider.push_back(pTemp);
	}
}

void CImguiMgr::Save_Map(const char* strStageName,const char* strFileName)
{
	char Filepath[255] = "../Bin/Resources/Map/";
	strcat_s(Filepath, sizeof(Filepath), strStageName);
	strcat_s(Filepath, sizeof(Filepath),"/");
	strcat_s(Filepath, sizeof(Filepath), strFileName);

	HANDLE hFileBrix = CreateFileA(Filepath,
		GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFileBrix)
	{
		MSG_BOX("Failed to save file"); 
		return;
	}

	DWORD dwByteBrix = 0;
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	
	MAP_DATA tMapData;
	for (int i = 0; i < (_uint)MODEL_TAG::MODEL_END; ++i)
	{
		for (auto& elem : m_vecCollocatedHouse[i])
		{
			ZeroMemory(&tMapData, sizeof(MAP_DATA));

			CTransform* pTranform = (CTransform*)elem->Get_Component(CGameObject::m_pTransformTag);
			tMapData.matWorld = pTranform->Get_WorldMatrix();
			tMapData.tModelTag = (MODEL_TAG)i;
			WriteFile(hFileBrix, &tMapData, sizeof(MAP_DATA), &dwByteBrix, nullptr);

		}
	}

	RELEASE_INSTANCE(CGameInstance);
	CloseHandle(hFileBrix);
	MSG_BOX("Saved file");
}

void CImguiMgr::Load_Map(const char* strStageName, const char* strFileName)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	char Filepath[255] = "../Bin/Resources/Map/";
	strcat_s(Filepath, sizeof(Filepath), strStageName);
	strcat_s(Filepath, sizeof(Filepath), "/");
	strcat_s(Filepath, sizeof(Filepath), strFileName);
	HANDLE hFile = CreateFileA(Filepath,
		GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed to load file");
		RELEASE_INSTANCE(CGameInstance);
		return;
	}
	DWORD dwByteHouse= 0;
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
				return;
			}
			CTransform* pTransform = (CTransform*)pTemp->Get_Component(CGameObject::m_pTransformTag);
			pTransform->Set_State(CTransform::STATE_RIGHT, tDataMap.matWorld.r[CTransform::STATE_RIGHT]);
			pTransform->Set_State(CTransform::STATE_UP, tDataMap.matWorld.r[CTransform::STATE_UP]);
			pTransform->Set_State(CTransform::STATE_LOOK, tDataMap.matWorld.r[CTransform::STATE_LOOK]);
			pTransform->Set_State(CTransform::STATE_TRANSLATION, tDataMap.matWorld.r[CTransform::STATE_TRANSLATION]);



			switch (iModelTag)
			{
			case MODEL_TAG::TRUCK:
				static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_Truck"));

				break;
			case MODEL_TAG::FURNISHEDCABIN:
				static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_FurnishedCabin"));

				break;
			case MODEL_TAG::GARAGE:
				static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_Garage"));

				break;
			case MODEL_TAG::PIER_HOUSE:
				static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_Pier_house"));

				break;
			case MODEL_TAG::PIER_HOUSE2:
				static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_Pier_house2"));

				break;
			case MODEL_TAG::OLDHOUSE:
				static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_OldHouse"));

				break;
			case MODEL_TAG::ROOFTOP:
				static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop"));

				break;
			case MODEL_TAG::ROOFTOP_BACKGROUND1:
				static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background1"));

			case MODEL_TAG::ROOFTOP_BACKGROUND2:
				static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background2"));

			case MODEL_TAG::ROOFTOP_BACKGROUND3:
				static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background3"));

			case MODEL_TAG::ROOFTOP_BACKGROUND4:
				static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background4"));

			case MODEL_TAG::ROOFTOP_BACKGROUND5:
				static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background5"));

			case MODEL_TAG::ROOFTOP_BACKGROUND6:
				static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background6"));

			case MODEL_TAG::ROOFTOP_BACKGROUND7:
				static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background7"));

			case MODEL_TAG::ROOFTOP_BACKGROUND8:
				static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background8"));

			case MODEL_TAG::ROOFTOP_BACKGROUND9:
				static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background9"));

			case MODEL_TAG::ROOFTOP_BACKGROUND10:
				static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background10"));

			case MODEL_TAG::ROOFTOP_BACKGROUND11:
				static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background11"));

			case MODEL_TAG::ROOFTOP_BACKGROUND12:
				static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background12"));

			case MODEL_TAG::ROOFTOP_BACKGROUND13:
				static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background13"));

			case MODEL_TAG::ROOFTOP_BACKGROUND14:
				static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_RoofTop_Background14"));

				break;
			case MODEL_TAG::SHELTER:
				static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_Shelter"));

				break;
			case MODEL_TAG::SLUMHOUSE1:
				static_cast<CHouse*>(pTemp)->SetUp_ModelCom(TEXT("Prototype_Component_Model_SlumHouse1"));

				break;

			}

			m_vecCollocatedHouse[(_uint)iModelTag].push_back(pTemp);

		}
	}

	RELEASE_INSTANCE(CGameInstance);
	CloseHandle(hFile);
	MSG_BOX("Loaded file");
}

void CImguiMgr::Save_Object(const char* strStageName, const char* strFileName)
{
	char Filepath[255] = "../Bin/Resources/Map/";
	strcat_s(Filepath, sizeof(Filepath), strStageName);
	strcat_s(Filepath, sizeof(Filepath), "/");
	strcat_s(Filepath, sizeof(Filepath), strFileName);

	HANDLE hFileBrix = CreateFileA(Filepath,
		GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFileBrix)
	{
		MSG_BOX("Failed to save file");
		return;
	}

	DWORD dwByteBrix = 0;
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	OBJ_DATA tObjData;
	for (int i = 0; i < (_uint)LAYER::LAYER_END; ++i)
	{
		_uint iIndex = 0;
		for (auto& elem : m_vecCollocatedObject[i])
		{
			ZeroMemory(&tObjData, sizeof(OBJ_DATA));

			CTransform* pTranform = (CTransform*)elem->Get_Component(CGameObject::m_pTransformTag);
			tObjData.matWorld = pTranform->Get_WorldMatrix();
			tObjData.tLayerTag = (LAYER)i;
			tObjData.tObjTag = m_vecObjectTag[i][iIndex];
			WriteFile(hFileBrix, &tObjData, sizeof(OBJ_DATA), &dwByteBrix, nullptr);
			++iIndex;
		}
	}

	RELEASE_INSTANCE(CGameInstance);
	CloseHandle(hFileBrix);
	MSG_BOX("Saved file");
}

void CImguiMgr::Load_Object(const char* strStageName, const char* strFileName)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	char Filepath[255] = "../Bin/Resources/Map/";
	strcat_s(Filepath, sizeof(Filepath), strStageName);
	strcat_s(Filepath, sizeof(Filepath), "/");
	strcat_s(Filepath, sizeof(Filepath), strFileName);
	HANDLE hFile = CreateFileA(Filepath,
		GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed to load file");
		RELEASE_INSTANCE(CGameInstance);
		return;
	}
	DWORD dwByteHouse = 0;
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

			CGameObject* pTemp = nullptr;
			LAYER iLayerTag = tDataObj.tLayerTag;
			const _tchar* strLayer = nullptr;

			OBJ_TAG iObjTag = tDataObj.tObjTag;
			const _tchar* strPrototypeTag=nullptr;

		
			switch (iObjTag)
			{
			case OBJ_TAG::DOTSPROJECTER:
				strPrototypeTag = TEXT("Prototype_GameObject_DotsProjecter");
				break;
			case OBJ_TAG::FLASHLIGHT:
				strPrototypeTag = TEXT("Prototype_GameObject_FlashLight");
				break;

			case OBJ_TAG::THERMOMETER:
				strPrototypeTag = TEXT("Prototype_GameObject_Thermometer");
				break;

			case OBJ_TAG::EMF:
				strPrototypeTag = TEXT("Prototype_GameObject_EMF");
				break;

			case OBJ_TAG::NOTE:
				strPrototypeTag = TEXT("Prototype_GameObject_Note");
				break;

			case OBJ_TAG::SPIRITBOX:
				strPrototypeTag = TEXT("Prototype_GameObject_SpiritBox");
				break;

			case OBJ_TAG::VIDEOCAMERA:
				strPrototypeTag = TEXT("Prototype_GameObject_Video_Camera");
				break;
			}
			

			if (FAILED(pGameInstance->Add_GameObject(
				LEVEL_STAGE1,
				TEXT("Layer_Object"),
				strPrototypeTag,
				&pTemp)))
			{
				MSG_BOX("Fail");
				RELEASE_INSTANCE(CGameInstance);
				return;
			}
			CTransform* pTransform = (CTransform*)pTemp->Get_Component(CGameObject::m_pTransformTag);
			pTransform->Set_State(CTransform::STATE_RIGHT, tDataObj.matWorld.r[CTransform::STATE_RIGHT]);
			pTransform->Set_State(CTransform::STATE_UP, tDataObj.matWorld.r[CTransform::STATE_UP]);
			pTransform->Set_State(CTransform::STATE_LOOK, tDataObj.matWorld.r[CTransform::STATE_LOOK]);
			pTransform->Set_State(CTransform::STATE_TRANSLATION, tDataObj.matWorld.r[CTransform::STATE_TRANSLATION]);




			m_vecCollocatedObject[(_uint)iLayerTag].push_back(pTemp);

		}
	}

	RELEASE_INSTANCE(CGameInstance);
	CloseHandle(hFile);
	MSG_BOX("Loaded file");
}

void CImguiMgr::Save_Collider(const char* strStageName, const char* strFileName)
{
	char Filepath[255] = "../Bin/Resources/Map/";
	strcat_s(Filepath, sizeof(Filepath), strStageName);
	strcat_s(Filepath, sizeof(Filepath), "/");
	strcat_s(Filepath, sizeof(Filepath), strFileName);

	HANDLE hFileBrix = CreateFileA(Filepath,
		GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFileBrix)
	{
		MSG_BOX("Failed to save file");
		return;
	}

	DWORD dwByteBrix = 0;
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	COLLIDER_DATA tColliderData;

	for (auto& elem : m_vecCollider)
	{
		ZeroMemory(&tColliderData, sizeof(COLLIDER_DATA));

		CTransform* pTranform = (CTransform*)elem->Get_Component(CGameObject::m_pTransformTag);
		tColliderData.matWorld = pTranform->Get_WorldMatrix();
		WriteFile(hFileBrix, &tColliderData, sizeof(COLLIDER_DATA), &dwByteBrix, nullptr);

	}
	

	RELEASE_INSTANCE(CGameInstance);
	CloseHandle(hFileBrix);
	MSG_BOX("Saved file");
}

void CImguiMgr::Load_Collider(const char* strStageName, const char* strFileName)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	char Filepath[255] = "../Bin/Resources/Map/";
	strcat_s(Filepath, sizeof(Filepath), strStageName);
	strcat_s(Filepath, sizeof(Filepath), "/");
	strcat_s(Filepath, sizeof(Filepath), strFileName);
	HANDLE hFile = CreateFileA(Filepath,
		GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Failed to load file");
		RELEASE_INSTANCE(CGameInstance);
		return;
	}
	DWORD dwByteHouse = 0;
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
				return;
			}

			CTransform* pTransform = (CTransform*)pTemp->Get_Component(CGameObject::m_pTransformTag);
			pTransform->Set_State(CTransform::STATE_RIGHT, tDataCollider.matWorld.r[CTransform::STATE_RIGHT]);
			pTransform->Set_State(CTransform::STATE_UP, tDataCollider.matWorld.r[CTransform::STATE_UP]);
			pTransform->Set_State(CTransform::STATE_LOOK, tDataCollider.matWorld.r[CTransform::STATE_LOOK]);
			pTransform->Set_State(CTransform::STATE_TRANSLATION, tDataCollider.matWorld.r[CTransform::STATE_TRANSLATION]);




			m_vecCollider.push_back(pTemp);

		}
	}

	RELEASE_INSTANCE(CGameInstance);
	CloseHandle(hFile);
	MSG_BOX("Loaded file");
}
