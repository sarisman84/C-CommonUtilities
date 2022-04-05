#include "stdafx.h"
#include "Transform.h"

using namespace Tga2D;

Transform::Transform(Vector3f somePosition, Rotator someRotation, Vector3f someScale)
{
	myPosition = somePosition;
	myRotation = someRotation;
	myScale = someScale;
}

void Transform::SetPosition(Vector3f somePosition)
{
	myPosition = somePosition;
	IsDirty = true;
}

void Transform::SetRotation(Rotator someRotation)
{
	myRotation.Roll = fmod(someRotation.Roll, 360.0f);
	if (myRotation.Roll > 180)
		myRotation.Roll -= 360;
	else if (myRotation.Roll < -180)
		myRotation.Roll += 360;

	myRotation.Pitch = fmod(someRotation.Pitch, 360.0f);
	if (myRotation.Pitch > 180)
		myRotation.Pitch -= 360;
	else if (myRotation.Pitch < -180)
		myRotation.Pitch += 360;

	myRotation.Yaw = fmod(someRotation.Yaw, 360.0f);
	if (myRotation.Yaw > 180)
		myRotation.Yaw -= 360;
	else if (myRotation.Yaw < -180)
		myRotation.Yaw += 360;

	IsDirty = true;
}

void Transform::SetScale(Vector3f someScale)
{
	myScale = someScale;
	IsDirty = true;
}

void Transform::AddRotation(Rotator someRotation)
{
	SetRotation(myRotation + someRotation);
	IsDirty = true;
}
