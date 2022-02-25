project "Game"
	location "%{wks.location}/Local/"
		
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
		"../tga2dcore",
		"../External/",
		"../External/DirectXTK/Inc/",
		"../../../"
	}

	libdirs { 
	"../../Lib/",
	"../../../Build/"
	}

	defines {
	}

	filter "configurations:Debug"
		defines {"_DEBUG", "TGE_DEBUG"}
		runtime "Debug"
		symbols "on"
		links{
			"CommonUtilities-d.lib"
		}
	filter "configurations:Release"
		defines "_RELEASE"
		runtime "Release"
		optimize "on"
		links{
			"CommonUtilities-r.lib"
		}
	filter "configurations:Retail"
		defines "_RETAIL"
		runtime "Release"
		optimize "on"
		links{
			"CommonUtilities-r.lib"
		}

	filter "system:windows"
		kind "StaticLib"
		staticruntime "off"
		symbols "On"		
		systemversion "latest"
		warnings "Extra"
		flags { 
			"FatalCompileWarnings",
			"MultiProcessorCompile"
		}
		
		defines {
			"WIN32",
			"_LIB", 
			"TGE_SYSTEM_WINDOWS" 
		}