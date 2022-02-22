#pragma once
#include <iostream>
#include <string>
namespace CommonUtilities
{
	template <class T>
	class Vector3
	{

	public:

		T x;
		T y;
		T z;

		//constructors
		Vector3() :
			x(T(0)),
			y(T(0)),
			z(T(0))
		{
		}
		Vector3(const T& aX, const T& aY, const T& aZ) :
			x(T(aX)),
			y(T(aY)),
			z(T(aZ))
		{
		}
		Vector3(const Vector3<T>& aVector3) = default;

		//destructor
		~Vector3() = default;

		//assignment operator 
		Vector3<T>& operator=(const Vector3<T>& aVector3) = default;

		//Vector3 Length
		T LengthSqr() const
		{
			T result = (x * x + y * y + z * z);
			return result;

		}

		T Length() const
		{
			T result = sqrt(x * x + y * y + z * z);
			return result;
		}

		//normalize
		void Normalize()
		{
			if (x == 0 && y == 0 && z == 0)
				return;

			T length = Length();
			x /= length;
			y /= length;
			z /= length;
		}

		Vector3<T> GetNormalized() const
		{
			if (x == 0 && y == 0 && z == 0)
				return Vector3<T>();
			Vector3<T> tempVector(x, y, z);
			T length = Length();
			return (tempVector / length);
		}

		//multiplacations
		T Dot(const Vector3<T>& aRHSVector3) const
		{
			T result = x * aRHSVector3.x + y * aRHSVector3.y + z * aRHSVector3.z;

			return result;
		}

		Vector3<T>
			Cross(const Vector3<T>& aRHSVector3) const
		{
			return Vector3(
				y * aRHSVector3.z - aRHSVector3.y * z,
				z * aRHSVector3.x - aRHSVector3.z * x,
				x * aRHSVector3.y - aRHSVector3.x * y
			);
		}

		//set
		void Set(const T& aX, const T& aY, const T& aZ) 
		{
			x = aX;
			y = aY;
			z = aZ;
		}


		Vector3<T> Clamp(Vector3<T> aMinValue, Vector3<T> aMaxValue, Vector3<T> aCurrentValue)
		{
			int x = aMinValue.x < aCurrentValue.x ? aMinValue.x : aMaxValue.x > aCurrentValue.x ? aMaxValue.x : aCurrentValue.x;
			int y = aMinValue.y < aCurrentValue.y ? aMinValue.y : aMaxValue.y > aCurrentValue.y ? aMaxValue.y : aCurrentValue.y;
			int z = aMinValue.z < aCurrentValue.z ? aMinValue.z : aMaxValue.z > aCurrentValue.z ? aMaxValue.z : aCurrentValue.z;

			return Vector3<T>(x, y, z);
		}

	};

	template <class T>
	Vector3<T> operator+(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		Vector3<T> result(
			aVector0.x + aVector1.x,
			aVector0.y + aVector1.y,
			aVector0.z + aVector1.z
		);

		return result;
	}

	template <class T>
	Vector3<T> operator-(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		Vector3<T>result(
			aVector0.x - aVector1.x,
			aVector0.y - aVector1.y,
			aVector0.z - aVector1.z
		);

		return result;
	}

	template <class T>
	Vector3<T> operator*(const Vector3<T>& aVector, const T& aScalar)
	{
		Vector3<T>result(
			aVector.x * aScalar,
			aVector.y * aScalar,
			aVector.z * aScalar
		);

		return result;
	}

	template <class T>
	Vector3<T> operator*(const T& aScalar, const Vector3<T>& aVector)
	{
		Vector3<T> result = aVector * aScalar;
		return result;
	}

	template <class T>
	Vector3<T> operator/(const Vector3<T>& aVector, T& aScalar)
	{
		Vector3<T>result(
			aVector.x / aScalar,
			aVector.y / aScalar,
			aVector.z / aScalar
		);

		return result;
	}

	template <class T>
	void operator+=(Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		aVector0 = aVector0 + aVector1;
	}

	//Equivalent to setting aVector0 to (aVector0 - aVector1)
	template <class T>
	void operator-=(Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		aVector0 = aVector0 - aVector1;
	}

	//Equivalent to setting aVector to (aVector * aScalar)
	template <class T>
	void operator*=(Vector3<T>& aVector, const T& aScalar)
	{
		aVector = aVector * aScalar;
	}

	//Equivalent to setting aVector to (aVector / aScalar)
	template <class T>
	void operator/=(Vector3<T>& aVector, const T& aScalar)
	{
		aVector = aVector / aScalar;
	}


	template <class T>
	std::ostream operator<<(std::ostream& aOutputStream, const Vector3<T> aVector)
	{
		aOutputStream << "(x:" << aVector.x << ", y:" << aVector.y << ", z:" << aVector.z << ")";
		return aOutputStream;
	}




};