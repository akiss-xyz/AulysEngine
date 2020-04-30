#include "src/pch.h"

#include <GLFW/glfw3.h>

#include "Platform/Windows/WindowsInput.h"
#include "Application.h"

namespace Aulys 
{
	Input* Input::sInstance = new WindowsInput();

	bool WindowsInput::isKeyPressedImpl(int keycode) {
		auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		
		auto state = glfwGetKey(window, keycode);
		return (state == GLFW_PRESS) || (state == GLFW_REPEAT);
	}

	bool WindowsInput::isMouseButtonPressedImpl(int keycode) {
		auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());

		return glfwGetMouseButton(window, keycode) == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::getMousePosImpl() {
		auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return std::make_pair<float, float>((float)xpos, (float)ypos);
	}

	float WindowsInput::getMouseXImpl() {
		auto[x, y] = getMousePosImpl();

		return x;
	}

	float WindowsInput::getMouseYImpl() {
		auto[x, y] = getMousePosImpl();

		return y;
	}

}; // namespace Aulys
