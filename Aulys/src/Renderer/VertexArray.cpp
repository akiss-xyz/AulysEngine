#include "src/pch.h"

#include "Renderer/VertexArray.h"

#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Aulys 
{
	Ref<VertexArray> VertexArray::create() {
		switch (Renderer::getAPI()) {
			case RendererAPI::API::None:
			{
				 AU_CORE_ASSERT(false, "RendererAPI::None isn't supported.");
				 return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLVertexArray>();
			}
			default:
				AU_CORE_ASSERT(false, "sRenderAPI is set to an unknown value.")
				return nullptr;
		}
	};


}; // namespace Aulys
