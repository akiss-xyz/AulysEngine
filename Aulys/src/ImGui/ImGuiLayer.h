#pragma once

#include "Layer.h"

#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"

namespace Aulys 
{
	class ImGuiLayer : public Layer 
	{
	public:
		ImGuiLayer(const std::string& name = "ImGui Layer");
		~ImGuiLayer();

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onImGuiRender(Timestep ts) override;
		virtual void onEvent(Event& e) override;

		virtual void swapFont(const char* name, Path path, float size = 14.0f);

		void begin();
		void end();
	private:
		float mTime = 0.0f;
	}; // class ImGuiLayer : public Layer
}; // namespace Aulys
