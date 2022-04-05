#pragma once
#include <Ultralight/Ultralight.h>
#include <tga2d/graphics/DX11.h>
#include <tga2d/math/vector2.h>
#include <iostream>
#include <fstream>
#include <string>



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
		RefPtr<View> CreateHTMLObject(const char* aFilePath, Vector2f anObjectSize);
		void Render(const RefPtr<View>& anObject);
		
	private:
		const ultralight::String& FetchHTML(const char* aPath);
		RefPtr<ultralight::Renderer> myRenderer;
		Config myConfig;
	};
}