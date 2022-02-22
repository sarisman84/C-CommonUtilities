#include "pch.h"
#include "ObjectFactory.h"

template<typename T, typename E, typename R>
CommonUtilities::ObjectFactory<T, E, R>::ObjectFactory()
{
}

template<typename T, typename E, typename R>
CommonUtilities::ObjectFactory<T, E, R>::ObjectFactory(std::map<E, T> anElementTypeReg)
{
	myRegisteredElements = anElementTypeReg;
}

template<typename T, typename E, typename R>
ELEMENTREF(R) CommonUtilities::ObjectFactory<T, E, R>::CreateElement(const E anElementType)
{
	return ELEMENTREF(R)((myRegisteredElements[anElementType]));
}
