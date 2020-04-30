#pragma once
#include "Aulys.h"
#include "Core/EntryPoint.h"

#include "imgui/imgui.h"

using namespace Aulys;

#include "Layers/SceneLayer.h"
#include "Layers/UIOverlay.h"

namespace App 
{
	class Sandbox : public Application 
	{
		public:
			Sandbox();

			void Run() override;
		private:

	}; // class Sandbox : public Application
}; // namespace App

// Export our application to the engine. This gets called in the actual main function, in EntryPoint
Application* Aulys::CreateApplication() { return new App::Sandbox(); }
