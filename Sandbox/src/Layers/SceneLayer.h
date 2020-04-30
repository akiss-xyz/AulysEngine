#pragma once

#include "Aulys.h"
using namespace Aulys;

#include "Renderer/FrameBuffer.h"

#include "Events/AppEvent.h"
#include "Geometry/GeometryMaths.h"

namespace App 
{
	class SceneLayer : public Layer 
	{
	public:
		SceneLayer(Ref<Texture2D> frameBufferTexture, const std::string& name = "SceneLayer")
			: Layer(name), mCamera(-1.6f, 1.6f, -0.9f, 0.9f),
			  mFrameBufferTexture(frameBufferTexture) {}; 
		
		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate(Timestep ts) override;
		virtual void onEvent(Event& event) override;

	private:
		Ref<Shader> mShaderProgram;
		Ref<VertexArray> mRenderQuadVA;

		Ref<Texture2D> mTexture;
		Ref<FrameBuffer> mFrameBuffer = FrameBuffer::create();
		Ref<Texture2D> mFrameBufferTexture;

		float mScale = 4.0f;
		float mTimeElapsed = 0.0f;

		OrthographicCamera mCamera;

		// Scene state
		glm::mat4 currentBoost{ 1.0f };
		glm::mat4 cellBoost{ 1.0f };
		glm::mat4 invCellBoost{ 1.0f };
		Geometry::V g = Geometry::Hyperbolic;
		float eToHScale = 0.01f;
		float mSpeed = 0.01f;
		float mRotSpeed = 0.001f;
		std::array<glm::mat4, 6> invGens;

		glm::vec2 lastMousePos;
		glm::vec3 deltaPosState{0.0f};
		glm::vec3 deltaRotState{0.0f};
	}; // class SceneLayer : public Layer
}; // namespace App
