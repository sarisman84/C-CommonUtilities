#pragma once
#include <Tga2d/Math/Color.h>
#include <Tga2d/Math/Transform.h>
#include <Tga2d/Math/Vector.h>

namespace Tga2D
{
class Light
{
protected:
	Color myColor;
	float myIntensity;
	Transform myTransform;

public:

	virtual ~Light() = default;
	
	Light(const Transform& someTransform, const Color& aColor, float aIntensity);

	FORCEINLINE float GetIntensity() const { return myIntensity; }
	FORCEINLINE Color GetColor() const { return myColor; }

	FORCEINLINE Transform GetTransform() const { return myTransform; }
};

} // namespace Tga2D