#include "src/pch.h"

#include "FrameBuffer.h"

#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Aulys 
{
	Ref<FrameBuffer> FrameBuffer::create() {
		switch(Renderer::getAPI()){
			case RendererAPI::API::None:
			{
				AU_CORE_ASSERT(false, "[FrameBuffer::create]: RenderAPI::None is not supported!"
						" (retrieved from Renderer::getAPI())");
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLFrameBuffer>();
			}
			default:
				AU_CORE_ASSERT(false, "[FrameBuffer::create]: sRenderAPI"
					" (retrieved from Renderer::getAPI()) is set to an unknown value"
					" (RendererAPI::API enum cast to int) ({0}).", 
					(int) Renderer::getAPI());
				return nullptr;
		}
	};
}; // namespace Aulys
