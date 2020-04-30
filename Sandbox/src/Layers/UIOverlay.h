#pragma once

#include "Aulys.h"

#include "Geometry/Maths.h"
#include "Events/AppEvent.h"

#include "Log/ConsoleSink.h"

using namespace Aulys;

namespace App 
{
	class UIOverlay : public Layer 
	{
	public:
		UIOverlay(const Ref<Texture2D> renderTexture, const std::string& name = "UIOverlay") : Layer(name), mRenderTexture(renderTexture) {};
		~UIOverlay() { };

		virtual void onAttach() override;
		virtual void onEvent(Event& event) override;
		virtual void onImGuiRender(Timestep ts) override;
		//virtual void onUpdate(Timestep ts) override;

		bool updateUniformsFromUI(GeometryChangedEvent& e);
	private:
		const Ref<Texture2D> mRenderTexture;
		Ref<Texture2D> testText;

		bool mDebugInfo = false;
		std::array<float, 2> mDebugVector = { 1920.0f, 1080.0f };
		std::array<float, 2> mDebugVector2 = { 0.0f, 0.0f };

		std::array<float, 2> mSize = {1920.0f, 1080.0f};
		float mFov = 90.0f;
		float mScale = 4.0f;

		int mSteps = 29;
		float mMaxDist = 6.4f;

		int mAttenuation = 1;
		float mAttnSpd = 0.05f;
		std::array<bool, 2> mRenderShads = {false, false};
		float mShadowSoftness = 0.25f;
		float mAmbient = 0.8f;

		/* Scene Geometry Settings */
		std::array<int, 3> pqr = {4,3,6};
		float mTubeRad = 0.15f;

		// UI Configuration settings, there are just TEMPorary.
		float mSizeSpd = 0.05f;
		float mScaleSpd = 0.05f;
		float mMaxStepsSpd = 0.05f;
		int mMaxValOfMaxSteps = 300;
		float mMaxValOfMaxDist = 100.0f;
		float mTubeRadSpeed = 0.05f;
		float pqrSpd = 0.05f;
		float mAmbientSpd = 0.10f;
		float mSpeedSpd = 0.10f;
		int pqrMin = 0; int pqrMax = 100;

		float hCWK = defhCWK;
		float hCWH = defhCWH;
		float mCellSurfaceOffset = 0.0f;

		glm::vec4 mVertexPosition;
		float mVertexSurfaceOffset;
		float mTargetFPS;
		uint32_t mMaxSteps;

		glm::vec4 mCellPosition;

		float mSpeed = 0.01f;
		float mRotSpeed = 0.001f;
		float fontScale = 1.0f;

		std::shared_ptr<ConsoleSink> consoleSink = std::make_shared<ConsoleSink>();
		Ref<Texture2D> mCurrentTexture;
	}; // class UIOverlay : public Layer

}; // namespace App
