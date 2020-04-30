#pragma once

#include "pch.h"

#include "Event.h"

namespace Aulys
{
	class KeyEvent : public Event
	{
	public:
		inline int getKeyCode() const { return mKeyCode; };

		EVENT_CLASS_TYPE(Key);
		EVENT_CLASS_CATEGORY(EventCategory::Keyboard | EventCategory::Input);

	protected:
		KeyEvent(int keycode) : mKeyCode(keycode){};

		int mKeyCode;
	}; // class KeyEvent


	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode), mRepeatCount(repeatCount){};

		inline int getRepeatCount() const { return mRepeatCount; };

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent(" << mKeyCode << " '" << (char)mKeyCode << "', "
				<< mRepeatCount << " reps)";
			return ss.str();
		};

		EVENT_CLASS_TYPE(KeyPressed);
		EVENT_CLASS_CATEGORY(EventCategory::Keyboard | EventCategory::Input);

	private:
		int mRepeatCount = 0;
	}; // class KeyPressedEvent


	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode)
			: KeyEvent(keycode){};

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent(" << mKeyCode << ")";
			return ss.str();
		};

		EVENT_CLASS_TYPE(KeyTyped);
		EVENT_CLASS_CATEGORY(EventCategory::Keyboard | EventCategory::Input);

	private:
	}; // class KeyTypedEvent


	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode) : KeyEvent(keycode){};

	std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent(" << mKeyCode << ")";
			return ss.str();
		};


		EVENT_CLASS_TYPE(KeyReleased)
		EVENT_CLASS_CATEGORY(EventCategory::Keyboard | EventCategory::Input);
	private:

	}; // class KeyReleasedEvent

}; // namespace Aulys
