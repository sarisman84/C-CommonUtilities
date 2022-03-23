#include "stdafx.h"

#include <tga2d/text/text.h>
#include <tga2d/engine.h>
#include <tga2d/text/text_service.h>

using namespace Tga2D;

CText::CText(const char* aPathAndName, EFontSize aFontSize, unsigned char aBorderSize)
: myTextService(&Tga2D::CEngine::GetInstance()->GetTextService())
{
	myColor.Set(1, 1, 1, 1);
	myScale = 1.0f;
	myPathAndName = aPathAndName;
	myFont = myTextService->GetOrLoad(myPathAndName, aBorderSize, aFontSize);
	myRotation = 0.0f;
}

CText::~CText() {}

void Tga2D::CText::Render()
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

void Tga2D::CText::Render(Tga2D::CSpriteShader* aCustomShaderToRenderWith)
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

float Tga2D::CText::GetWidth()
{
	if (!myTextService)
	{
		return 0.0f;
	}

	return myTextService->GetSentenceWidth(*this) / CEngine::GetInstance()->GetWindowSize().x;
}

float Tga2D::CText::GetHeight()
{
	if (!myTextService)
	{
		return 0.0f;
	}

	return myTextService->GetSentenceHeight(*this) / CEngine::GetInstance()->GetWindowSize().y;
}

void Tga2D::CText::SetColor(const CColor& aColor)
{
	myColor = aColor;
}

Tga2D::CColor Tga2D::CText::GetColor() const
{
	return myColor;
}

void Tga2D::CText::SetText(const std::string& aText)
{
	myText = aText;
}

std::string Tga2D::CText::GetText() const
{
	return myText;
}

void Tga2D::CText::SetPosition(const VECTOR2F& aPosition)
{
	myPosition = aPosition;
}

VECTOR2F Tga2D::CText::GetPosition() const
{
	return myPosition;
}

void Tga2D::CText::SetScale(float aScale)
{
	myScale = aScale;
}

float Tga2D::CText::GetScale() const
{
	return myScale;
}

std::string Tga2D::CText::GetPathAndName() const
{
	return myPathAndName;
}
