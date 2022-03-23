project "Tutorial-15_Imgui"
	location "%{wks.location}/Local/"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir ("%{wks.location}/Bin/")
	targetname("%{prj.name}_%{cfg.buildcfg}")
	objdir ("%{wks.location}/Temp/%{prj.name}/%{cfg.buildcfg}")

	debugdir "%{wks.location}/Bin/"

	files {
		"**.h",
		"**.cpp",
	}

	includedirs {
		"../../tga2dcore",
		"../TutorialCommon",
		"../../External",
		"source/"
	}

	libdirs { "%{wks.location}/Lib/" }

	links {
		"tga2dcore"
	}

	defines {"_CONSOLE"}
	
	filter "configurations:Debug"
		defines {"_DEBUG"}
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

	systemversion "latest"
	
	filter "system:linux"
		debugenvs { "LD_LIBRARY_PATH=../bin/Debug-linux-x86_64/" }
		defines { "TGE_SYSTEM_LINUX" }

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
