#include "src/pch.h"

#include "Renderer/RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Aulys 
{
	RendererAPI* RenderCommand::sRendererAPI = new OpenGLRendererAPI;
}; // namespace Aulys

