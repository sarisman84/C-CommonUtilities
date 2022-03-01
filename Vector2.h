#pragma once
#include <iostream>
#include "UtilityFunctions.h"
namespace CommonUtilities
{
	template <class T>
	class Vector2
	{

	public:

		T x;
		T y;

		//constructors
		Vector2() :
			x(T(0)),
			y(T(0))
		{
		}
		Vector2(const T& aX, const T& aY) :
			x(T(aX)),
			y(T(aY))
		{
		}
		Vector2(const Vector2<T>& aVector2) = default;

		//destructor
		~Vector2() = default;

		//assignment operator 
		Vector2<T>& operator=(const Vector2<T>& aVector2) = default;

		//Vector2 Magnitude SQUARED
		T LengthSqr() const
		{
			T result = (x * x + y * y);
			return result;

		}


		//Vectors Magnitude
		float Length() const
		{
			float result = sqrt((float)(x * x + y * y));
			return result;
		}


		//normalize the current vector
		void Normalize()
		{
			if (x == 0 && y == 0)
				return;

			T length = Length();
			x /= length;
			y /= length;
		}

		//get a normalized version of the vector
		Vector2<T> GetNormalized() const
		{
			if (x == 0 && y == 0)
				return Vector2<T>();

			Vector2<T> tempVector(x, y);
			T length = Length();
			return (tempVector / length);
		}

		//multiplications
		T Dot(const Vector2<T>& aRHSVector2) const
		{
			T result = x * aRHSVector2.x + y * aRHSVector2.y;

			return result;
		}

		//set
		void Set(const T& aX, const T& aY)
		{
			x = aX;
			y = aY;
		}

		Vector2<T> Lerp(Vector2<T> aRhis, float aTValue)
		{
			x = CommonUtilities::Lerp(x, aRhis.x, aTValue);
			y = CommonUtilities::Lerp(y, aRhis.y, aTValue);
			return *this;
		}


		Vector2<T> Clamp(Vector2<T> aMinValue, Vector2<T> aMaxValue)
		{
			x = aMinValue.x > x ? aMinValue.x : aMaxValue.x < x ? aMaxValue.x : x;
			y = aMinValue.y > y ? aMinValue.y : aMaxValue.y < y ? aMaxValue.y : y;

			return Vector2<T>(x, y);
		}

		Vector2<T> RandomRange(Vector2<T> aMinValue, Vector2<T> aMaxValue)
		{
			std::random_device rd;
			std::uniform_real_distribution<T> d(aMinValue.x, aMaxValue.x);
			x = d(rd);

			d = std::uniform_real_distribution<T>(aMinValue.y, aMaxValue.y);
			y = d(rd);
			return Vector2<T>(x, y);
		}

		static float Distance(Vector2<T> aLhs, Vector2<T> aRhs)
		{
			return Abs((aLhs - aRhs).Length());
		}


		const bool operator==(Vector2<T> aRhs) {
			return x == aRhs.x && y == aRhs.y;
		}

		const bool operator!=(Vector2<T> aRhs) {
			return x != aRhs.x && y != aRhs.y;
		}

		template<typename OtherT>
		Vector2<OtherT> Cast() {
			return Vector2<OtherT>(static_cast<OtherT>(x), static_cast<OtherT>(y));
		}

	};





	template <class T>
	Vector2<T> operator+(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		Vector2<T> result(
			aVector0.x + aVector1.x,
			aVector0.y + aVector1.y
		);

		return result;
	}

	template <class T>
	Vector2<T> operator-(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		Vector2<T>result(
			aVector0.x - aVector1.x,
			aVector0.y - aVector1.y
		);

		return result;
	}

	template <class T>
	Vector2<T> operator*(const Vector2<T>& aVector, const T& aScalar)
	{
		Vector2<T>result(
			aVector.x * aScalar,
			aVector.y * aScalar
		);

		return result;
	}

	template <class T>
	Vector2<T> operator*(const T& aScalar, const Vector2<T>& aVector)
	{
		Vector2<T> result = aVector * aScalar;
		return result;
	}


	template <class T>
	Vector2<T> operator/(const Vector2<T>& aVector, T& aScalar)
	{
		Vector2<T>result(
			aVector.x / aScalar,
			aVector.y / aScalar
		);

		return result;
	}

	template <class T>
	void operator+=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		aVector0 = aVector0 + aVector1;
	}

	//Equivalent to setting aVector0 to (aVector0 - aVector1)
	template <class T>
	void operator-=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		aVector0 = aVector0 - aVector1;
	}

	//Equivalent to setting aVector to (aVector * aScalar)
	template <class T>
	void operator*=(Vector2<T>& aVector, const T& aScalar)
	{
		aVector = aVector * aScalar;
	}

	//Equivalent to setting aVector to (aVector / aScalar)
	template <class T>
	void operator/=(Vector2<T>& aVector, const T& aScalar)
	{
		aVector = aVector / aScalar;
	}


	template <class T>
	std::ostream& operator<<(std::ostream& aOutputStream, const Vector2<T>& aVector)
	{
		aOutputStream << "(x:" << aVector.x << ", y:" << aVector.y << ")";
		return aOutputStream;
	}



};