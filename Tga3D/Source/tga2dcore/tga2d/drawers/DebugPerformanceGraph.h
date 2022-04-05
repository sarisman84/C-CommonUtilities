#pragma once
#include <vector>

#define DEBUG_PERFGRAPH_SAMPLES 500
namespace Tga2D
{
	class Text;
	class LinePrimitive;
	class CustomShape;
	class DebugDrawer;
	class PerformanceGraph
	{
	public:
		PerformanceGraph(DebugDrawer* aDrawer);
		~PerformanceGraph(void);
		void Init(Tga2D::Color& aBackgroundColor, Tga2D::Color& aLineColor, const std::string& aText);
		void Render();

		void FeedValue(int aValue);
	private:
		std::unique_ptr<CustomShape> myBackground;
		std::vector<int> myBuffer;
		DebugDrawer* myDrawer;
		Tga2D::Color myLineColor;
		std::unique_ptr<Text> myText;

	};
}
