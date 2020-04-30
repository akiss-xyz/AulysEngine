#include "src/pch.h"

#include "Platform/Window.h"

#ifdef AU_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"

namespace Aulys {
	Uni<Window> Window::Create(const WindowProps& props) {
		return std::unique_ptr<Window>( (Window*)(new WindowsWindow(props)) );
	}
} // namespace Aulys

#elif AU_PLATFORM_LINUX
#include "Platform/Linux/LinuxWindow.h"

namespace Aulys {
	Uni<Window> Window::Create(const WindowProps& props) {
		return std::unique_ptr<Window>( (Window*)(new LinuxWindow(props)) );
	}
} // namespace Aulys

#else
namespace Aulys {
	Uni<Window> Window::Create(const WindowProps& props) {
		AU_CORE_ASSERT(false,
			"Unsupported platform error in creating a window - no AU_PLATFORM_<PLATFORM> "
			"(e.g. AU_PLATFORM_WINDOWS) defines were defined, and so a window couldn't be created."
			" Make sure you're using a supported platform, and if you are, "
			"ensure the appropriate defines are set "
			"(look in the premake5.lua file and AulysConf.h).");
		return nullptr; }
} // namespace Aulys

#endif
