#pragma once

#include "Platform/Linux/LinuxInput.h"
#include "Input/Input.h"

namespace Aulys
{
	class LinuxInput : public Input 
	{
	public:

	protected:
		virtual bool isKeyPressedImpl(int keycode) override;
		virtual bool isMouseButtonPressedImpl(int keycode) override;
		virtual float getMouseXImpl() override;
		virtual float getMouseYImpl() override;
		virtual std::pair<float, float> getMousePosImpl() override;

	private:
	}; // class LinuxInput : public Input
}; // namespace Aulys 
