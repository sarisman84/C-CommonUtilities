#pragma once

#include <tga2d/render/RenderObject.h>


namespace Tga2D
{
	class Engine;
	class GraphicsEngine;
	class LinePrimitive : public RenderObjectLine
	{
	public:
		LinePrimitive(void);
		~LinePrimitive(void);
		void Render();
		void SetFrom(float aX, float aY)
		{
			myFromPosition.Set(aX, aY);
		}
		void SetTo(float aX, float aY)
		{
			myToPosition.Set(aX, aY);
		}
	private:
	};

	class LineMultiPrimitive : public RenderObject
	{
	public:
		LineMultiPrimitive(void){}
		~LineMultiPrimitive(void){}
		void Render();
		void Draw(GraphicsEngine* aRenderer) override;
		Color *myColor;
		Vector2f *myFromPosition;
		Vector2f *myToPosition;
		unsigned int myCount;
	};
}