#pragma once
#include <Windows.h>
#include <Windowsx.h>
#include <map>
#include <functional>
#include <WinUser.h>
#include <bitset>

#include <tga2d/input/XInput.h>



namespace CommonUtilities
{
	class Result;
}


#define MOUSEPOSITION GetMousePosition()
#define MOUSEDELTA GetMouseDelta()
//#define PRINT KEYEVENT({DebugPrint(aMessage, anWParam); return false;})

namespace CommonUtilities
{


	enum class KeyCode
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


		LeftMouseButton = VK_LBUTTON,
		RightMouseButton = VK_RBUTTON,
		MiddleMouseButton = VK_MBUTTON,

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



		Gamepad_A = 10, //0x1000 - 4096
		Gamepad_B = 11, //0x2000 - 8192
		Gamepad_X = 12, //0x4000 - 16384
		Gamepad_Y = 13, //0x8000 - 32768

		Gamepad_LShoulder = 14, //0x0100 - 256
		Gamepad_RShoulder = 15, //0x0200 - 512

		Gamepad_LThumb = 16, //0x0040 - 64
		Gamepad_RThumb = 17, //0x0080 - 128

		Gamepad_Dpad_Up = 18, //0x0001 - 1
		Gamepad_Dpad_Down = 19, //0x0002 - 2
		Gamepad_Dpad_Left = 20, //0x0004 - 4
		Gamepad_Dpad_Right = 21 //0x0008 - 8


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
	};



	class Point
	{
	public:
		Point(const int anX, const int anY);
		Point() = default;
		Point(const Point& aRhs);
		const int GetXPos() const;
		const int GetYPos() const;
		Point operator-(const Point& aRhs);
		Point operator+(const Point& aRhs);
		void operator=(const Point& aRhs);
	private:
		int myXPos;
		int myYPos;
	};





	enum class Axis
	{
		Horizontal, Vertical
	};

	const unsigned int ourKeyboardSize = 500;
	const unsigned int ourGamepadSize = 12;
	class Input
	{
	public:

		
		Input() = default;
		Input(const Input& aRhs) = default;
		bool UpdateEvents(UINT aMessage, WPARAM anWParam, LPARAM anLParam);
		bool GetButtonDown(KeyCode aKey);
		bool GetButton(KeyCode aKey);
		bool GetButtonUp(KeyCode aKey);
		Point GetMousePosition();
		Point GetMouseDelta();
		Point GetMouseScrollDir();
		float GetAxisRaw(Axis anAxis);





	private:
		Tga2D::CXInput myGamepadInput;


		const bool IsXInput(const KeyCode aKey);
		const WORD GetInput(const KeyCode aKey);

		Point myMouseScroll;
		Point myMousePosition;
		Point myPastMousePosition;
		std::bitset<ourKeyboardSize> myKeyboardState;
		std::bitset<ourKeyboardSize> myPastKeyboardState;


		std::bitset<ourGamepadSize> myPastGamepadState;

	};



	
	
}


