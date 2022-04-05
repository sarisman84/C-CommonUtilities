include "../../Premake/extensions.lua"

workspace "Tutorial"
	location "../../"
	--startproject "" -- proably launcher
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Distribution"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "../External"
include "../tga2dcore"

group ("Tutorials") -- creates a filter where all tutorial projects are grouped
--include "TutorialCommon"
include "Tutorial1Init"
include "Tutorial2Sprite"
include "Tutorial3SpriteBatch"
include "Tutorial4Text"
include "Tutorial5Light"
include "Tutorial6CustomShapes"
include "Tutorial7RenderTarget"
include "Tutorial8Debug"
include "Tutorial9Spritesheets"
include "Tutorial10CustomShader"
include "Tutorial11SkinnedAnimation"
include "Tutorial12Audio"
include "Tutorial13Gamepad"
include "Tutorial14Threading"
include "Tutorial15Imgui"
include "Tutorial163DModels"
include "Tutorial17PBR"
include "Tutorial18UnityLoader"
include "Tutorial19ModelViewer"
--include "Source/Game"
--include "Source/Launcher"
