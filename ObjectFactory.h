#pragma once
#include <map>
#include <vector>
#include <iostream>

#define ELEMENTREF(type) std::shared_ptr<type>

namespace CommonUtilities
{
	template<typename T, typename E, typename R>
	class ObjectFactory
	{
	public:
		ObjectFactory();
		ObjectFactory(std::map<E, T> anElementTypeReg);
		ObjectFactory(const ObjectFactory& aRhs) = delete;
	
		ELEMENTREF(R) CreateElement(const E anElementType);
	private:
		std::map<E, T> myRegisteredElements;
	};
}


