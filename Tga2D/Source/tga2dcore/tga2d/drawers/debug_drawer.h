#pragma once
#include <memory>
#include <vector>
#include <tga2d/math/vector2.h>
#include <tga2d/math/vector4.h>
#include <tga2d/math/color.h>

#ifndef _RETAIL
#define DEBUG_DRAW_LINE(aFrom, aTo, aColor) Tga2D::CDebugDrawer::DrawLine(aFrom, aTo, aColor);
#define DEBUG_DRAW_LINES(aFrom, aTo, aColor, aCount) Tga2D::CDebugDrawer::DrawLines(aFrom, aTo, aColor, aCount);
#define DEBUG_DRAW_ARROW(aFrom, aTo, aColor, aArrowHeadSize) Tga2D::CDebugDrawer::DrawArrow(aFrom, aTo, aColor, aArrowHeadSize);
#define DEBUG_DRAW_CIRCLE(aFrom, aRadius, aColor) Tga2D::CDebugDrawer::DrawCircle(aFrom, aRadius, aColor);
#else
#define DEBUG_DRAW_LINE(aFrom, aTo, aColor)
#define DEBUG_DRAW_LINES(aFrom, aTo, aColor, aCount)
#define DEBUG_DRAW_ARROW(aFrom, aTo, aColor, aArrowHeadSize)
#define DEBUG_DRAW_CIRCLE(aFrom, aRadius, aColor) 
#endif

#ifndef _RETAIL
#define MAXSAMPLES 100
namespace Tga2D
{
	class CText;
	class CLinePrimitive;
	class CPerformanceGraph;
	class CLineMultiPrimitive;
	class CTexture;
	class CDebugDrawer
	{
		friend class CErrorManager;
	public:
		CDebugDrawer(bool aIsEnabled);
		~CDebugDrawer(void);
		void Init();
		void Update(float aTimeDelta);
		void Render();
		void DrawLine(VECTOR2F aFrom, VECTOR2F aTo, CColor aColor = CColor(1, 1, 1, 1));
		void DrawLines(VECTOR2F* aFrom, VECTOR2F* aTo, CColor* aColor, unsigned int aCount);

		void DrawArrow(VECTOR2F aFrom, VECTOR2F aTo, CColor aColor = CColor(1, 1, 1, 1), float aArrowHeadSize = 1.0f);
		void DrawCircle(VECTOR2F aPos, float aRadius, CColor aColor = CColor(1, 1, 1, 1));

	private:
		void ShowErrorImage();
		double CalcAverageTick(int newtick);

		int myNumberOfRenderedLines = 0;
		static constexpr int myMaxLines = 10000;
		std::unique_ptr<std::array<CLinePrimitive, myMaxLines>> myLineBuffer = nullptr;
		std::unique_ptr<CLineMultiPrimitive> myLineMultiBuffer = nullptr;

		std::unique_ptr<CText> myFPS;
		std::unique_ptr<CText> myMemUsage;
		std::unique_ptr<CText> myDrawCallText;
		std::unique_ptr<CText> myCPUText;
		std::unique_ptr<CText> myErrorsText;
		CTexture* myErrorTexture;

		int tickindex;
		int ticksum;
		int ticklist[MAXSAMPLES];
		float myShowErrorTimer;

		int myIsEnabled;
		std::unique_ptr<CPerformanceGraph> myPerformanceGraph;
		unsigned short myRealFPS;
		unsigned long myRealFPSAvarage;
	};
}
#else

namespace Tga2D
{
	class CDebugDrawer
	{
	public:
		CDebugDrawer(int /*aFlagset*/) {}
		~CDebugDrawer() {}
		void Init() {}
		void Update(float /*aTimeDelta*/) {}
		void Render() {}
		void DrawLine(VECTOR2F /*aFrom*/, VECTOR2F /*aTo*/, Vector4f /*aColor*/) {}
		void DrawLines(VECTOR2F* /*aFrom*/, VECTOR2F* /*aTo*/, CColor* /*aColor*/, unsigned int /*aCount*/) {}

		void DrawArrow(VECTOR2F /*aFrom*/, VECTOR2F /*aTo*/, Vector4f /*aColor*/, float /*aArrowHeadSize*/) {}
		static void DrawArrow(VECTOR2F /*aFrom*/, VECTOR2F /*aTo*/, CColor aColor = CColor(1, 1, 1, 1), float aArrowHeadSize = 1.0f) { aColor; aArrowHeadSize; }
		void DrawCircle(VECTOR2F /*aPos*/, float /*aRadius*/, CColor aColor = CColor(1, 1, 1, 1)) { aColor; }
		void ShowErrorImage() {}
	private:

	};
}


#endif