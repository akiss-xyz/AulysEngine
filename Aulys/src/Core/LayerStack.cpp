#include "src/pch.h"

#include "LayerStack.h"

namespace Aulys 
{
	LayerStack::LayerStack() {
		// this->mLayerInsert = this->mLayers.begin();
	};

	LayerStack::~LayerStack()
	{
		for (Layer* layer : this->mLayers)
			delete layer;
	};

	void LayerStack::pushLayer(Layer* layer) noexcept
	{
		mLayers.emplace(mLayers.begin() + mLayerIndex, layer);
		mLayerIndex++;
	};

	void LayerStack::pushOverlay(Layer* overlay) noexcept
	{
		this->mLayers.emplace_back(overlay);
	};

	void LayerStack::popLayer(Layer* layer) noexcept 
	{
		auto it = std::find(mLayers.begin(), mLayers.end(), layer);
		if (it != mLayers.end())
		{
			mLayers.erase(it);
			mLayerIndex--;
		}
	};

	void LayerStack::popOverlay(Layer* overlay) noexcept 
	{
		auto it = std::find(mLayers.begin(), mLayers.end(), overlay);
		if (it != mLayers.end())
		{
			mLayers.erase(it);
		}
	};
		
}; // namespace Aulys

