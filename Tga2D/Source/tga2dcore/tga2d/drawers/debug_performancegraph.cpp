#include "stdafx.h"

#include <tga2d/drawers/debug_performancegraph.h>
#include <tga2d/drawers/debug_drawer.h>
#include <tga2d/engine.h>
#include <tga2d/d3d/direct_3d.h>
#include <tga2d/sprite/sprite.h>
#include <tga2d/primitives/custom_shape.h>
#include <tga2d/text/Text.h>

#define WIN32_LEAN_AND_MEAN 
#define NOMINMAX 
#include <windows.h>

#include <stdio.h>
#include <psapi.h>


using namespace Tga2D;

CPerformanceGraph::CPerformanceGraph(CDebugDrawer* aDrawer)
	:myDrawer(aDrawer)
	, myBackground(nullptr)
{
}


CPerformanceGraph::~CPerformanceGraph(void)
{}

void CPerformanceGraph::Init(Tga2D::CColor& aBackgroundColor, Tga2D::CColor& aLineColor, const std::string& aText)
{
	myBackground = std::make_unique<CCustomShape>();
	myLineColor = aLineColor;
	const float sizeX = -0.4f;
	const float sizeY = 0.11f;
	const VECTOR2F position(1, -0.01f);

	myBackground->AddPoint(VECTOR2F(position.x, position.y), aBackgroundColor);
	myBackground->AddPoint(VECTOR2F(position.x + sizeX, position.y), aBackgroundColor);
	myBackground->AddPoint(VECTOR2F(position.x + sizeX, position.y + sizeY), aBackgroundColor);

	myBackground->AddPoint(VECTOR2F(position.x, position.y), aBackgroundColor);
	myBackground->AddPoint(VECTOR2F(position.x, position.y + sizeY), aBackgroundColor);
	myBackground->AddPoint(VECTOR2F(position.x + sizeX, position.y + sizeY), aBackgroundColor);
	myBackground->BuildShape();

	myText = std::make_unique<Tga2D::CText>("Text/arial.ttf");
	myText->SetText("--");
	myText->SetPosition(VECTOR2F(0.6f, 0.02f));
	myText->SetColor({ 1, 1, 1, 1.0f });
	myText->SetScale(1.0f);
	myText->SetText(aText);
}


void CPerformanceGraph::FeedValue(int aValue)
{
	if (myBuffer.size() > DEBUG_PERFGRAPH_SAMPLES)
	{
		myBuffer.erase(myBuffer.begin());
	}
	myBuffer.push_back(aValue);
}


void CPerformanceGraph::Render()
{
	if (!CEngine::GetInstance()->IsDebugFeatureOn(EDebugFeature::FpsGraph))
	{
		return;
	}
	float startX = 0.6f;
	float startY = 0.10f;

	int maxVal = 0;
	int minVal = INT_MAX;

	for (unsigned int i = 0; i < myBuffer.size(); i++)
	{
		if (myBuffer[i] > maxVal)
		{
			maxVal = myBuffer[i];
		}
		if (myBuffer[i] < minVal)
		{
			minVal = myBuffer[i];
		}

	}

	float increaseX = 0.4f / (float)DEBUG_PERFGRAPH_SAMPLES;

	float lastY = startY;

	std::vector<VECTOR2F> theTos;
	std::vector<VECTOR2F> theFrom;
	std::vector<Tga2D::CColor> theColors;
	theTos.reserve(myBuffer.size());
	theFrom.reserve(myBuffer.size());
	theColors.reserve(myBuffer.size());

	for (unsigned int i = 0; i < myBuffer.size(); i++)
	{
		float thisY = (float)myBuffer[i] / (float)maxVal;

		thisY = thisY / 10.0f;

		float incrWithI = increaseX * ((float)i+1);

		theTos.push_back(VECTOR2F(startX + incrWithI, lastY));
		theFrom.push_back(VECTOR2F((startX + incrWithI) + increaseX, thisY));
		theColors.push_back(myLineColor);
	
		
		lastY = thisY;
	}
	myBackground->Render();
	if (theTos.size() > 0)
	{		
		myDrawer->DrawLines(&theTos[0], &theFrom[0], &theColors[0], static_cast<int>(theTos.size()));
	}
	
	myText->Render();

}
