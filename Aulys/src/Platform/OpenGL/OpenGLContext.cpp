#include "src/pch.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include "Platform/OpenGL/OpenGLContext.h"


namespace Aulys 
{
	OpenGLContext::OpenGLContext(GLFWwindow* window) : mWindow(window) {
		AU_CORE_ASSERT(window, "Handle to the GLFWwindow given to the context is null!");
	}
	void OpenGLContext::init()
	{
		glfwMakeContextCurrent(mWindow);

		int status = gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress );
		AU_CORE_ASSERT(status, "Failed to initialize Glad.");

		AU_LOG_INFO("OpenGL Info:"
		            "\n   Vendor: {}"
		            "\n   Renderer: {}"
		            "\n   Version: {}", 
		            glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));
	}


	void OpenGLContext::swapBuffers(){
		glfwSwapBuffers(mWindow);
	}
}; // namespace Aulys

