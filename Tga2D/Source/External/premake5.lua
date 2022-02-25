project "External"
	location "%{wks.location}/Local/"
		
	language "C++"
	cppdialect "C++17"

	targetdir ("../../Lib/")
	targetname("%{prj.name}_%{cfg.buildcfg}")
	objdir ("../../Temp/%{prj.name}/%{cfg.buildcfg}")

	--pchheader "stdafx.h"
	--pchsource "Source/%{prj.name}/stdafx.cpp"

	files {
		"**.h",
		"**.hpp",
		"**.inl",
		"**.c",
		"**.cpp",
	}

	excludes {
		"ffmpeg-2.0/**.h",
		"ffmpeg-2.0/**.c",
		"ffmpeg-2.0/**.cpp"
	}

	includedirs {
		".",
		"../tga2dcore/",
		"source/",
		"DirectXTK/Inc/",
		"ffmpeg-2.0/",
		"imgui/",
		--?? on these, spine part of assimp?
		"spine/include/",
		"vld"
	}

	libdirs { "Lib/" }

	defines {"_CONSOLE"}

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
		flags { 
			"MultiProcessorCompile"
		}
		links {
			"DXGI"
		}

		defines { "_WIN32_WINNT=0x0601" }