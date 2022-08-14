#include "stdafx.h"
#include "ImguiMgr.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "GameObject.h"


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
		Tool_Map();
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
		bFirst = true;

		CGameObject* pTerrain = nullptr;
		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Terrain"), TEXT("Prototype_GameObject_Terrain"), &pTerrain)))
			return;

		m_pTerrainTransform = (CTransform*)pTerrain->Get_Component(CGameObject::m_pTransformTag);
		m_pTerrainVIBuffer = (CVIBuffer_Terrain*)pTerrain->Get_Component(TEXT("Com_VIBuffer"));

		CGameObject* pTemp = nullptr;
		if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Prototype"), TEXT("Prototype_GameObject_DotsProjecter"), &pTemp)))
			return;
		pTemp->Set_Enable(false);
		m_vecPrototype.push_back(pTemp);
	}
}

void CImguiMgr::ShowWindow(bool* p_open)
{
}

void CImguiMgr::Tool_Map()
{
	ImGui::Begin("Tool_Map");


	const char* items[] = { "DotsProjecter" };
	static int item_current_idx = 0; // Here we store our selection data as an index.
	if (ImGui::BeginListBox("listbox 1"))
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			m_vecPrototype[n]->Set_Enable(false);
			const bool is_selected = (item_current_idx == n);
			if (ImGui::Selectable(items[n], is_selected))
				item_current_idx = n;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndListBox();
		m_pSelectedObject = m_vecPrototype[item_current_idx];
		m_pSelectedObject->Set_Enable(true);

	}
	ImGui::End();


	//ImGui::Begin("Main (Map Tool)", 0, ImGuiWindowFlags_AlwaysAutoResize);
//if (ImGui::BeginTabBar("Main Tab Bar Map Tool"))
//{
//	Map_MapTool();
//	Object_MapTool();

//	ImGui::EndTabBar();
//} /*TabBar*/
//ImGui::End();

}

void CImguiMgr::Picking_Object()
{
	_float4 fPosition;
	if (m_pSelectedObject)
	{
		if (CMath_Utility::Picking(m_pTerrainVIBuffer, m_pTerrainTransform, &fPosition));
		{
			CTransform* pSelectedTransfrom = (CTransform*)m_pSelectedObject->Get_Component(CGameObject::m_pTransformTag);
			pSelectedTransfrom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&fPosition));
		}
	}
}


void CImguiMgr::Object_MapTool()
{
	if (ImGui::BeginTabItem("Objects"))
	{
		//Widget_WallListBox_Map();
		SelectObjectButton_Map();
		ImGui::EndTabItem();
	}
}

void CImguiMgr::Map_MapTool()
{
}

void CImguiMgr::SelectObjectButton_Map()
{
	//static char SelectObjectBuffer[255] = "";
	//ImGui::InputText("Input object to select", SelectObjectBuffer, sizeof(SelectObjectBuffer));
	//m_strSelectObject_Map = SelectObjectBuffer;
	//if (ImGui::Button("Select object"))
	//{
	//	for (auto& iter : m_PrototypeTagList)
	//	{
	//		if (0 == m_strSelectObject_Map.compare(iter.c_str()))
	//		{
	//			m_strCurObj = m_strSelectObject_Map;
	//			MessageBox(0, TEXT("changed object"), TEXT("message box"), MB_OK);
	//		/*	if ("Brix" == m_strCurObj)
	//			{
	//				m_bIsBrix = true;
	//				Update_PreviewCubeScale(_float3(1.f, 1.f, 1.f));
	//			}
	//			else
	//			{
	//				m_bIsBrix = false;*/
	//				if (0 == m_strCurObj.compare("Monster_Afrit"))
	//				{
	//					m_tInstallObjData.tObjTag = OBJ_TAG::AFRIT;
	//					m_tInstallObjData.tLayerTag = LAYER_TAG::MONSTER;
	//					Update_PreviewCubeScale(_float3(1.f, 1.f, 1.f));
	//				}
	//			

	//			}
	//		}
	//	}
	//}
}



//void  CImguiMgr::CleanupDeviceD3D()
//{
//	CleanupRenderTarget();
//	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
//	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
//	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
//}