#pragma once

#include "Core/Core.h"

namespace Aulys 
{
	class Input 
	{
	public:
		inline static bool isKeyPressed(int keycode) {
			return sInstance->isKeyPressedImpl(keycode);
		} 

		inline static bool isMouseButtonPressed(int button) {
			return sInstance->isMouseButtonPressedImpl(button);
		} 

		inline static std::pair<float, float> getMousePos() {
			return sInstance->getMousePosImpl();
		}

		inline static float getMouseX() {
			return sInstance->getMouseXImpl();
		} 

		inline static float getMouseY() {
			return sInstance->getMouseYImpl();
		} 

	protected:
		virtual bool isKeyPressedImpl(int keycode) = 0;
		virtual bool isMouseButtonPressedImpl(int keycode) = 0;
		virtual std::pair<float, float> getMousePosImpl() = 0;
		virtual float getMouseXImpl() = 0;
		virtual float getMouseYImpl() = 0;


	private:
		static Input* sInstance;

	}; // class Input
}; // namespace Aulys
