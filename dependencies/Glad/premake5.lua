project "Glad"
	kind "StaticLib"
	staticruntime "on"
	systemversion "latest"
	language "C"

	targetdir ("build/" .. outputdir .. "/%{prj.name}")
	objdir ("build/int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c",
	}

    includedirs
    {
		"include",
		"include/glad/",
    }

	filter "system:linux"
		-- pic "On"

	filter "configurations:Debug"
		runtime "Debug"
		optimize "off"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
		symbols "on"

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"
		symbols "off"
