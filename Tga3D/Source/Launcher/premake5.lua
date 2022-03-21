project "Launcher"
	location "%{wks.location}/Local/"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++17"
	--staticruntime "on"

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
		"../../../",
		"../../../Ultralight/include/"
	}

	libdirs { 
	"../../Lib/", 
	"../../../Build/",
	"../../../Ultralight/lib"
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
		"tga2dcore",
		"AppCore.lib",
		"Ultralight.lib",
		"UltralightCore.lib",
		"WebCore.lib"
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
		defines "TGE_RELEASE"
		runtime "Release"
		optimize "on"
		links{
			"CommonUtilities-r.lib"
		}

	filter "configurations:Distribution"
		defines "TGE_DIST"
		runtime "Release"
		optimize "on"
		links{
			"CommonUtilities-r.lib"
		}

	systemversion "latest"
	
	filter "system:linux"
		debugenvs { "LD_LIBRARY_PATH=../bin/Debug-linux-x86_64/" }
		defines { "TGE_SYSTEM_LINUX" }

	filter "system:windows"
		symbols "On"		
		systemversion "latest"
		warnings "Extra"
		-- sdlchecks "true"
		--conformanceMode "On"
		--buildoptions { "/STACK:8000000" }
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