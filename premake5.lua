workspace "Hazel_Engine"
	system "Windows"
	platforms { "x86_64" }
	architecture "x86_64"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to the root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Hazel_Engine/vendor/GLFW/include"
IncludeDir["Glad"] = "Hazel_Engine/vendor/Glad/include"
IncludeDir["glm"] = "Hazel_Engine/vendor/glm/"
IncludeDir["ImGui"] = "Hazel_Engine/vendor/imgui"
IncludeDir["stb_image"] = "Hazel_Engine/vendor/stb_image/"

group "Dependencies"
	include "Hazel_Engine/vendor/GLFW"
	include "Hazel_Engine/vendor/Glad"
	include "Hazel_Engine/vendor/imgui"
	
group ""

project "Hazel_Engine"
	location "Hazel_Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "hzpch.h"
	pchsource "Hazel_Engine/src/hzpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm**.hpp",
		"%{prj.name}/vendor/glm/glm**.inl",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.inl"
	}
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/src/Hazel",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.ImGui}"
	}
	
	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "platforms:x86_64"
		architecture "x86_64"
		system "Windows"
		systemversion "latest"

		defines
		{
			"HZ_PLATFORM_WINDOWS",
			"HZ_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Hazel_Engine/vendor/spdlog/include",
		"Hazel_Engine/vendor/glm/",
		"Hazel_Engine/src",
		"Hazel_Engine/src/Hazel",
		"Hazel_Engine/vendor/"
	}

	links
	{
		"Hazel_Engine"
	}

	filter "platforms:x86_64"
		architecture "x86_64"
		system "Windows"
		systemversion "latest"

		defines
		{
			"HZ_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "on"
