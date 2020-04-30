#pragma once

#include "Renderer/RendererAPI.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/OrthographicCamera.h"

#include "Shader.h"

namespace Aulys
{
	// The renderer defines a number of default uniforms to the shader.
	// uniform mat4 uViewProjection <-> mSceneData->viewProjectionMatrix <-> beginScene(cam)
	// uniform mat4 uTransform <-> Renderer::submit(..., const glm::mat4& transform)
	class Renderer 
	{
	public:
		static void init();

		static void beginScene(OrthographicCamera& camera);
		static void endScene();

		static void submit(const Ref<Shader>& shader,
				const Ref<VertexArray>& vertexArray, 
				const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API getAPI() {
			return RendererAPI::getAPI();
		}
	private:
		struct SceneData {
			glm::mat4 viewProjectionMatrix;
		};

		static SceneData* mSceneData;
	}; // class Renderer
}; // namespace Aulys
