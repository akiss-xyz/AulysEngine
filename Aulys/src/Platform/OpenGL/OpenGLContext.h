#pragma once

#include "Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Aulys 
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* window);

		void init() override;
		void swapBuffers() override;

	private:
		GLFWwindow* mWindow;
	
	}; // class OpenGLContext : 
}; // namespace Aulys
