require "export-compile-commands"

-- Guaranteed defines:
	-- AU_PLATFORM_<PLATFORM>
		-- e.g. AU_PLATFORM_LINUX, AU_PLATFORM_WINDOWS
		-- Is guaranteed by this premake5.lua file.

	-- AU_COMPILER_<COMPILER>
		-- e.g. AU_COMPILER_GCC, AU_COMPILER_MSVC, AU_COMPILER_CLANG, AU_COMPILER_UNKNOWN
		-- Is guaranteed by AulysConf.h
			-- Works by checking __GNUC__ and _MSC_VER.

workspace "Aulys"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.architecture}"

include "dependencies/GLFW"
include "dependencies/Glad"
include "dependencies/imgui"

project "Aulys"
	location "Aulys"
	kind "StaticLib"
	staticruntime "on"

	language "C++"
	cppdialect "C++17"

	targetdir ("build/" .. outputdir .. "/%{prj.name}")
	objdir ("build/int/" .. outputdir .. "/%{prj.name}")

	pchheader "src/pch.h"
	pchsource "src/pch.cpp"

	files {
		"Aulys/**.cpp",
		"Aulys/**.h",
		"Aulys/**.hpp",
		"dependencies/stb/**.h",
		"dependencies/stb/**.cpp",
	}

	includedirs {
		"Aulys",
		"Aulys/src/",
		"dependencies/spdlog/include/",
		"dependencies/GLFW/include/",
		"dependencies/Glad/include/",
		"dependencies/imgui",
		"dependencies/glm/",
		"dependencies/stb/", -- [Textures] Image loading.
		"dependencies/",
	}

	links {
		 "Glad",
		 "GLFW",
		 "ImGui",
	}

	defines {
		"GLFW_INCLUDE_NONE",
	}

	buildoptions {
	}

	filter "system:windows"
		defines {
		  "AU_PLATFORM_WINDOWS",
		  "AU_BUILD_DLL"
		}

	filter "system:linux"
		links {
			-- Included by the client app.
			-- "Xrandr",
			-- "Xi",
			-- "GL",
             -- "dl",
             -- "pthread",
			"X11",
		}

	defines {
		"AU_PLATFORM_LINUX",
		"AU_BUILD_SO"
	}

	filter "configurations:Debug"
		defines "AU_DEBUG"
		runtime "Debug"


		optimize "off"
		symbols "on"

	filter "configurations:Release"
		defines "AU_RELEASE"
		runtime "Release"

		optimize "on"
		symbols "on"


	filter "configurations:Dist"
		defines "AU_DIST"
		runtime "Release"

		optimize "on"
		symbols "off"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	staticruntime "on"

	language "C++"
	cppdialect "C++17"

	targetdir ("build/" .. outputdir .. "/%{prj.name}")
	objdir ("build/int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/**.h",
		"%{prj.name}/**.hpp",
	}

	includedirs {
		"Aulys/src/",
		"Sandbox/src/",
		"dependencies/spdlog/include/",
		"dependencies/spdlog/include/spdlog/",
		"dependencies/imgui/",
		"dependencies/glm/",
		"dependencies/stb/",
		"dependencies/",
	}

	links {
		"Aulys",
		"Glad",
		"GLFW",
		"ImGui",
	}

	defines {
	}

	filter "system:windows"
		defines {
			"AU_PLATFORM_WINDOWS",
		}

	filter "system:linux"
		links {
			"X11",
			"dl",
			"pthread",
		}

		defines {
			"AU_PLATFORM_LINUX",
			"SANDBOX_AU"
		}


	filter "configurations:Debug"
		defines "AU_DEBUG"
		runtime "Debug"

		optimize "off"
		symbols "on"

	filter "configurations:Release"
		defines "AU_RELEASE"
		runtime "Release"

		optimize "on"
		symbols "on"

	filter "configurations:Dist"
		defines "AU_DIST"
		runtime "Release"

		optimize "on"
		symbols "off"

