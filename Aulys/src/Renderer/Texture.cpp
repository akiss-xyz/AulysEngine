#include "src/pch.h"

#include "Texture.h"

#include <string>

#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Aulys 
{
	Ref<Texture2D> Texture2D::create(const Path& path) {
		switch (Renderer::getAPI()) {
			case RendererAPI::API::None:
			{
				 AU_CORE_ASSERT(false, "[Texture2D::create] RendererAPI::None isn't supported.");
				 return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				try {
					return std::make_shared<OpenGLTexture2D>(path);
				}
				catch(std::exception& e) {
					AU_LOG_ERROR("Failed to create OpenGL Texture2D from path=\"{0}\". Caught exception:\n{1}", path.get(), e.what());
					return std::make_shared<OpenGLTexture2D>(Path("Aulys/assets/textures/errortexture2d.png"));
				}
				catch (...) {
					AU_LOG_ERROR("Failed to create OpenGL Texture2D from path=\"{0}\". Caught exception:\nUnknown", path.get());
					return std::make_shared<OpenGLTexture2D>(Path("Aulys/assets/textures/errortexture2d.png"));
				}
			}
		}
		AU_CORE_ASSERT(false, "[Texture2D::create] RendererAPI not found.");
		return nullptr;
	};

	Ref<Texture2D> Texture2D::create(int width, int height) {
		switch (Renderer::getAPI()) {
			case RendererAPI::API::None:
			{
				 AU_CORE_ASSERT(false, "[Texture2D::create] RendererAPI::None isn't supported.");
				 return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				try {
					return std::make_shared<OpenGLTexture2D>(width, height);
				}
				catch(std::exception& e) {
					AU_LOG_ERROR("Failed to create OpenGL Texture2D (width={0}, height={1}. Caught exception:\n{2}", width, height, e.what());
					return std::make_shared<OpenGLTexture2D>(Path("Aulys/assets/textures/errortexture2d.png"));
				}
			}
		}
		AU_CORE_ASSERT(false, "[Texture2D::create] RendererAPI not found.");
		return nullptr;
	};
}; // namespace Aulys

