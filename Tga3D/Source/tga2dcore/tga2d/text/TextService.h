/*
This class handles all texts that will be rendered, don't use this to show text, only use the Text class
*/

#pragma once

#include <tga2d/EngineDefines.h>
#include <tga2d/math/color.h>
#include <tga2d/text/fontfile.h>
#include <tga2d/text/text.h>
#include <unordered_map>
#include <vector>

namespace Tga2D
{
	class Texture;
	class Text;
	class TextService
	{
	public:
		TextService();
		~TextService();

		void Init();

		Font GetOrLoad(std::wstring aFontPathAndName, unsigned char aBorderSize, int aSize);
		bool Draw(Tga2D::Text& aText, Tga2D::SpriteShader* aCustomShaderToRenderWith = nullptr);
		float GetSentenceWidth(Tga2D::Text& aText);
		float GetSentenceHeight(Tga2D::Text& aText);

	private:
		struct FT_LibraryRec_* myLibrary;

		std::unordered_map<std::wstring, std::weak_ptr<InternalTextAndFontData>> myFontData;
	};
}