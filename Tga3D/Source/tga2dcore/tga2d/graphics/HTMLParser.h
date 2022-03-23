#pragma once

#include <Ultralight/Ultralight.h>
#include <tga2d/graphics/DX11.h>
#include <tga2d/texture/texture.h>
#include <tga2d/texture/TextureManager.h>
#include <tga2d/math/vector2.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>



using namespace ultralight;
namespace Tga2D
{
	struct Utilities
	{
		static Vector2ui ConvertToPixelUnits(Vector2f someValue)
		{
			Vector2ui size = Tga2D::Engine::GetInstance()->GetTargetSize();

			return { (unsigned int)someValue.x * size.x, (unsigned int)someValue.y * size.y };
		}

		static Vector2f ConvertToLinearUnits(Vector2ui someValue)
		{
			Vector2ui size = Tga2D::Engine::GetInstance()->GetTargetSize();

			return { (float)someValue.x / (float)size.x, (float)someValue.y / (float)size.y };
		}
	};








	class HTML_Parser
	{

	public:
		HTML_Parser();
		Texture* ParseHTMLFile(const wchar_t* aFilePath, RefPtr<View>* anViewObject = nullptr);

	private:
		void Update();
		void Render();
		Texture* CreateTextureFromHTML(void* aPixelBuffer, uint32_t aWidth, uint32_t aHeight, uint32_t aStride);
		std::vector<RefPtr<View>> myCurrentViews;
		RefPtr<ultralight::Renderer> myRenderer;
		Config myConfig;
	};
}