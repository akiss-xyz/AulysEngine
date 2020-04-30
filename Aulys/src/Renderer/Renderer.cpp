#include "src/pch.h"

#include "Renderer/Renderer.h"

namespace Aulys 
{
	Renderer::SceneData* Renderer::mSceneData = new Renderer::SceneData;

	void Renderer::init() {
		RenderCommand::init();
	}

	void Renderer::beginScene(OrthographicCamera& camera) {
		mSceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
	}

	void Renderer::endScene() {
		
	}

	void Renderer::submit(const Ref<Shader>& shader, 
			const Ref<VertexArray>& vertexArray, const glm::mat4& transform) {
		shader->bind();
		// This does not need to be done every time! If one shader is shared across many objects in
		// a scene, you can actually just upload once and render it all together without updating
		// that each time.
		shader->uploadUniformMat4("uViewProjection",  mSceneData->viewProjectionMatrix);
		shader->uploadUniformMat4("uTransform", transform);

		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}

}; // namespace Aulys
