project "tga2dcore"
	location "%{wks.location}/../Local/"
		
	language "C++"
	cppdialect "C++17"

	targetdir ("../../Lib/")
	targetname("%{prj.name}_%{cfg.buildcfg}")
	objdir ("../../Temp/%{prj.name}/%{cfg.buildcfg}")

	pchheader "stdafx.h"
	pchsource "stdafx.cpp"

	files {
		"**.h",
		"**.cpp",
	}

	includedirs {
		".",
		"../External/",
		"source/",
		"../External/DirectXTK/Inc/",
		"../spine/include/", -- what is this? is it used?
		"../External/ffmpeg-2.0/",
		"vld" -- what is this? is it used?
	}

	filter "configurations:Debug"
		defines {"_DEBUG", "TGE_DEBUG"}
		runtime "Debug"
		symbols "on"
	filter "configurations:Release"
		defines "_RELEASE"
		runtime "Release"
		optimize "on"
	filter "configurations:Retail"
		defines "_RETAIL"
		runtime "Release"
		optimize "on"

	filter "system:windows"
		kind "StaticLib"
		staticruntime "off"
		symbols "On"		
		systemversion "latest"
		warnings "Extra"
		sdlchecks "true"
		flags { 
			"FatalCompileWarnings",
			"MultiProcessorCompile"
		}
		links {
			"DXGI"
		}

		defines {
			"WIN32",
			"_CRT_SECURE_NO_WARNINGS", 
			"_LIB", 
			"_WIN32_WINNT=0x0601",
			"TGE_SYSTEM_WINDOWS" 
		}