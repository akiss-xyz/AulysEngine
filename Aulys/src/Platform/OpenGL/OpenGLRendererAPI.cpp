#include "src/pch.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include "glad/glad.h"

namespace Aulys 
{
	// This fn is called by OpenGL as a callback when an error occurs. This processes the passed in
	// info a bit, and then calls the OpenGLRendererAPI instance's handleError fn, so that we can
	// print or do whatever we want, really.
	void errorCallback(GLenum source, GLenum type, GLuint id, 
			GLenum severity, GLsizei length, const GLchar* message, 
			const void* pointerToOpenGLRendererAPIInstance) 
	{
		unsigned int logLevel = 0;
		switch (severity) {
			// See OpenGLRendererAPI::glCallbackError in Platform/OpenGL/OpenGLRendererAPI.h
			// This OpenGL severity here            corresponds to:
			case GL_DEBUG_SEVERITY_NOTIFICATION:    logLevel = 0; // ^ Log level INFO
			case GL_DEBUG_SEVERITY_LOW:             logLevel = 1; // ^ Log level WARN
			case GL_DEBUG_SEVERITY_MEDIUM:          logLevel = 2; // ^ Log level ERROR
			case GL_DEBUG_SEVERITY_HIGH:            logLevel = 3; // ^ Log level ERROR, with note
		}

		switch(id) {
			case 131185:
				// AU_LOG_TRACE_LINE("Collapsed error of type GL_DEBUG_TYPE_OTHER, id=131185.");
				return;
			case 5:
				 // AU_LOG_TRACE_LINE("Collapsed error of type GL_DEBUG_TYPE_OTHER, id=5.");
				 return;
			case 6:
				 // AU_LOG_TRACE_LINE("Collapsed error of type GL_DEBUG_TYPE_OTHER, id=6.");
				 return;
			default:
				((const OpenGLRendererAPI*)pointerToOpenGLRendererAPIInstance)->handleError(
					{source, type, id, logLevel, message}
				);
		}

	}

	void OpenGLRendererAPI::init() {
#ifdef ENABLE_GRAPHICS_DEBUG_OUTPUT
		glEnable(GL_DEBUG_OUTPUT);
#endif
#ifdef ENABLE_GRAPHICS_BLEND
		glEnable(GL_BLEND); // Enable blending colours on top of each other.
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif
		glDebugMessageCallback(errorCallback, this);
	}

	void OpenGLRendererAPI::setClearColor(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::drawIndexed(const Ref<VertexArray>& vertexArray) {
		glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT,
				nullptr);
		//// ^ Why nullptr?
		// We're allowed to pass nullptr for the 'offset pointer to indices' argument, because
		// the offset of the first index is 0. 
		// To get a clear picture of what's happening here, look at
		// Renderer/Renderer.cpp::Renderer::submit(..) - we bind our vertexArray before calling
		// this drawIndexed function, so OpenGL knows exactly what we're talking about.
		// In that fn, vertexArray is expected to have an indexBuffer bound to it, and if you
		// look at the function VertexArray::setIndexBuffer (implemented by OpenGLVertexArray),
		// then you'll see that that binds the index buffer.
	}

	void OpenGLRendererAPI::handleError(const glCallbackError& e) const noexcept {
		switch(e.logLevel) {
			case 0: 
				AU_LOG_INFO("OpenGL Notification (severity _NOTIFICATION)"
				                                 " (source: {0}, type: {1}, id: {2}):\n"
				            "\n"
				            "    {3}"
				            "\n", 
						e.source, e.type, e.id, e.message);
			case 1: 
				AU_LOG_WARN("OpenGL Warning (severity _LOW) (source: {0}, type: {1}, id: {2}):\n"
				            "\n"
				            "    {3}"
				            "\n", 
						e.source, e.type, e.id, e.message);
			case 2: 
				AU_LOG_ERROR("OpenGL Error (severity _MEDIUM) (source: {0}, type: {1}, id: {2}):\n"
				             "\n"
				             "    {3}"
				             "\n",
						e.source, e.type, e.id, e.message);
			case 3: 
				AU_LOG_ERROR("OpenGL Fatal (severity _HIGH) (source: {0}, type: {1}, id: {2}):\n"
				             "\n"
				             "    {3}"
				             "\n",
						e.source, e.type, e.id, e.message);
		}
	}

}; // namespace Aulys

