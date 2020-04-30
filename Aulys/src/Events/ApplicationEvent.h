#pragma once

#include "pch.h"

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
namespace Aulys
{

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: mWidth(width), mHeight(height){};

		inline unsigned int getWidth() const { return mWidth; };
		inline unsigned int getHeight() const { return mHeight; };

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent(" << mWidth << ", " << mHeight << ")";
			return ss.str();
		};

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategory::Application)
	private:
		unsigned int mWidth, mHeight;
	}; // class WindowResizeEvent


	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "WindowCloseEvent()";
			return ss.str();
		};

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategory::Application)
	private:
	}; // class WindowCloseEvent

	class DragAndDropEvent : public Event
	{
	public:
		DragAndDropEvent(const std::string& path, const std::string& extension) : mPath(path), extension(extension) {};

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "DragAndDropEvent(" << mPath << ")";
			return ss.str();
		}

		const std::string mPath;
		const std::string extension;

		EVENT_CLASS_TYPE(DragAndDrop);
		EVENT_CLASS_CATEGORY(EventCategory::Application);
	private:

	};


	// Note: The events below are currently not utilised, but are here for convenience. These might
	// actually mean something in the future e.g. when we have a seperate render thread, 
	// AppRenderEvents may actually be worth listening to.
	class AppTickEvent : public Event
	{
	public:
		AppTickEvent(){};

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategory::Application)
	}; // class AppTickEvent


	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent(){};

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategory::Application)
	}; // class AppUpdateEvent


	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent(){};

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategory::Application)
	}; // class AppRenderEvent

}; // namespace Aulys
