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




	myGamepadLeftStick = Point(myGamepadInput.leftStickX, myGamepadInput.leftStickY);
	myGamepadRightStick = Point(myGamepadInput.rightStickX, myGamepadInput.rightStickY);
	myGamepadInput.Refresh();

	return false;

}



bool CommonUtilities::Input::GetButtonDown(const KeyCode aKey)
{
	


	bool result = myKeyboardState[static_cast<int>(aKey)] && !myPastKeyboardState[static_cast<int>(aKey)];
	myPastKeyboardState[static_cast<int>(aKey)] = myKeyboardState[static_cast<int>(aKey)];
	return result;
}

bool CommonUtilities::Input::GetButton(const KeyCode aKey)
{
	

	return myKeyboardState[static_cast<int>(aKey)];
}

bool CommonUtilities::Input::GetButtonUp(const KeyCode aKey)
{
	

	bool result = !myKeyboardState[static_cast<int>(aKey)] && myPastKeyboardState[static_cast<int>(aKey)];

	myPastKeyboardState[static_cast<int>(aKey)] = myKeyboardState[static_cast<int>(aKey)];
	return result;
}

CommonUtilities::Point CommonUtilities::Input::GetGamepadStick(const Stick aStick)
{
	switch (aStick)
	{
	default:
		return Point();
	case Stick::Left:
		return myGamepadLeftStick;
	case Stick::Right:
		return myGamepadRightStick;
	}
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

const bool CommonUtilities::Input::IsXInput(const KeyCode akey)
{
	return
		akey == KeyCode::Gamepad_A ||
		akey == KeyCode::Gamepad_B ||
		akey == KeyCode::Gamepad_X ||
		akey == KeyCode::Gamepad_Y ||
		akey == KeyCode::Gamepad_Dpad_Down ||
		akey == KeyCode::Gamepad_Dpad_Up ||
		akey == KeyCode::Gamepad_Dpad_Left ||
		akey == KeyCode::Gamepad_Dpad_Right ||
		akey == KeyCode::Gamepad_LShoulder ||
		akey == KeyCode::Gamepad_RShoulder ||
		akey == KeyCode::Gamepad_LThumb ||
		akey == KeyCode::Gamepad_RThumb;
}

const WORD CommonUtilities::Input::GetInput(const KeyCode aKey)
{
	switch (aKey)
	{
	default:
		return WORD();
	case KeyCode::Gamepad_A:
		return XINPUT_GAMEPAD_A;
	case KeyCode::Gamepad_B:
		return XINPUT_GAMEPAD_B;
	case KeyCode::Gamepad_X:
		return XINPUT_GAMEPAD_X;
	case KeyCode::Gamepad_Y:
		return XINPUT_GAMEPAD_Y;
	case KeyCode::Gamepad_LShoulder:
		return XINPUT_GAMEPAD_LEFT_SHOULDER;
	case KeyCode::Gamepad_RShoulder:
		return XINPUT_GAMEPAD_RIGHT_SHOULDER;
	case KeyCode::Gamepad_LThumb:
		return XINPUT_GAMEPAD_LEFT_THUMB;
	case KeyCode::Gamepad_RThumb:
		return XINPUT_GAMEPAD_RIGHT_THUMB;
	case KeyCode::Gamepad_Dpad_Up:
		return XINPUT_GAMEPAD_DPAD_UP;
	case KeyCode::Gamepad_Dpad_Down:
		return XINPUT_GAMEPAD_DPAD_DOWN;
	case KeyCode::Gamepad_Dpad_Left:
		return XINPUT_GAMEPAD_DPAD_LEFT;
	case KeyCode::Gamepad_Dpad_Right:
		return XINPUT_GAMEPAD_DPAD_RIGHT;
	}
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

std::ostream& CommonUtilities::operator<<(std::ostream& aStream, const Point& aPoint)
{
	aStream << "(x:" << aPoint.GetXPos() << ", y:" << aPoint.GetYPos() << ")";
	return aStream;
}

const UINT CommonUtilities::Input::Gamepad::GetControllerID() const noexcept
{
	return 0;
}

XINPUT_GAMEPAD* const CommonUtilities::Input::Gamepad::GetGamepad()
{
	return nullptr;
}

bool CommonUtilities::Input::Gamepad::IsConnected()
{
	return false;
}
