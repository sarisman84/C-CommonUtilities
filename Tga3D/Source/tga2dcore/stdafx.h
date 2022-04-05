#pragma once
#pragma message("-------Never give up your dreams!-------------")
#include "targetver.h"
#include <tga2d/error/ErrorManager.h>

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif

#include <tga2d/math/matrix4x4.h>
#include <tga2d/math/vector2.h>
#include <tga2d/math/vector4.h>
#include <tga2d/sprite/sprite.h>
#include <tga2d/shaders/shader.h>

#include <algorithm>
#include <array>
#include <exception>
#include <malloc.h>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"