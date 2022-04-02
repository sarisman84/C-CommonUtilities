#pragma once
#include <iostream>
namespace CommonUtilities
{
	template <class T>
	class Vector4
	{

	public:

		T x;
		T y;
		T z;
		T w;

		//constructors
		Vector4() :
			x(T(0)),
			y(T(0)),
			z(T(0)),
			w(T(0))
		{
		}
		Vector4(const T& aX, const T& aY, const T& aZ, const T& aW) :
			x(T(aX)),
			y(T(aY)),
			z(T(aZ)),
			w(T(aW))
		{
		}
		Vector4(const Vector4<T>& aVector4) = default;

		//destructor
		~Vector4() = default;

		//assignment operator 
		Vector4<T>& operator=(const Vector4<T>& aVector4) = default;

		//Vector4 Magnitude SQUARED
		T LengthSqr() const
		{
			T result = (x * x + y * y + z * z + w * w);
			return result;

		}

		//Vectors Magnitude
		T Length() const
		{
			T result = sqrt(x * x + y * y + z * z + w * w);
			return result;
		}

		//normalize the current vector
		void Normalize()
		{
			if (x == 0 && y == 0 && z == 0 && w == 0)
				return;

			T length = Length();
			x /= length;
			y /= length;
			z /= length;
			w /= length;
		}

		//get a normalized version of the vector
		Vector4<T> GetNormalized() const
		{
			if (x == 0 && y == 0 && z == 0 && w == 0)
				return Vector4<T>();

			Vector4<T> tempVector(x, y, z, w);
			T length = Length();
			return (tempVector / length);
		}

		//multiplacations
		T Dot(const Vector4<T>& aRHSVector4) const
		{
			T result = x * aRHSVector4.x + y * aRHSVector4.y + z * aRHSVector4.z + w * aRHSVector4.w;

			return result;
		}

		//set 
		void Set(const T& aX, const T& aY, const T& aZ, const T& aW)
		{
			x = aX;
			y = aY;
			z = aZ;
			w = aW;


		}

		Vector4<T> Clamp(Vector4<T> aMinValue, Vector4<T> aMaxValue, Vector4<T> aCurrentValue)
		{
			int x = aMinValue.x < aCurrentValue.x ? aMinValue.x : aMaxValue.x > aCurrentValue.x ? aMaxValue.x : aCurrentValue.x;
			int y = aMinValue.y < aCurrentValue.y ? aMinValue.y : aMaxValue.y > aCurrentValue.y ? aMaxValue.y : aCurrentValue.y;
			int z = aMinValue.z < aCurrentValue.z ? aMinValue.z : aMaxValue.z > aCurrentValue.z ? aMaxValue.z : aCurrentValue.z;
			int w = aMinValue.w < aCurrentValue.w ? aMinValue.w : aMaxValue.w > aCurrentValue.w ? aMaxValue.w : aCurrentValue.w;

			return Vector4<T>(x, y, z, w);
		}
	};

	template <class T>
	Vector4<T> operator+(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		Vector4<T> result(
			aVector0.x + aVector1.x,
			aVector0.y + aVector1.y,
			aVector0.z + aVector1.z,
			aVector0.w + aVector1.w
		);

		return result;
	}

	template <class T>
	Vector4<T> operator-(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		Vector4<T>result(
			aVector0.x - aVector1.x,
			aVector0.y - aVector1.y,
			aVector0.z - aVector1.z,
			aVector0.w - aVector1.w
		);

		return result;
	}

	template <class T>
	Vector4<T> operator*(const Vector4<T>& aVector, const T& aScalar)
	{
		Vector4<T>result(
			aVector.x * aScalar,
			aVector.y * aScalar,
			aVector.z * aScalar,
			aVector.w * aScalar
		);

		return result;
	}

	template <class T>
	Vector4<T> operator*(const T& aScalar, const Vector4<T>& aVector)
	{
		Vector4<T> result = aVector * aScalar;
		return result;
	}


	template <class T>
	Vector4<T> operator/(const Vector4<T>& aVector, T& aScalar)
	{
		Vector4<T>result(
			aVector.x / aScalar,
			aVector.y / aScalar,
			aVector.z / aScalar,
			aVector.w / aScalar
		);

		return result;
	}

	template <class T>
	void operator+=(Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		aVector0 = aVector0 + aVector1;
	}


	template <class T>
	void operator-=(Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		aVector0 = aVector0 - aVector1;
	}


	template <class T>
	void operator*=(Vector4<T>& aVector, const T& aScalar)
	{
		aVector = aVector * aScalar;
	}


	template <class T>
	void operator/=(Vector4<T>& aVector, const T& aScalar)
	{
		aVector = aVector / aScalar;
	}


	template <class T>
	std::ostream operator<<(std::ostream& aOutputStream, const Vector4<T> aVector)
	{
		aOutputStream << "(x:" << aVector.x << ", y:" << aVector.y << ", z:" << aVector.z << ", w:" << aVector.w << ")";
		return aOutputStream;
	}

};