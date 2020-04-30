#pragma once

#include "Renderer/RendererAPI.h"

namespace Aulys 
{
	class OpenGLRendererAPI : public RendererAPI 
	{
	public:
		virtual void init() override;
		virtual void setClearColor(const glm::vec4& color) override;
		virtual void clear() override;

		virtual void drawIndexed(const Ref<VertexArray>& vertexArray) override;

		struct glCallbackError
		{
			// This struct is created in the Aulys::errorCallback fn, located at
			// (Platform/OpenGL/OpenGLRendererAPI.cpp), from the data provided by OpenGL to the
			// callback function. This is then parsed and passed to OpenGLRendererAPI::handleError.
			unsigned int source;
			unsigned int type;
			unsigned int id;
			unsigned int logLevel; // Converted from GL_DEBUG_SEVERITY_[WHATEVER] to our log levels.
			// _NOTIFICATION = INFO = 0
			// _LOW = WARN = 1
			// _MEDIUM = ERROR = 2
			// _HIGH = ERROR = 3, with different note.
			//         ^ For more details on these log levels, see Log.h
			std::string message = "";
		}; // struct glCallbackError

		void handleError(const glCallbackError& e) const noexcept;
	private:

	}; // class OpenGLRendererAPI : RendererAPI
}; // namespace Aulys
