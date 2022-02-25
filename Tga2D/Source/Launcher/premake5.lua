project "Launcher"
	location "%{wks.location}/Local/"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++17"

	targetdir ("../../Bin/")
	targetname("%{prj.name}_%{cfg.buildcfg}")
	objdir ("../../Temp/%{prj.name}/%{cfg.buildcfg}")

	debugdir "../../Bin/"

	files {
		"**.h",
		"**.cpp",
	}

	includedirs {
		"../tga2dcore",
		"../Game/",
		"../../../"
	}

	libdirs { 
	"../../Lib/",
	"../../../Build"
	}

	links { 
		"avcodec.lib",
		"avdevice.lib",
		"avfilter.lib",
		"avformat.lib",
		"avutil.lib",
		"postproc.lib",
		"swresample.lib",
		"swscale.lib",
		"Game",
		"tga2dcore"
	}

	defines {"_CONSOLE"}
	
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

	systemversion "latest"

	filter "system:windows"
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