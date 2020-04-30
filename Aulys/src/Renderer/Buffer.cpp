#include "src/pch.h"

#include "Renderer/Buffer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Renderer/Renderer.h"

namespace Aulys 
{
	Ref<VertexBuffer> VertexBuffer::create(float* vertices, uint32_t size){
		switch (Renderer::getAPI()){
			case RendererAPI::API::None:
			{
				AU_CORE_ASSERT(false, "Error: RenderAPI::None is not supported!"
				               " For some reason, Renderer::getAPI() (reading from sAPI)"
				               " returned RenderAPI::None. Are you sure you have the correct one"
							   " set in RendererAPI.cpp?");
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLVertexBuffer>(vertices, size);
			}
			default:
				AU_CORE_ASSERT(false, "sAPI, instantiated in RendererAPI.cpp (retrieved by "
						"Renderer::getAPI()) is set to an unknown value.");
				return nullptr;
		}
	};

	Ref<IndexBuffer> IndexBuffer::create(uint32_t* indices, uint32_t size){
		switch (Renderer::getAPI()){
			case RendererAPI::API::None:
			{
				AU_CORE_ASSERT(false, "Error from IndexBuffer::create: RenderAPI::None is not supported!");
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLIndexBuffer>(indices, size);
			}
			default:
				AU_CORE_ASSERT(false, "sAPI, instantiated in RendererAPI.cpp (retrieved by "
						"Renderer::getAPI()) is set to an unknown value.");
				return nullptr;
		}
	};

}; // namespace Aulys
