#include "src/pch.h"

#include "ImGui/ImGuiLayer.h"

#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"
// Temporary, ideally this would be platform independent.
// However, as all renderers currently use opengl/glfw, there's no real need to get rid of this.
// To use something else, simply change the ImGui_ImplGlfw/ImplOpengl lines as needed.

#include "Core/Core.h"
#include "Log/Log.h"
#include "Events/Event.h"
#include "Events/MouseEvent.h"
#include "Application.h"

namespace Aulys 
{
	ImGuiLayer::ImGuiLayer(const std::string& name) : Layer(name) {

	};
	
	ImGuiLayer::~ImGuiLayer()
	{
	
	};

	void ImGuiLayer::onAttach() {
		IMGUI_CHECKVERSION();

		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		// io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports | ImGuiBackendFlags_RendererHasViewports;
		// io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		// io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		// io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		ImGuiStyle& style = ImGui::GetStyle();
 		if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
 			style.WindowRounding = 0.0f;
 			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
 		}

		Application& app = Application::get();
		auto window = static_cast<GLFWwindow*>(app.getWindow().getNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	};

	void ImGuiLayer::onDetach() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	};

	void ImGuiLayer::onImGuiRender(Timestep ts){
	}

	void ImGuiLayer::onEvent(Event& e) {
		EventDispatcher disp(e);

		disp.dispatch<KeyPressedEvent>(
			[this](KeyPressedEvent& e) {
				if (ImGui::GetIO().WantCaptureKeyboard) {
					e.handled = true;
					return true;
				}
				return false;
			}
		);

		disp.dispatch<MouseButtonEvent>(
			[this](MouseButtonEvent& e) {
				if (ImGui::GetIO().WantCaptureMouse) {
					e.handled = true;
					return true;
				}
				return false;
			}
		);
	};
		

	void ImGuiLayer::swapFont(const char* name, Path path, float size) {
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF(
				path.gets()->data(), size);
	}

	void ImGuiLayer::begin() {
		 ImGui_ImplOpenGL3_NewFrame();
		 ImGui_ImplGlfw_NewFrame();
		 ImGui::NewFrame();
	};

	void ImGuiLayer::end() {
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::get();

		io.DisplaySize = ImVec2(app.getWindow().getWidth(), app.getWindow().getHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

 		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
 		{
 			GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
 			ImGui::UpdatePlatformWindows();
 			ImGui::RenderPlatformWindowsDefault();
 			glfwMakeContextCurrent(backupCurrentContext);
 		}
	}
}; // namespace Aulys
