#include "Sandbox.h"

namespace App 
{
	Sandbox::Sandbox() : Application({"A real hyperbolic mess.", 1920, 1080}){
		// this->getWindow().setFullscreen();
		Ref<Texture2D> renderTexture = Texture2D::create(this->getWindow().getWidth(),
		                                                 this->getWindow().getHeight());
		this->pushOverlay(new UIOverlay(renderTexture));
		// Manages input and output, communicates changes to SceneLayer through events.
		this->pushLayer(new SceneLayer(renderTexture));
			// Manages rendering, assets, etc.
	}

	void Sandbox::Run() { // The central function of the whole program.
		while (this->mRunning) {
			Timestep deltaTime = this->mWindow->calculateDeltaTime(mLastFrameTime);

			for (Layer* layer : this->mLayerStack) {
				layer->onUpdate(deltaTime);
			}

			mImGuiLayer->begin();
			for (Layer* layer : this->mLayerStack) {
				layer->onImGuiRender(deltaTime);
			}
			mImGuiLayer->end();
			mWindow->onUpdate();
		}
	}
}; // namespace App
