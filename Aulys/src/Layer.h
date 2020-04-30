
#pragma once

#include "pch.h"

#include "Events/Event.h"

#include "Core/Timestep.h"

namespace Aulys 
{
	class Layer 
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void onAttach() {};
		virtual void onDetach() {};
		virtual void onUpdate(Timestep ts) {};
		virtual void onImGuiRender(Timestep ts) {};
		virtual void onEvent(Event& event) {};

		inline const std::string& getName() const { return mName; };
	private:
		std::string mName;
	}; // class Layer

}; // namespace Aulys
