#include "stdafx.h"

#include <tga2d/text/text.h>
#include <tga2d/engine.h>
#include <tga2d/text/TextService.h>

using namespace Tga2D;

Text::Text(const wchar_t* aPathAndName, FontSize aFontSize, unsigned char aBorderSize)
: myTextService(&Tga2D::Engine::GetInstance()->GetTextService())
{
	myColor.Set(1, 1, 1, 1);
	myScale = 1.0f;
	myPathAndName = aPathAndName;
	myFont = myTextService->GetOrLoad(myPathAndName, aBorderSize, aFontSize);
	myRotation = 0.0f;
}

Text::~Text() {}

void Tga2D::Text::Render()
{
	if (!myTextService)
	{
		return;
	}
	if (!myTextService->Draw(*this))
	{
		ERROR_PRINT("%s", "Text rendering error! Trying to render a text where the resource has been deleted! Did you clear the memory for this font? OR: Did you set the correct working directory?");
	}
}

void Tga2D::Text::Render(Tga2D::SpriteShader* aCustomShaderToRenderWith)
{
	if (!myTextService)
	{
		return;
	}
	if (!myTextService->Draw(*this, aCustomShaderToRenderWith))
	{
		ERROR_PRINT("%s", "Text rendering error! Trying to render a text where the resource has been deleted! Did you clear the memory for this font? OR: Did you set the correct working directory?");
	}
}

float Tga2D::Text::GetWidth()
{
	if (!myTextService)
	{
		return 0.0f;
	}

	return myTextService->GetSentenceWidth(*this) / Engine::GetInstance()->GetWindowSize().x;
}

float Tga2D::Text::GetHeight()
{
	if (!myTextService)
	{
		return 0.0f;
	}

	return myTextService->GetSentenceHeight(*this) / Engine::GetInstance()->GetWindowSize().y;
}

void Tga2D::Text::SetColor(const Color& aColor)
{
	myColor = aColor;
}

Tga2D::Color Tga2D::Text::GetColor() const
{
	return myColor;
}

void Tga2D::Text::SetText(const std::string& aText)
{
	myText = aText;
}

std::string Tga2D::Text::GetText() const
{
	return myText;
}

void Tga2D::Text::SetPosition(const Vector2f& aPosition)
{
	myPosition = aPosition;
}

Vector2f Tga2D::Text::GetPosition() const
{
	return myPosition;
}

void Tga2D::Text::SetScale(float aScale)
{
	myScale = aScale;
}

float Tga2D::Text::GetScale() const
{
	return myScale;
}

std::wstring Tga2D::Text::GetPathAndName() const
{
	return myPathAndName;
}
