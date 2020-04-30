#include "src/pch.h"

#include <glad/glad.h>

#include "Renderer/Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer/Renderer.h"

namespace Aulys {
	Ref<Shader> Shader::create(Path vertexPath, Path fragmentPath){
		switch(Renderer::getAPI()){
			case RendererAPI::API::None:
			{
				AU_CORE_ASSERT(false, "[Shader::create]: RenderAPI::None is not supported!"
						" (retrieved from Renderer::getAPI())");
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLShader>(vertexPath, fragmentPath);
			}
			default:
				AU_CORE_ASSERT(false, "[Shader::create]: sRenderAPI"
					" (retrieved from Renderer::getAPI()) is set to an unknown value"
					" (RendererAPI::API enum cast to int) ({0}).", 
					(int) Renderer::getAPI());
				return nullptr;
		}
		AU_CORE_ASSERT(false, "[Shader::create]: sRenderAPI"
			" (retrieved from Renderer::getAPI()) is set to an unknown value"
			" (RendererAPI::API enum cast to int) ({0}).", 
			(int) Renderer::getAPI());
		return nullptr;
	};
	Ref<Shader> Shader::create(const std::string& vertexSrc, const std::string& fragmentSrc){
		switch(Renderer::getAPI()){
			case RendererAPI::API::None:
			{
				AU_CORE_ASSERT(false, "[Shader::create]: RenderAPI::None is not supported!"
						" (retrieved from Renderer::getAPI())");
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLShader>(vertexSrc, fragmentSrc);
			}
			default:
				AU_CORE_ASSERT(false, "[Shader::create]: sRenderAPI"
					" (retrieved from Renderer::getAPI()) is set to an unknown value"
					" (RendererAPI::API enum cast to int) ({0}).", 
					(int) Renderer::getAPI());
				return nullptr;
		}
		AU_CORE_ASSERT(false, "[Shader::create]: sRenderAPI"
			" (retrieved from Renderer::getAPI()) is set to an unknown value"
			" (RendererAPI::API enum cast to int) ({0}).", 
			(int) Renderer::getAPI());
		return nullptr;
	};

	Ref<Shader> Shader::create(Path path){
		switch(Renderer::getAPI()){
			case RendererAPI::API::None:
			{
				AU_CORE_ASSERT(false, "[Shader::create]: RenderAPI::None is not supported!"
						" (retrieved from Renderer::getAPI())");
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLShader>(path);
			}
			default:
				AU_CORE_ASSERT(false, "[Shader::create]: sRenderAPI"
					" (retrieved from Renderer::getAPI()) is set to an unknown value"
					" (RendererAPI::API enum cast to int) ({0}).", 
					(int) Renderer::getAPI());
				return nullptr;
		}
		AU_CORE_ASSERT(false, "[Shader::create]: sRenderAPI"
			" (retrieved from Renderer::getAPI()) is set to an unknown value"
			" (RendererAPI::API enum cast to int) ({0}).", 
			(int) Renderer::getAPI());
		return nullptr;
	};

	Ref<Shader> Shader::create(const std::string& source){
		switch(Renderer::getAPI()){
			case RendererAPI::API::None:
			{
				AU_CORE_ASSERT(false, "[Shader::create]: RenderAPI::None is not supported!"
						" (retrieved from Renderer::getAPI())");
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLShader>(source);
			}
			default:
				AU_CORE_ASSERT(false, "[Shader::create]: sRenderAPI"
					" (retrieved from Renderer::getAPI()) is set to an unknown value"
					" (RendererAPI::API enum cast to int) ({0}).", 
					(int) Renderer::getAPI());
				return nullptr;
		}
		AU_CORE_ASSERT(false, "[Shader::create]: sRenderAPI"
			" (retrieved from Renderer::getAPI()) is set to an unknown value"
			" (RendererAPI::API enum cast to int) ({0}).", 
			(int) Renderer::getAPI());
		return nullptr;
	};
}
