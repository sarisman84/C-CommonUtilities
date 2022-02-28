#include "pch.h"
#include "Input.h"
#include <optional>
#include "windowsx.h"



static float normalize(const float aValue, const float aMin, const float aMax);


static float normalize(const float aValue, const float aMin, const float aMax)
{
	const float average = (aMin + aMax) / 2.0f;
	const float range = (aMax - aMin) / 2.0f;
	return (aValue - average) / range;
}



#pragma region Gamepad Impl

std::vector<CommonUtilities::GClient> CommonUtilities::Gamepad::myCurrentGamepads;



CommonUtilities::GClient::GClient(const unsigned int anID, Vector2<float> aDeadzone) : myControllerID(anID), myDeadzone(aDeadzone)
{
	ZeroMemory(&myState, sizeof(XINPUT_STATE));
	ZeroMemory(&myVibration, sizeof(XINPUT_VIBRATION));
	ZeroMemory(&myBatteryInfo, sizeof(XINPUT_BATTERY_INFORMATION));
	ZeroMemory(&myCapabilities, sizeof(XINPUT_CAPABILITIES));
}

const UINT CommonUtilities::GClient::GetControllerID() const noexcept
{
	return myControllerID - 1;
}

XINPUT_GAMEPAD* const CommonUtilities::GClient::GetGamepad()
{
	return &this->myState.Gamepad;
}

const bool CommonUtilities::GClient::IsConnected()
{
	return (XInputGetState(this->myControllerID - 1, &myState) == ERROR_SUCCESS);
}

const bool CommonUtilities::GClient::UpdateClient()
{
	if (!IsConnected())
		return false;

	if (myState.Gamepad.wButtons != 0u) {

		OnButtonPressed(static_cast<Button>(myState.Gamepad.wButtons));
	}
	else {

		if (!ButtonIsEmpty()) {
			OnButtonReleased(myButtonbuffer.front().getButton());
		}
	}


	const float normLX = normalize(static_cast<float>(myState.Gamepad.sThumbLX), SHRT_MIN, SHRT_MAX);
	const float normLY = normalize(static_cast<float>(myState.Gamepad.sThumbLY), SHRT_MIN, SHRT_MAX);

	const float normRX = normalize(static_cast<float>(myState.Gamepad.sThumbRX), SHRT_MIN, SHRT_MAX);
	const float normRY = normalize(static_cast<float>(myState.Gamepad.sThumbRY), SHRT_MIN, SHRT_MAX);



	myLeftStick.x = ApplyDeadzone(normLX, ourMaxAxisValue, normalize(myDeadzone.x, SHRT_MIN, SHRT_MAX));
	myLeftStick.y = ApplyDeadzone(normLY, ourMaxAxisValue, normalize(myDeadzone.y, SHRT_MIN, SHRT_MAX));

	myRightStick.x = ApplyDeadzone(normRX, ourMaxAxisValue, normalize(myDeadzone.x, SHRT_MIN, SHRT_MAX));
	myRightStick.y = ApplyDeadzone(normRY, ourMaxAxisValue, normalize(myDeadzone.y, SHRT_MIN, SHRT_MAX));

	return true;
}

void CommonUtilities::GClient::Vibrate(unsigned short aLeftSpeed, unsigned short aRightSpeed)
{
	myVibration.wLeftMotorSpeed = aLeftSpeed;
	myVibration.wRightMotorSpeed = aRightSpeed;
	XInputSetState(GetControllerID(), &myVibration);
}

void CommonUtilities::GClient::Vibrate(unsigned short aSpeed)
{
	myVibration.wLeftMotorSpeed = aSpeed;
	myVibration.wRightMotorSpeed = aSpeed;
	XInputSetState(GetControllerID(), &myVibration);
}

void CommonUtilities::GClient::ResetVibration()
{
	myVibration.wLeftMotorSpeed = 0u;
	myVibration.wRightMotorSpeed = 0u;
	XInputSetState(GetControllerID(), &myVibration);
}

const bool CommonUtilities::GClient::GetButton(const Button aButton)
{
	
	return (myState.Gamepad.wButtons & static_cast<unsigned short>(aButton)) != 0u;
}

const bool CommonUtilities::GClient::GetButtonDown(const Button aButton)
{
	bool result = (myState.Gamepad.wButtons & static_cast<unsigned int>(aButton)) != 0u && !myPastGamepadState[static_cast<unsigned int>(aButton)];
	myPastGamepadState[static_cast<unsigned int>(aButton)] = (myState.Gamepad.wButtons & static_cast<unsigned int>(aButton)) != 0u;
	return result;
}

const bool CommonUtilities::GClient::GetButtonUp(const Button aButton)
{
	bool result = (myState.Gamepad.wButtons & static_cast<unsigned int>(aButton)) == 0u && myPastGamepadState[static_cast<unsigned int>(aButton)];
	myPastGamepadState[static_cast<unsigned int>(aButton)] = (myState.Gamepad.wButtons & static_cast<unsigned int>(aButton)) != 0u;
	return result;
}

std::optional<CommonUtilities::GClient::ButtonEvent> CommonUtilities::GClient::ReadButtonBuffer() noexcept
{
	if (myButtonbuffer.size() > 0u)
	{
		GClient::ButtonEvent e = myButtonbuffer.front();
		myButtonbuffer.pop();
		return e;
	}
	return {};
}

CommonUtilities::Vector2<float>& CommonUtilities::GClient::GetLeftStick() noexcept
{
	return myLeftStick;
}

CommonUtilities::Vector2<float>& CommonUtilities::GClient::GetRightStick() noexcept
{
	return myRightStick;
}

float CommonUtilities::GClient::LeftTrigger() const noexcept
{
	return myLeftTrigger;
}

float CommonUtilities::GClient::RightTrigger() const noexcept
{
	return myRightTrigger;
}

bool CommonUtilities::GClient::GetAudioDeviceIDs(const std::wstring& aPRenderDeviceId, unsigned int* aPRenderCount, const std::wstring& aPCaptureDeviceId, unsigned int* aPCaptureCount) const
{
	// I'm not quite sure how to use this one but this should wrap it okay
	const auto result = XInputGetAudioDeviceIds(GetControllerID(),
		const_cast<wchar_t*>(aPRenderDeviceId.c_str()), aPRenderCount,
		const_cast<wchar_t*>(aPCaptureDeviceId.c_str()), aPCaptureCount);
	if (result != ERROR_SUCCESS)
		return false;
	return true;
}

XINPUT_CAPABILITIES* const CommonUtilities::GClient::GetCapabilities(const unsigned long someflags)
{
	const auto result = XInputGetCapabilities(GetControllerID(), someflags,
		&this->myCapabilities);
	return &this->myCapabilities;
}

void CommonUtilities::GClient::SetDeadzone(const Vector2<float> aDeadzone)
{
	myDeadzone = aDeadzone;
}

CommonUtilities::Vector2<float>& CommonUtilities::GClient::GetDeadzone() noexcept
{
	return myDeadzone;
}

bool CommonUtilities::GClient::ButtonIsEmpty() const noexcept
{
	return myButtonbuffer.empty();
}

void CommonUtilities::GClient::Flush() noexcept
{
	myButtonbuffer = std::queue<GClient::ButtonEvent>();
}

void CommonUtilities::GClient::OnButtonPressed(Button aButton) noexcept
{
	myButtonbuffer.push(GClient::ButtonEvent(GClient::ButtonEvent::Type::PRESS, aButton));
	TrimBuffer(myButtonbuffer);
}

void CommonUtilities::GClient::OnButtonReleased(Button aButton) noexcept
{
	myButtonbuffer.push(GClient::ButtonEvent(GClient::ButtonEvent::Type::RELEASE, aButton));
	TrimBuffer(myButtonbuffer);
}

float CommonUtilities::GClient::ApplyDeadzone(float aValue, const float aMaxValue, const float aDeadzone)
{
	if (aValue < -(aDeadzone))
	{
		aValue += aDeadzone; // increase neg vals to remove deadzone discontinuity
	}
	else if (aValue > aDeadzone)
	{
		aValue -= aDeadzone; // decrease pos vals to remove deadzone discontinuity
	}
	else
	{
		return 0.0f; // hey values are zero for once
	}
	const float normValue = aValue / (aMaxValue - aDeadzone);
	return std::max(-1.0f, std::min(normValue, 1.0f));
}


void CommonUtilities::Gamepad::Update()
{
	for (unsigned int i = 0; i < myCurrentGamepads.size(); i++)
	{
		myCurrentGamepads[i].UpdateClient();
	}
}

CommonUtilities::GClient& CommonUtilities::Gamepad::AddGamepad()
{
	myCurrentGamepads.push_back(GClient(myCurrentGamepads.size() + 1));
	return myCurrentGamepads[myCurrentGamepads.size() - 1];
}

CommonUtilities::GClient& CommonUtilities::Gamepad::GetGamepad(const unsigned int anIndex)
{

	return myCurrentGamepads[anIndex - 1];
}

#pragma endregion

enum class MouseEvent
{
	Down, Up
};

const UINT GetButtonEvent(const MouseEvent aMouseEvent, WPARAM anWParam) {

	switch (anWParam)
	{
	case VK_LBUTTON:
		switch (aMouseEvent)
		{
		case MouseEvent::Down:
			return WM_LBUTTONDOWN;
		case MouseEvent::Up:
			return WM_LBUTTONUP;
		}

	case VK_RBUTTON:
		switch (aMouseEvent)
		{
		case MouseEvent::Down:
			return WM_RBUTTONDOWN;
		case MouseEvent::Up:
			return WM_RBUTTONUP;
		}

	case VK_MBUTTON:
		switch (aMouseEvent)
		{
		case MouseEvent::Down:
			return WM_MBUTTONDOWN;
		case MouseEvent::Up:
			return WM_MBUTTONUP;
		}


	}

	return UINT();
}


std::bitset<1000> CommonUtilities::Mouse::ourMouseEvents;
std::bitset<1000> CommonUtilities::Mouse::ourPastMouseEvents;

CommonUtilities::Vector2<float> CommonUtilities::Mouse::ourMouseDelta;
CommonUtilities::Vector2<int> CommonUtilities::Mouse::ourMousePosition;

void CommonUtilities::Mouse::Update(UINT aMessage, WPARAM anWParam, LPARAM anLParam)
{
	ourMouseDelta = { 0,0 };

	if (anWParam <= 1000) {
		ourMouseEvents[anWParam] =
			aMessage == GetButtonEvent(MouseEvent::Down, anWParam) ?
			true :
			aMessage == GetButtonEvent(MouseEvent::Up, anWParam) ?
			false :
			ourMouseEvents[anWParam];
	}



	

	if (aMessage == WM_MOUSEMOVE) {
		Vector2< int> oldMousePos = ourMousePosition;
		ourMousePosition = { GET_X_LPARAM(anLParam),GET_Y_LPARAM(anLParam) };

		Vector2<float> newPos = { (float)ourMousePosition.x, (float)ourMousePosition.y };
		Vector2<float> curPos = { (float)oldMousePos.x, (float)oldMousePos.y };

		ourMouseDelta = (newPos - curPos).GetNormalized();
	}


}

const bool CommonUtilities::Mouse::GetButtonDown(const MouseKey aKey)
{
	bool result = ourMouseEvents[static_cast<int>(aKey)] && !ourPastMouseEvents[static_cast<int>(aKey)];
	ourPastMouseEvents[static_cast<int>(aKey)] = ourMouseEvents[static_cast<int>(aKey)];
	return result;
}

const bool CommonUtilities::Mouse::GetButton(const MouseKey aKey)
{
	return ourMouseEvents[static_cast<int>(aKey)];
}

const bool CommonUtilities::Mouse::GetButtonUp(const MouseKey aKey)
{
	bool result = !ourMouseEvents[static_cast<int>(aKey)] && ourPastMouseEvents[static_cast<int>(aKey)];
	ourPastMouseEvents[static_cast<int>(aKey)] = ourMouseEvents[static_cast<int>(aKey)];
	return result;
}

const CommonUtilities::Vector2< int> CommonUtilities::Mouse::GetMousePosition()
{
	return ourMousePosition;
}

const CommonUtilities::Vector2<float> CommonUtilities::Mouse::GetMouseDelta()
{
	return ourMouseDelta;
}




std::bitset<1000> CommonUtilities::Keyboard::ourKeyboardState;
std::bitset<1000> CommonUtilities::Keyboard::ourPastKeyboardState;


void CommonUtilities::Keyboard::Update(UINT aMessage, WPARAM anWParam, LPARAM anLParam)
{
	if (anWParam > 1000) return;

	ourKeyboardState[anWParam] =
		aMessage == WM_SYSKEYDOWN ||
		aMessage == WM_SYSCHAR ||
		aMessage == WM_KEYDOWN ||
		aMessage == WM_CHAR;



}

const bool CommonUtilities::Keyboard::GetButtonDown(const KeyboardKey aKey)
{
	bool result = ourKeyboardState[static_cast<int>(aKey)] && !ourPastKeyboardState[static_cast<int>(aKey)];
	ourPastKeyboardState[static_cast<int>(aKey)] = ourKeyboardState[static_cast<int>(aKey)];
	return result;
}

const bool CommonUtilities::Keyboard::GetButton(const KeyboardKey aKey)
{
	return ourKeyboardState[static_cast<int>(aKey)];
}

const bool CommonUtilities::Keyboard::GetButtonUp(const KeyboardKey aKey)
{
	bool result = !ourKeyboardState[static_cast<int>(aKey)] && ourPastKeyboardState[static_cast<int>(aKey)];
	ourPastKeyboardState[static_cast<int>(aKey)] = ourKeyboardState[static_cast<int>(aKey)];
	return result;
}
