#pragma once

#include <functional>

namespace Tga2D
{
	struct SStackTraceImpl;
	class CStackTrace
	{
	public:
		CStackTrace() : myImpl(nullptr) {}

		static CStackTrace CaptureStackTrace(int aSkipDepth);
		void Print() const;
		std::size_t ComputeHash() const;
		bool operator==(const CStackTrace& aStackTrace) const noexcept
		{
			return myImpl == aStackTrace.myImpl;
		}
	private:
		CStackTrace(const SStackTraceImpl&);
		const SStackTraceImpl* myImpl;
	};
}

namespace std
{
	template<> struct hash<Tga2D::CStackTrace>
	{
		std::size_t operator()(const Tga2D::CStackTrace& aStackTrace) const noexcept
		{
			return aStackTrace.ComputeHash();
		}
	};
}
