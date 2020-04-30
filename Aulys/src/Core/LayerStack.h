#pragma once

#include "pch.h"

#include "Layer.h"

namespace Aulys 
{
    class LayerStack 
    {
    public:
        LayerStack();
        ~LayerStack();

        void pushLayer(Layer* layer) noexcept;
        void pushOverlay(Layer* overlay) noexcept;

        void popLayer(Layer* layer) noexcept;
        void popOverlay(Layer* overlay) noexcept;

        // Allows us to use this in a range-based context. Also, just makes life good!
        std::vector<Layer*>::iterator begin() { return mLayers.begin(); };
        std::vector<Layer*>::iterator end() { return mLayers.end(); };

    private:
        std::vector<Layer*> mLayers;
        // std::vector<Layer*>::iterator mLayerInsert;
        // This tells us where to insert new layers below overlays.
        unsigned int mLayerIndex = 0;
    }; // class LayerStack
}; // namespace Aulys
