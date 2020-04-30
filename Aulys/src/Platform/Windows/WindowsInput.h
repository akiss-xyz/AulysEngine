#pragma once

#include "Platform/Windows/WindowsInput.h"
#include "Input/Input.h"

namespace Aulys
{
	class WindowsInput : public Input 
	{
	public:

	protected:
		virtual bool isKeyPressedImpl(int keycode) override;
		virtual bool isMouseButtonPressedImpl(int keycode) override;
		virtual float getMouseXImpl() override;
		virtual float getMouseYImpl() override;
		virtual std::pair<float, float> getMousePosImpl() override;

	private:
	}; // class WindowsInput : public Input
}; // namespace Aulys 
