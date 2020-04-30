#pragma once

#include "pch.h"
#include <spdlog/fmt/ostr.h>

#include "Core/Core.h"

namespace Aulys
{
	// clang-format off

	// This class describes what an event represents. This'll be queried by getEventType and such so
	// we can deal with events polymorphically and still be able to tell what it is.
	namespace EventType
	{
		enum V {
			None = 0,
			AppDefined,
			WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved, DragAndDrop,
			AppTick, AppUpdate, AppRender,
			Key, KeyPressed, KeyTyped, KeyReleased,
			MouseButton, MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
		}; // enum V
	}; // namespace EventType

	// This allows you to filter events, e.g. maybe you only want to log every mouse event that
	// happens -  you don't want to have to check all 4 types of mouse events that can happen, that's
	// a bit annoying, so hence we have this enum to keep track of categories.
	namespace EventCategory 
	{
		enum V {
			None            = 0,
			Application     = 1 << 0,
			Input           = 1 << 1,
			Keyboard        = 1 << 2,
			Mouse           = 1 << 3,
			MouseButton     = 1 << 4,
			AppDefined      = 1 << 5, // Defined for use by the client app.
			// This can be used to tell whether it's safe to cast back into your Event type.
		}; // enum V
	}; // namespace EventCategory

	// clang-format on

#define EVENT_CLASS_TYPE(type)\
	static bool isCustomStatic() { return false; }\
	virtual bool isCustomEvent() const override { return isCustomStatic(); }\
	static uint32_t getStaticType() { return EventType::V::type; }\
	virtual uint32_t getEventType() const override { return getStaticType(); }\
	virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
	virtual uint32_t getCategoryFlags() const override { return category; }\
	virtual uint32_t getCustomCategoryFlags() const override { return 0; }

// yourTypeEnum - a uint32_t that is the ID of your type. It gets compared against
// T::getCustomStaticType() in the EventDispatcher class below, so make sure your
// EventType-inheriting classes override that static function 
	// !!! Note: this shouldn't ever be 0, because that value represents to Aulys that this isn't in
	// fact a custom type, and hence your custom event will never be caught by anything.
#define EVENT_CUSTOM_TYPE(yourTypeEnum)\
	static bool isCustomStatic() { return true; }\
	virtual bool isCustomEvent() const override { return isCustomStatic(); }\
	static uint32_t getStaticType() { return yourTypeEnum; }\
	virtual uint32_t getEventType() const override { return getStaticType(); }\
	virtual const char* getName() const override { return #yourTypeEnum; }\

#define EVENT_CUSTOM_CATEGORY(yourCustomCategory)\
	virtual uint32_t getCategoryFlags() const override { return 0; }\
	virtual uint32_t getCustomCategoryFlags() const override { return yourCustomCategory; }

#define EVENT_CUSTOM_EXTEND_CATEGORY(newCategories, parent)\
	virtual uint32_t getCustomCategoryFlags() const override {\
		return parent::getCustomCategoryFlags() || newCategories;\
	}

	class Event
	{
		friend class EventDispatcher;
	public:
		virtual bool isCustomEvent() const = 0; // Used to decide if an event is an Aulys intrinsic.
		virtual uint32_t getEventType() const = 0;
			// Note: The value 0 is reserved for "I'm not a custom event type.", it signals to Au
			// to not bother checking this type for any custom dispatchers. See EventDispatcher
			// below.
		virtual const char* getName() const { return "Event()"; } // Potentially only in debug mode.
		virtual uint32_t getCategoryFlags() const { return 0; };
		virtual uint32_t getCustomCategoryFlags() const { return 0; };
		virtual std::string toString() const { return getName(); };

		inline bool isInCategory(EventCategory::V category) const
			{ return getCategoryFlags() & category; };
		// Can be overriden if you like, to do some crazy stuff!
		virtual inline bool isInCustomCategory(uint32_t category) const
			{ return getCustomCategoryFlags() & category; };

		bool handled = false;
	protected:

	}; // class Event

	// Notes:
	// Dispatching to member functions is a challenge, but it's a challenge worth overcoming,
	// because access to our instances would be lovely from event handling functions. The way to go
	// about this is like so:
	// ` (...) myClass::onEvent(Event& e) {
	//     EventDispatcher d(e);
	//     d.dispatch<myCustomEventType>(
	//         [this](myCustomEventType& e){ return this->handleMyCustomEventType(e); }
	//     );
	// } `
	// This way, you have full access to your instance, no problem! Lovely stuff!
	
	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event) : mEvent(event){};

		template <typename T>
		bool dispatch(std::function<bool(T&)> func)
		{
			if (mEvent.isCustomEvent() == T::isCustomStatic()) {
				if (mEvent.getEventType() == T::getStaticType()) {
					func( *dynamic_cast<T*>(&mEvent));
				}
			}
			return false;
		}

	private:
		Event& mEvent;
	}; // class EventDispatcher

	std::ostream& operator<<(std::ostream& os, const Event& e);

}; // namespace Aulys
