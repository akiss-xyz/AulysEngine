#pragma once

#include "Core/Core.h"
#include "Core/Timestep.h"

#include "Events/Event.h"

namespace Aulys
{

	struct WindowProps
	{
		std::string title;
		unsigned int width, height;

		WindowProps(const std::string& Title = "Aulys Engine! Eat your veggies.",
					unsigned int Width = 1920, unsigned int Height = 1080) // Changed to 1920x1080
			: title(Title), width(Width), height(Height){};
	}; // struct WindowProps


  // An interface representing a desktop window, abstracted across the supported platforms. There's
  // no way to contruct an instance - the corresponding Window::Create function must be provided by
  // inheriting classes.
	class Window
	{
	public:
		virtual ~Window() = default;

		virtual void onUpdate() = 0;

		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;
		virtual bool isFullscreen() const = 0;
		virtual void setFullscreen(bool fullscreen = true) = 0;

		virtual void setEventCallback(const std::function<void(Event&)> callback) = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;
		virtual Timestep calculateDeltaTime(float& lastFrameTime) const = 0;

		static Uni<Window> Create(const WindowProps& props = WindowProps());

        // A function that exposes access to the underlying system-specific window being used,
        // intended primarily to be used as a way to access functionality Aulys doesn't provide
        // yet, or functionality that cannot be provided at a high level of abstraction.
        // Currently, this could be any of: 
        // GLFWwindow*
        //
        // In the future, this could be pointers to direct Windows API windows, etc.
        virtual void* getNativeWindow() const = 0;
	private:
	}; // class Window

}; // namespace Aulys
