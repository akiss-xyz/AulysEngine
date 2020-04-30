#pragma once

#include <string>
#include <sstream>

#include "Event.h"

namespace Aulys
{
	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y) : mMouseX(x), mMouseY(y){};

		inline float getX() const { return mMouseX; };
		inline float getY() const { return mMouseY; };

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent(" << mMouseX << ", " << mMouseY << ")";
			return ss.str();
		};

		EVENT_CLASS_TYPE(MouseMoved);
		EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input);
	private:
		float mMouseX, mMouseY;
	}; // class MouseMovedEvent


	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float x, float y) : mXOffset(x), mYOffset(y){};

		inline float getXOffset() const { return mXOffset; };
		inline float getYOffset() const { return mYOffset; };

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent(" << mXOffset << ", " << mYOffset << ")";
			return ss.str();
		};

		EVENT_CLASS_TYPE(MouseScrolled);
		EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input);

	private:
		float mXOffset, mYOffset;
	}; // class MouseScrolledEvent


	class MouseButtonEvent : public Event
	{
	public:
		inline int getMouseButton() const { return mButton; };

		EVENT_CLASS_TYPE(MouseButton);
		EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input);
	protected:
		MouseButtonEvent(int button) : mButton(button){};
		int mButton;
	}; // class MouseButtonEvent


	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button) : MouseButtonEvent(button){};

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent(" << mButton << ")";
			return ss.str();
		};

		EVENT_CLASS_TYPE(MouseButtonPressed);
		EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input);
	}; // class MouseButtonPressedEvent


	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button) : MouseButtonEvent(button){};

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent(" << mButton << ")";
			return ss.str();
		};

		EVENT_CLASS_TYPE(MouseButtonReleased);
		EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input);
	}; // class MouseButtonReleasedEvent


}; // namespace Aulys
