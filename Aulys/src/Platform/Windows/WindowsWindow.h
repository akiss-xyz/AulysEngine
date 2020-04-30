#pragma once

#include "Platform/Window.h"

#include "Core/Timestep.h"

#include "Renderer/GraphicsContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Aulys
{

	class WindowsWindow : Window
	{
	public:
		WindowsWindow(const WindowProps& props);

		virtual ~WindowsWindow() = default;

		void onUpdate() override;

		inline unsigned int getWidth() const override { return mData.width; };
		inline unsigned int getHeight() const override { return mData.height; };
		bool isFullscreen() const override { return glfwGetWindowMonitor(mWindow);  };
		void setFullscreen(bool fullscreen = true) override;

		inline void setEventCallback(const std::function<void(Event&)> callback) override {
			mData.eventCallback = callback;  
		};

		void setVSync(bool enabled) override;
		bool isVSync() const override;
		Timestep calculateDeltaTime(float& lastFrameTime) const override;

        void* getNativeWindow() const override { return this->mWindow; };

		struct WindowData
		{
			std::string title;
			int width, height;
			std::array<int, 2> pos;
			bool vSync;

			std::function<void(Event&)> eventCallback;
		}; // struct WindowData
	private:
		static void resizeEventCallback(GLFWwindow* window, int width, int height);

		virtual void init(const WindowProps& props);
		virtual void shutdown();

		GLFWwindow* mWindow;
		GraphicsContext* mContext;

		WindowData mData;
	}; // class WindowsWindow : Window

};	 // namespace Aulys
