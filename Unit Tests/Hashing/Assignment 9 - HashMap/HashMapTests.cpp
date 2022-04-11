#include "stdafx.h"
#include "CppUnitTest.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <C-CommonUtilities/HashMap.h>
#include "Common\UtilityFunctions.hpp"
#include <string>
#include <vector>
#include <assert.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Assignment9HashMap
{		
	TEST_CLASS(HashMapTests)
	{
	public:
		TEST_METHOD(Constructor_GetOnEmptyMap)
		{
			CommonUtilities::HashMap<std::string, int> strMap(20);
			CommonUtilities::HashMap<int, std::string> intMap(20);

			Assert::IsNull(strMap.Get(""), L"Get returned a value on an empty HashMap<string, int>.");
			Assert::IsNull(intMap.Get(0), L"Get returned a value on an empty HashMap<int, string>.");

			//CommonUtilities::HashMap<std::string, int> strEmptyMap(0);
			//CommonUtilities::HashMap<int, std::string> intEmptyMap(0);

			////Assert::IsNull(strEmptyMap.Get(""), L"Get returned a value on an empty HashMap<string, int>.");
			//Assert::IsNull(intEmptyMap.Get(0), L"Get returned a value on an empty HashMap<int, string>.");
		}

		TEST_METHOD(Insert_Get)
		{
			CommonUtilities::HashMap<std::string, int> strMap(10);

			Assert::IsNull(strMap.Get("tga"), L"Get returned a value for a key in an empty HashMap<str, int>.");
			Assert::IsNull(strMap.Get("spel"), L"Get returned a value for a key in an empty HashMap<str, int>.");
			Assert::IsNull(strMap.Get("programmering"), L"Get returned a value for a key in an empty HashMap<str, int>.");

			Assert::IsTrue(strMap.Insert("tga", 30), L"Element <\"tga\", 30> could not be inserted into the HashMap<string, int>.");
			Assert::IsTrue(strMap.Insert("spel", 10), L"Element <\"spel\", 10> could not be inserted into the HashMap<string, int>.");
			Assert::IsTrue(strMap.Insert("programmering", 20), L"Element <\"programmering\", 20> could not be inserted into the HashMap<string, int>.");

			Assert::IsNotNull(strMap.Get("tga"), L"Get returned nullptr for a key that should exist.");
			Assert::AreEqual(30, *strMap.Get("tga"), L"Get returned the wrong value for a key.");

			Assert::IsNotNull(strMap.Get("spel"), L"Get returned nullptr for a key that should exist.");
			Assert::AreEqual(10, *strMap.Get("spel"), L"Get returned the wrong value for a key.");

			Assert::IsNotNull(strMap.Get("programmering"), L"Get returned nullptr for a key that should exist.");
			Assert::AreEqual(20, *strMap.Get("programmering"), L"Get returned the wrong value for a key.");

			std::string key = "";
			for (int iterator = 0; iterator < 7; ++iterator)
			{
				key += "a";
				Assert::IsTrue(strMap.Insert(key, iterator), L"Element could not be inserted into the HashMap<string, int> although there is space left.");
			}

			Assert::IsFalse(strMap.Insert("b", 3), L"Element insertion succeeded even though the HashMap<string, int> is full.");
		}

		TEST_METHOD(InsertToMapSize0)
		{
			CommonUtilities::HashMap<std::string, int> strEmptyMap(0);
			CommonUtilities::HashMap<int, std::string> intEmptyMap(0);

			Assert::IsFalse(strEmptyMap.Insert("a", 3), L"Element insertion into a HashMap<string, int> with 0 capacity succeeded.");
			Assert::IsFalse(intEmptyMap.Insert(3, "a"), L"Element insertion into a HashMap<int, string> with 0 capacity succeeded.");
		}

		TEST_METHOD(InsertSameKey)
		{
			CommonUtilities::HashMap<std::string, int> strMap(10);

			Assert::IsNull(strMap.Get("tga"), L"Get returned a value for a key in an empty HashMap<str, int>.");
			Assert::IsTrue(strMap.Insert("tga", 30), L"Element <\"tga\", 30> could not be inserted into the HashMap<string, int>.");
			Assert::AreEqual(30, *strMap.Get("tga"), L"Get returned the wrong value for a key.");

			for (int iterator = 0; iterator < 100; ++iterator)
			{
				int value = TestUtility::GetRandomInt(-100, 100);
				Assert::IsTrue(strMap.Insert("tga", value), L"Element with key <\"tga\"> could not be replaced in the HashMap<string, int>.");
				Assert::AreEqual(value, *strMap.Get("tga"), L"Get returned the wrong value for a key.");
			}
		}

		TEST_METHOD(Remove)
		{
			CommonUtilities::HashMap<std::string, int> strMap(10);

			Assert::IsNull(strMap.Get("tga"), L"Get returned a value for a key in an empty HashMap<string, int>.");
			Assert::IsTrue(strMap.Insert("tga", 30), L"Element <\"tga\", 30> could not be inserted into the HashMap<string, int>.");
			Assert::IsNotNull(strMap.Get("tga"), L"Get returned nullptr for a key that should exist.");
			Assert::AreEqual(30, *strMap.Get("tga"), L"Get returned the wrong value for a key.");
			Assert::IsTrue(strMap.Remove("tga"), L"Remove returned false for a key that exists.");
			Assert::IsNull(strMap.Get("tga"), L"Get returned a value for a key that was removed.");
			Assert::IsFalse(strMap.Remove("tga"), L"Remove returned true for a key that was just removed.");

			CommonUtilities::HashMap<int, std::string> intMap(10);

			Assert::IsNull(intMap.Get(30), L"Get returned a value for a key in an empty HashMap<int, string>.");
			Assert::IsTrue(intMap.Insert(30, "tga"), L"Element <30, \"tga\"> could not be inserted into the HashMap<string, int>.");
			Assert::IsNotNull(intMap.Get(30), L"Get returned nullptr for a key that should exist.");
			Assert::AreEqual(std::string("tga"), *intMap.Get(30), L"Get returned the wrong value for a key.");
			Assert::IsTrue(intMap.Remove(30), L"Remove returned false for a key that exists.");
			Assert::IsNull(intMap.Get(30), L"Get returned a value for a key that was removed.");
			Assert::IsFalse(intMap.Remove(30), L"Remove returned true for a key that was just removed.");

			CommonUtilities::HashMap<int, int> intIntMap(100);
			std::vector<int> keys;

			for (int iterator = 0; iterator < 100; ++iterator)
			{
				int key = TestUtility::GetRandomInt(0, 500);
				int value = TestUtility::GetRandomInt(0, 1000);

				while (intIntMap.Get(key) != nullptr)
				{
					key = TestUtility::GetRandomInt(0, 500);
				}

				Assert::IsTrue(intIntMap.Insert(key, value), L"Key could not be inserted into a HashMap<int, int> even though it has space.");
				keys.push_back(key);
			}

			for (int index = 0; index < static_cast<int>(keys.size()); ++index)
			{
				assert(intIntMap.Remove(keys[index]) && L"A key that was inserted int a HashMap<int, int> could not be removed.");
			}

		//	CommonUtilities::HashMap<std::string, std::string> emptyMap(0);
			//Assert::IsFalse(emptyMap.Remove("tga"), L"Removing a key from an empty HashMap<string, string> succeeded.");
		}
		TEST_METHOD(MemoryLeaks)
		{
			struct CrtCheckMemory
			{
				_CrtMemState state1;
				_CrtMemState state2;
				_CrtMemState state3;
				CrtCheckMemory()
				{
					_CrtMemCheckpoint(&state1);
				}
				~CrtCheckMemory()
				{
					_CrtMemCheckpoint(&state2);

					if (_CrtMemDifference(&state3, &state1, &state2)) {
						_CrtMemDumpStatistics(&state3);
						Assert::Fail(L"Memory leaks detected.");
					}
					_CrtMemDumpStatistics(&state3);
				}
			};
			CrtCheckMemory crt;
			CommonUtilities::HashMap<int, int> intIntMap(100);
			std::vector<int> keys;

			for (int iterator = 0; iterator < 100; ++iterator)
			{
				int key = TestUtility::GetRandomInt(0, 500);
				int value = TestUtility::GetRandomInt(0, 1000);

				while (intIntMap.Get(key) != nullptr)
				{
					key = TestUtility::GetRandomInt(0, 500);
				}
				keys.push_back(key);
				intIntMap.Insert(key, value);
			}

			for (int index = 0; index < static_cast<int>(keys.size()); ++index)
			{
				intIntMap.Remove(keys[index]);
			}

			//	CommonUtilities::HashMap<std::string, std::string> emptyMap(0);
				//Assert::IsFalse(emptyMap.Remove("tga"), L"Removing a key from an empty HashMap<string, string> succeeded.");
		}
#define TEST_LESS_THAN
#ifdef TEST_LESS_THAN
		TEST_METHOD(OnlyUseLessThan)
		{
			struct TestStruct
			{
				int myInt = 0;

				TestStruct() = default;
				TestStruct(const int aInt) :myInt(aInt) {}
				TestStruct(const TestStruct& aOther) :myInt(aOther.myInt) {}
				TestStruct& operator=(const TestStruct& arhs)
				{
					myInt = arhs.myInt;
					return *this;
				}
				bool operator<(const TestStruct& arhs) const
				{
					return myInt < arhs.myInt;
				}
			};
			CommonUtilities::HashMap<TestStruct, TestStruct> intIntMap(100);
			std::vector<TestStruct> keys;

			for (int iterator = 0; iterator < 100; ++iterator)
			{
				int key = TestUtility::GetRandomInt(0, 500);
				int value = TestUtility::GetRandomInt(0, 1000);

				while (intIntMap.Get(key) != nullptr)
				{
					key = TestUtility::GetRandomInt(0, 500);
				}
				keys.push_back(key);
				intIntMap.Insert(key, value);
			}

			for (int index = 0; index < static_cast<int>(keys.size()); ++index)
			{
				intIntMap.Remove(keys[index]);
			}
		}
#else
		TEST_METHOD(SkippedLessThan)
		{
			Assert::Fail(L"Skipped testing less than use.");
		}
#endif // TEST_LESS_THAN
	};
}