#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#pragma comment(lib, "xinput.lib")

#include <map>
#include <functional>
#include <WinUser.h>
#include <bitset>

#include <Xinput.h>
#include <ostream>
#include <queue>
#include <string>
#include <optional>

#include "Vector2.h"



namespace CommonUtilities
{
	class Result;
}


#define MOUSEPOSITION GetMousePosition()
#define MOUSEDELTA GetMouseDelta()
//#define PRINT KEYEVENT({DebugPrint(aMessage, anWParam); return false;})


/*
dec hex key type def   dec hex key type   dec hex key type   dec hex key type
  0  00  13 C-@  NULL   32  20  5E space   64  40  13 @       96  60  11 `
  1  01  3C C-A  SOH    33  21  12 !       65  41  3C A       97  61  3C a
  2  02  50 C-B  STX    34  22  46 "       66  42  50 B       98  62  50 b
  3  03  4E C-C  ETX    35  23  14 #       67  43  4E C       99  63  4E c
  4  04  3E C-D  EOT    36  24  15 $       68  44  3E D      100  64  3E d
  5  05  29 C-E  ENQ    37  25  16 %       69  45  29 E      101  65  29 e
  6  06  3F C-F  ACK    38  26  18 &       70  46  3F F      102  66  3F f
  7  07  40 C-G  BEL    39  27  46 '       71  47  40 G      103  67  40 g
  8  08  41 C-H  BS     40  28  1A (       72  48  41 H      104  68  41 h
  9  09  2E C-I  HT     41  29  1B )       73  49  2E I      105  69  2E i
 10  0A  42 C-J  LF     42  2A  19 *       74  4A  42 J      106  6A  42 j
 11  0B  43 C-K  VT     43  2B  1D +       75  4B  43 K      107  6B  43 k
 12  0C  44 C-L  FF     44  2C  53 ,       76  4C  44 L      108  6C  44 l
 13  0D  52 C-M  CR     45  2D  1C -       77  4D  52 M      109  6D  52 m
 14  0E  51 C-N  SO     46  2E  54 .       78  4E  51 N      110  6E  51 n
 15  0F  2F C-O  SI     47  2F  55 /       79  4F  2F O      111  6F  2F o
 16  10  30 C-P  DLE    48  30  1B 0       80  50  30 P      112  70  30 p
 17  11  27 C-Q  DC1    49  31  12 1       81  51  27 Q      113  71  27 q
 18  12  2A C-R  DC2    50  32  13 2       82  52  2A R      114  72  2A r
 19  13  3D C-S  DC3    51  33  14 3       83  53  3D S      115  73  3D s
 20  14  2B C-T  DC4    52  34  15 4       84  54  2B T      116  74  2B t
 21  15  2D C-U  NAK    53  35  16 5       85  55  2D U      117  75  2D u
 22  16  4F C-V  SYN    54  36  17 6       86  56  4F V      118  76  4F v
 23  17  2E C-W  ETB    55  37  17 7       87  57  28 W      119  77  28 w
 24  18  4D C-X  CAN    56  38  19 8       88  58  4D X      120  78  4D x
 25  19  2C C-Y  EM     57  39  1A 9       89  59  2C Y      121  79  2C y
 26  1A  4C C-Z  SUB    58  3A  45 :       90  5A  4C Z      122  7A  4C z
 27  1B  31 C-[  ESC    59  3B  45 ;       91  5B  31 [      123  7B  31 {
 28  1C  33 C-\  FS     60  3C  53 <       92  5C  33 \      124  7C  33 |
 29  1D  32 C-]  GS     61  3D  3D =       93  5D  32 ]      125  7D  32 }
 30  1E  17 C-^  RS     62  3E  54 >       94  5E  17 ^      126  7E  11 ~
 31  1F  1C C-_  US     63  3F  55 ?       95  5F  1C _      127  7F  34 delete
	*/



namespace CommonUtilities
{







	enum class Stick
	{
		Left, Right
	};





	enum class Axis
	{
		Horizontal, Vertical
	};






	class Keyboard
	{
	public:
		enum class Key : unsigned int
		{
			Num0 = 0x30,
			Num1 = 0x31,
			Num2 = 0x32,
			Num3 = 0x33,
			Num4 = 0x34,
			Num5 = 0x35,
			Num6 = 0x36,
			Num7 = 0x37,
			Num8 = 0x38,
			Num9 = 0x39,

			LeftArrow = VK_LEFT,
			RightArrow = VK_RIGHT,
			UpArrow = VK_UP,
			DownArrow = VK_DOWN,
			LeftShift = VK_SHIFT,
			RightShift = VK_SHIFT,
			Escape = VK_ESCAPE,
			Alt = VK_MENU,
			Space = VK_SPACE,
			Control = VK_CONTROL,
			Enter = VK_RETURN,

			A = 0x41,
			B = 0x42,
			C = 0x43,
			D = 0x44,
			E = 0x45,
			F = 0x46,
			G = 0x47,
			H = 0x48,
			I = 0x49,
			J = 0x4A,
			K = 0x4B,
			L = 0x4C,
			M = 0x4D,
			N = 0x4E,
			O = 0x4F,
			P = 0x50,
			Q = 0x51,
			R = 0x52,
			S = 0x53,
			T = 0x54,
			U = 0x55,
			V = 0x56,
			W = 0x57,
			X = 0x58,
			Y = 0x59,
			Z = 0x5A,

		};

		static void Update(UINT aMessage, WPARAM anWParam, LPARAM anLParam);


		static const bool GetButtonDown(const Key aKey);
		static const bool GetButton(const Key aKey);
		static const bool GetButtonUp(const Key aKey);

	private:
		static std::bitset<1000> ourKeyboardState;
		static std::bitset<1000> ourPastKeyboardState;
	};


	using InputEvent = std::bitset<1000>;

	class Mouse
	{
	public:
		enum class Key : unsigned int
		{

			LeftMouseButton,
			RightMouseButton,
			MiddleMouseButton,
		};

		enum class Event : unsigned int
		{
			Press, Release
		};
		static void UpdateEvents(HWND aWindowsIns,UINT aMessage, WPARAM anWParam, LPARAM anLParam);
		static void EndFrame();

		static const bool GetButtonDown(const Key aKey);
		static const bool GetButton(const Key aKey);
		static const bool GetButtonUp(const Key aKey);

		static const Vector2<int> GetMousePosition();
		static const Vector2<float> GetMouseDelta();

	private:
		static const bool IsAMouseButtonEvent(const UINT aMessage);
		static const Key GetKeyFromMessage(const UINT aMessage);
		static const UINT GetMessageFromKey(const Key aKey, const Event anEvent);

		static InputEvent ourMouseState;
		static InputEvent ourPreviousMouseState;

		static Vector2<float> ourMouseDelta;
		static Vector2<int> ourMousePosition;
		static Vector2<int> ourPastMousePosition;
	};



	class GClient
	{
	public:

		enum class Button : unsigned int
		{

			DPAD_UP = XINPUT_GAMEPAD_DPAD_UP,
			DPAD_DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
			DPAD_LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
			DPAD_RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,
			START = XINPUT_GAMEPAD_START,
			BACK = XINPUT_GAMEPAD_BACK,
			LEFT_THUMB = XINPUT_GAMEPAD_LEFT_THUMB,
			RIGHT_THUMB = XINPUT_GAMEPAD_RIGHT_THUMB,
			LEFT_SHOULDER = XINPUT_GAMEPAD_LEFT_SHOULDER,
			RIGHT_SHOULDER = XINPUT_GAMEPAD_RIGHT_SHOULDER,
			A = XINPUT_GAMEPAD_A,
			B = XINPUT_GAMEPAD_B,
			X = XINPUT_GAMEPAD_X,
			Y = XINPUT_GAMEPAD_Y,
		};

		class ButtonEvent
		{
		public:
			enum class Type
			{
				PRESS,
				RELEASE,
			};
		private:
			Type myType;
			Button myButton;
		public:
			ButtonEvent(Type aType, Button aButton) noexcept
				:
				myType(aType),
				myButton(aButton)
			{}
			bool IsPress() const noexcept
			{
				return myType == Type::PRESS;
			}
			bool IsRelease() const noexcept
			{
				return myType == Type::RELEASE;
			}
			Button getButton() const noexcept
			{
				return myButton;
			}
		};



		GClient() = default;
		GClient(const unsigned int id, Vector2<float> deadzone = { XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE,XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE });


		const UINT GetControllerID() const noexcept;
		XINPUT_GAMEPAD* const GetGamepad();

		const bool IsConnected();
		const bool UpdateClient();

		void Vibrate(unsigned short aLeftSpeed, unsigned short aRightSpeed);
		void Vibrate(unsigned short aSpeed);
		void ResetVibration();

		const bool GetButton(const Button aButton);
		const bool GetButtonDown(const Button aButton);
		const bool GetButtonUp(const Button aButton);

		std::optional<ButtonEvent> ReadButtonBuffer() noexcept;


		Vector2<float>& GetLeftStick() noexcept;
		Vector2<float>& GetRightStick() noexcept;


		float LeftTrigger() const noexcept;
		float RightTrigger() const noexcept;

		bool GetAudioDeviceIDs(const std::wstring& aPRenderDeviceId, unsigned int* aPRenderCount,
			const std::wstring& aPCaptureDeviceId, unsigned int* aPCaptureCount) const;

		XINPUT_CAPABILITIES* const GetCapabilities(const unsigned long someflags = 0u);
		void SetDeadzone(const Vector2<float> aDeadzone);
		Vector2<float>& GetDeadzone() noexcept;

		bool ButtonIsEmpty() const noexcept;
		void Flush() noexcept;



	private:
		void OnButtonPressed(Button aButton) noexcept;
		void OnButtonReleased(Button aButton) noexcept;

		static float ApplyDeadzone(float aValue, const float aMaxValue, const float aDeadzone);

		template<typename buf>
		static void TrimBuffer(std::queue<buf>& buffer) noexcept;


		static constexpr unsigned int ourBufferSize = 16u;
		static constexpr float ourMaxAxisValue = 1.0f;
		static constexpr float ourTriggerThreshold = XINPUT_GAMEPAD_TRIGGER_THRESHOLD / 255.0f;



		std::bitset<10000> myPastGamepadState;


		const unsigned int myControllerID;

		XINPUT_GAMEPAD* myGamepad;
		XINPUT_STATE myState;
		XINPUT_VIBRATION myVibration;
		//XINPUT_BATTERY_INFORMATION myBatteryInfo;
		XINPUT_CAPABILITIES myCapabilities;

		Vector2<float> myLeftStick;
		Vector2<float> myRightStick;

		Vector2<float> myDeadzone;


		float myLeftTrigger;
		float myRightTrigger;


		std::queue<ButtonEvent> myButtonbuffer;
	};

	class Gamepad
	{
	public:
		static void Update();
		static GClient& AddGamepad();
		static GClient& GetGamepad(const unsigned int anIndex);

	private:
		static std::vector<GClient> myCurrentGamepads;

	};



	template<typename buf>
	inline void GClient::TrimBuffer(std::queue<buf>& aBuffer) noexcept
	{

		while (aBuffer.size() > ourBufferSize)
		{
			aBuffer.pop(); // [[yesdiscard]] xD
		}
	}

}


