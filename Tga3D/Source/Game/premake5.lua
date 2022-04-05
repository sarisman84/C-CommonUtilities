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
		"../../../"
	}

	libdirs { "../../Lib/", "../../../Build/" }

	defines {
	}

	filter "configurations:Debug"
		defines {"_DEBUG", "TGE_DEBUG"}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "TGE_RELEASE"
		runtime "Release"
		optimize "on"
	filter "configurations:Distribution"
		defines "TGE_DIST"
		runtime "Release"
		optimize "on"

	filter "system:linux"
		kind "SharedLib"
		staticruntime "off"
		links { 
			"X11"			
		}

		defines { "TGE_SYSTEM_LINUX" }

	filter "system:windows"
		kind "StaticLib"
		staticruntime "off"
		symbols "On"		
		systemversion "latest"
		warnings "Extra"
		--conformanceMode "On"
		--buildoptions { "/permissive" }
		flags { 
		--	"FatalWarnings", -- would be both compile and lib, the original didn't set lib
			"FatalCompileWarnings",
			"MultiProcessorCompile"
		}
		
		defines {
			"WIN32",
			"_LIB", 
			"TGE_SYSTEM_WINDOWS" 
		}