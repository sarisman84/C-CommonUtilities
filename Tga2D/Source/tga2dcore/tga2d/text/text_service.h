/*
This class handles all texts that will be rendered, don't use this to show text, only use the CText class
*/

#pragma once

#include <tga2d/engine_defines.h>
#include <tga2d/math/color.h>
#include <tga2d/text/fontfile.h>
#include <tga2d/text/text.h>
#include <unordered_map>
#include <vector>

namespace Tga2D
{
	class CSpriteBatch;
	class CTexture;
	class CText;
	class CTextService
	{
	public:
		CTextService();
		~CTextService();

		void Init();

		SFont GetOrLoad(std::string aFontPathAndName, unsigned char aBorderSize, int aSize);
		bool Draw(Tga2D::CText& aText, Tga2D::CSpriteShader* aCustomShaderToRenderWith = nullptr);
		float GetSentenceWidth(Tga2D::CText& aText);
		float GetSentenceHeight(Tga2D::CText& aText);

	private:
		struct FT_LibraryRec_* myLibrary;

		std::unordered_map<std::string, std::weak_ptr<CInternalTextAndFontData>> myFontData;
	};
}