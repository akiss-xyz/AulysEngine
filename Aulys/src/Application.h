#pragma once

#include "Core/Core.h"
#include "Core/Timestep.h"
#include "Core/LayerStack.h"

#include "Platform/Window.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "Log/Log.h"

#include "ImGui/ImGuiLayer.h"

#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/OrthographicCamera.h"

namespace Aulys
{
	class Application
	{
	public:
		Application(const WindowProps& windowSettings = {"Au Engine", 1920, 1080});

		virtual ~Application() = default;

		virtual void Run();

		void onEvent(Event& e) noexcept;

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);

		static inline Application& get() { return *sInstance; };
		inline Window& getWindow() const { return *mWindow; };
	protected:
		Uni<Window> mWindow;
		ImGuiLayer* mImGuiLayer;

		bool mRunning = true;
		LayerStack mLayerStack;

		float mLastFrameTime = 0.0f;

		OrthographicCamera mCamera;

		// Some utility functions for managing how the application exits.
		// These are called in Application::onWindowClose, which hooks into WindowCloseEvents.
		virtual bool decideIfWindowCanClose(const WindowCloseEvent& e) { return true; };
			// This is used to decide whether we should go ahead with shutting down. You might use
			// this for a dialogue popup "Are you sure you want to lose all of your work or
			// whatever? Yes/No"
		virtual void closeWindow(const WindowCloseEvent& e) {};
			// This is called before mRunning is set to false, so that you can clean up whatever you
			// need to clean up.
	private:
		static Application* sInstance;

		bool onWindowClose(WindowCloseEvent& e);
	}; // class Application

	// To be defined by the app using Aulys.
	Aulys::Application* CreateApplication();
}; // namespace Aulys
