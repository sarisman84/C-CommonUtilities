#pragma once
#include <vector>


namespace CommonUtilities
{
	//Cant use >, == or !=
	//To check for equality:
	// x < y && y < x
	//To check for inequality:
	//!(x < y && y < x)



	template <class T>
	void SelectionSort(std::vector<T>& aVector)
	{
		size_t leftSide, rightSide, min;

		for (leftSide = 0; leftSide < aVector.size() - 1; leftSide++)
		{
			min = leftSide;
			for (rightSide = leftSide + 1; rightSide < aVector.size(); rightSide++)
			{
				if (aVector[rightSide] < aVector[min])
					min = rightSide;
			}


			std::swap(aVector[min], aVector[leftSide]);
		}
	}



	template <class T>
	void BubbleSort(std::vector<T>& aVector)
	{
		size_t leftSide, rightSide;

		for (leftSide = 0; leftSide < aVector.size() - 1; leftSide++)
		{
			for (rightSide = 0; rightSide < aVector.size() - leftSide - static_cast<size_t>(1); rightSide++)
			{
				if (aVector[rightSide + 1] < aVector[rightSide])
					std::swap(aVector[rightSide], aVector[rightSide + 1]);
			}



		}
	}
	template <class T>
	void QuickSort(std::vector<T>& aVector)
	{
		QuickSort(aVector, 0, aVector.size() - 1);
	}

	template <class T>
	void QuickSort(std::vector<T>& aVector, const int& aLow, const int& aHigh)
	{
		if (aLow < aHigh)
		{

			T pivot = aVector[aHigh];
			int smallI = (aLow - 1);

			for (int j = aLow; j < aHigh; j++)
			{
				if (aVector[j] < pivot)
				{
					smallI++;
					std::swap(aVector[smallI], aVector[j]);
				}
			}
			std::swap(aVector[smallI + 1], aVector[aHigh]);



			int pi = smallI + 1;



			QuickSort(aVector, aLow, pi - 1);
			QuickSort(aVector, pi + 1, aHigh);
		}
	}



	template <class T>
	void Merge(std::vector<T>& aVector, const int& aLeft, const int& aMid, const int& aRight)
	{

		auto const subVecOne = aMid - aLeft + 1;
		auto const subVecTwo = aRight - aMid;

		//Create temp vectors and copy their shit into each vector
		std::vector<T> lVec, rVec;
		for (size_t i = 0; i < subVecOne; i++)
		{
			lVec.push_back(aVector[aLeft + i]);
		}

		for (size_t i = 0; i < subVecTwo; i++)
		{
			rVec.push_back(aVector[aMid + 1 + i]);
		}


		auto indexVecOne = 0, indexVecTwo = 0;
		int indexMergeVec = aLeft;

		//Merge the two arrays
		while (indexVecOne < subVecOne && indexVecTwo < subVecTwo)
		{
			if (lVec[indexVecOne] < rVec[indexVecTwo] || (!(lVec[indexVecOne] < rVec[indexVecTwo]) && !(rVec[indexVecTwo] < lVec[indexVecOne])))
			{
				aVector[indexMergeVec] = lVec[indexVecOne];
				indexVecOne++;
			}
			else
			{
				aVector[indexMergeVec] = rVec[indexVecTwo];
				indexVecTwo++;
			}
			indexMergeVec++;
		}

		//Copy remaining shit of left vector if there is any
		while (indexVecOne < subVecOne)
		{
			aVector[indexMergeVec] = lVec[indexVecOne];
			indexVecOne++;
			indexMergeVec++;
		}
		//Copy remaining shit of right vector if there is any
		while (indexVecTwo < subVecTwo)
		{
			aVector[indexMergeVec] = rVec[indexVecTwo];
			indexVecTwo++;
			indexMergeVec++;
		}

	}

	template <class T>
	void MergeSort(std::vector<T>& aVector, const int& aBegin, const int& aEnd)
	{
		if (aEnd < aBegin || (!(aEnd < aBegin) && !(aBegin < aEnd)))
			return;

		auto mid = aBegin + (aEnd - aBegin) / 2;
		MergeSort(aVector, aBegin, mid);
		MergeSort(aVector, mid + 1, aEnd);
		Merge(aVector, aBegin, mid, aEnd);
	}

	template <class T>
	void MergeSort(std::vector<T>& aVector)
	{
		MergeSort(aVector, 0, static_cast<int>(aVector.size()) - 1);
	}

}