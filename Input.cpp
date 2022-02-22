#include "pch.h"
#include "Input.h"











bool CommonUtilities::Input::UpdateEvents(UINT aMessage, WPARAM anWParam, LPARAM anLParam)
{
	if (aMessage == WM_MOUSEMOVE)
	{
		myMousePosition = Point(GET_X_LPARAM(anLParam), GET_Y_LPARAM(anLParam));
	}
	else  if (aMessage == WM_MOUSEWHEEL)
	{
		myMouseScroll = Point(0, GET_Y_LPARAM(anLParam));
	}
	if (anWParam < myKeyboardState.size())
	{
		bool isLPressed = aMessage == WM_LBUTTONDOWN ? true : aMessage == WM_LBUTTONUP ? false : false;
		bool isRPressed = aMessage == WM_RBUTTONDOWN ? true : aMessage == WM_RBUTTONUP ? false : false;
		bool isMPressed = aMessage == WM_MBUTTONDOWN ? true : aMessage == WM_MBUTTONUP ? false : false;

		myKeyboardState[anWParam] =
			anWParam == VK_LBUTTON ? isLPressed :
			anWParam == VK_RBUTTON ? isRPressed :
			anWParam == VK_MBUTTON ? isMPressed :
			aMessage == WM_KEYDOWN || aMessage == WM_SYSKEYDOWN || aMessage == WM_CHAR;


		return myKeyboardState[anWParam];
	}


	


	return false;
	
}



bool CommonUtilities::Input::GetButtonDown(KeyCode aKey)
{
	bool result = myKeyboardState[static_cast<int>(aKey)] && !myPastKeyboardState[static_cast<int>(aKey)];
	myPastKeyboardState[static_cast<int>(aKey)] = myKeyboardState[static_cast<int>(aKey)];
	return result;
}

bool CommonUtilities::Input::GetButton(KeyCode aKey)
{
	return myKeyboardState[static_cast<int>(aKey)];
}

bool CommonUtilities::Input::GetButtonUp(KeyCode aKey)
{
	bool result = !myKeyboardState[static_cast<int>(aKey)] && myPastKeyboardState[static_cast<int>(aKey)];

	myPastKeyboardState[static_cast<int>(aKey)] = myKeyboardState[static_cast<int>(aKey)];
	return result;
}

CommonUtilities::Point CommonUtilities::Input::GetMousePosition()
{
	return myMousePosition;
}

CommonUtilities::Point CommonUtilities::Input::GetMouseScrollDir()
{
	return myMouseScroll;
}

CommonUtilities::Point CommonUtilities::Input::GetMouseDelta()
{
	Point result = myMousePosition - myPastMousePosition;
	myPastMousePosition = myMousePosition;
	return result;
}

float CommonUtilities::Input::GetAxisRaw(Axis anAxis)
{
	switch (anAxis)
	{
		case CommonUtilities::Axis::Horizontal:
			return GetButton(KeyCode::A) || GetButton(KeyCode::LeftArrow) ? -1 : GetButton(KeyCode::D) || GetButton(KeyCode::RightArrow) ? 1 : 0;
		case CommonUtilities::Axis::Vertical:
			return GetButton(KeyCode::W) || GetButton(KeyCode::UpArrow) ? -1 : GetButton(KeyCode::S) || GetButton(KeyCode::DownArrow) ? 1 : 0;

	}
	return 0.0f;
}









































CommonUtilities::Point::Point(const Point& aRhs)
{
	myXPos = aRhs.myXPos;
	myYPos = aRhs.myYPos;
}

void CommonUtilities::Point::operator=(const Point& aRhs)
{
	myXPos = aRhs.myXPos;
	myYPos = aRhs.myYPos;
}

CommonUtilities::Point CommonUtilities::Point::operator+(const Point& aRhs)
{
	myXPos += aRhs.myXPos;
	myYPos += aRhs.myYPos;
	return *this;
}

CommonUtilities::Point CommonUtilities::Point::operator-(const Point& aRhs)
{
	myXPos -= aRhs.myXPos;
	myYPos -= aRhs.myYPos;
	return *this;
}

CommonUtilities::Point::Point(const int anX, const int anY)
{
	myXPos = anX;
	myYPos = anY;
}

const int CommonUtilities::Point::GetXPos() const
{
	return myXPos;
}

const int CommonUtilities::Point::GetYPos() const
{
	return myYPos;
}

