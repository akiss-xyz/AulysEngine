#include "src/pch.h"

#include "Core/Timestep.h"

#include "Platform/Linux/LinuxWindow.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include "Renderer/GraphicsContext.h"

#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"

namespace Aulys
{

	void LinuxWindow::resizeEventCallback(GLFWwindow* window, int width, int height) {
		// Handle the event
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		WindowResizeEvent event(width, height);

		data.width = width;
		data.height = height;

		data.eventCallback(event);
	};

	static bool GLFWInitialized = false;

	LinuxWindow::LinuxWindow(const WindowProps& props)
	{
		init(props);
	};

	LinuxWindow::~LinuxWindow() = default;

	static void glfwErrorCallback(int error, const char* description)
	{
		AU_LOG_ERROR("GLFW Error ({0}): ({1})", error, description);
	};

	void LinuxWindow::init(const WindowProps& props)
	{
		mData.title = props.title;
		mData.width = props.width;
		mData.height = props.height;

		ALOGIL("Creating (Linux) Window \"{0}\" ({1}, {2})", props.title, props.width, props.height);

		if (!GLFWInitialized)
		{
			int success = glfwInit();

			AU_CORE_ASSERT(success,
		"Failed to initialize GLFW - please ensure you have all dependencies (check dependencies.md and glfw documentation). glfwInit() failed.")

			glfwSetErrorCallback(glfwErrorCallback);

			// Tell glfw about which version and profile we'd like to use, so that we have access to
			// what we expect and can use ImGui.
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			GLFWInitialized = true;
		}
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		if(mode->width < props.width || mode->height < props.height) {
			mWindow = glfwCreateWindow(mode->width, mode->height, mData.title.c_str(), nullptr,
			                           nullptr);
		}
		else {
			mWindow = glfwCreateWindow((int)props.width, (int)props.height, mData.title.c_str(),
									   nullptr, nullptr);
		}

		mContext = new OpenGLContext(mWindow);
		mContext->init();

		glfwSetWindowUserPointer(mWindow, &mData);
		setVSync(true);

		// Set GLFW event callbacks
		glfwSetWindowSizeCallback(mWindow, LinuxWindow::resizeEventCallback);

		glfwSetWindowCloseCallback(mWindow, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.eventCallback(event);
		});

		glfwSetKeyCallback(mWindow, [](GLFWwindow* window, int key,
									   int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				data.eventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.eventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, 1);
				data.eventCallback(event);
				break;
			}
			};
		});

		glfwSetCharCallback(mWindow, [](GLFWwindow* window, unsigned int key){
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(key);
			data.eventCallback(event);
		});

		glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.eventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.eventCallback(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(mWindow, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.eventCallback(event);
		});

		glfwSetCursorPosCallback(mWindow, [](GLFWwindow* window, double xPos, double yPos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.eventCallback(event);
		});

		glfwSetDropCallback(mWindow, [](GLFWwindow* window, int count, const char** paths) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			for (int i = 0; i < count; i++) {
				auto s = std::string(paths[i]);
				DragAndDropEvent event(s, s.substr(s.find_last_of('.')+1));
				data.eventCallback(event);
			}
		});
	};


	void LinuxWindow::shutdown()
	{
		glfwDestroyWindow(mWindow);
	};


	void LinuxWindow::onUpdate()
	{
		mContext->swapBuffers();
		glfwPollEvents();
	};


	void LinuxWindow::setVSync(bool enabled)
	{
		if (enabled) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}

		mData.vSync = enabled;
	};


	bool LinuxWindow::isVSync() const
	{
		return mData.vSync;
	};

	// Takes the lastFrameTime as an output parameter - it uses this to calculate the deltaTime and
	// then writes into it the new FrameTime for next use.
	Timestep LinuxWindow::calculateDeltaTime(float& lastFrameTime) const {
		Timestep deltatime = (float)glfwGetTime() - lastFrameTime;
		lastFrameTime = deltatime.getSeconds() + lastFrameTime;
		return deltatime;
	};

	void LinuxWindow::setFullscreen(bool fullscreen) {
		if (isFullscreen() == fullscreen) {
			return;
		}
		if (fullscreen) {
			glfwGetWindowPos(mWindow, &(mData.pos[0]), &(mData.pos[1]));
			glfwGetWindowSize(mWindow, &mData.width, &mData.height);

			const auto monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			glfwSetWindowMonitor(mWindow, monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
			LinuxWindow::resizeEventCallback(mWindow, mode->width, mode->height);
		}
		else {
			glfwSetWindowMonitor(mWindow, nullptr, mData.pos[0], mData.pos[1], mData.width, mData.height, GLFW_DONT_CARE);
			LinuxWindow::resizeEventCallback(mWindow, mData.width, mData.height);
		}
	};

}; // namespace Aulys
