#pragma once
#include <set>
#include <vector>

template<class T>
class zCArrayDegenerated
{
public:
	T* Array;
	int NumAlloc;
	int NumInArray;

	void Reallocate(const int nSize)
	{
		if (nSize == 0)
			return;

		T* pArray = new T[this->NumAlloc + nSize];

		if (this->NumInArray > 0)
		{
			for (int i = 0; i < this->NumInArray; i++)
				pArray[i] = Array[i];
		};

		delete[] this->Array;
		this->NumAlloc += nSize;
		this->Array = pArray;
	};

public:
	zCArrayDegenerated()
	{
		this->Array = NULL;
		this->NumAlloc = 0;
		this->NumInArray = 0;
	};

	~zCArrayDegenerated()
	{
		this->Clear();
	};

	const T& operator [] (const unsigned int pos) const
	{
		if ((int)pos <= this->NumInArray)
			return this->Array[pos];
	};

	T& operator [] (const unsigned int pos)
	{
		if ((int)pos <= this->NumInArray)
			return this->Array[pos];
	};

	/** Copy data from std::vector */
	void FromVector(const std::vector<T> &v)
	{
		Clear();
		Reallocate(v.size());
		memcpy(Array, &v[0], sizeof(T) * v.size());

		this->NumInArray = v.size();
	}

	void Clear()
	{
		this->NumAlloc = 0;
		this->NumInArray = 0;
		if (this->Array != NULL)
		{
			delete[] this->Array;
			this->Array = NULL;
		};
	};

	/**
	* @brief Adds an element to the zCArray.
	*
	* This method adds an element to the zCArray. If the preallocated memory of the zCArray
	* is full then this method will allocate one additional field to store the the new element.
	* This behaviour saves memory but is very inefficient when adding many elements as memory
	* is allocated on every method call when the preallocated area is full.
	*
	* This method can be interleaved with PushBackFast.
	*/
	void PushBack(const T& in)
	{
		if ((this->NumInArray + 1) > this->NumAlloc) // -- PB throws around with memory. But we don't, so we use the allocated space and don't allocate more and more memory... (memory leak!?)
			Reallocate(1);

		this->Array[this->NumInArray++] = in;
	};

	/**
	* @brief Adds an element to the zCArray.
	*
	* This method adds an element to the zCArray. If the preallocated memory of the zCArray
	* is full then this method will double the size of the preallocated memory. This might lead
	* to the worst case situation where twice as much memory than needed is used for this zCArray.
	* Use this method in often created/short lived zCArrays.
	*
	* This method can be interleaved with PushBack.
	*/
	void PushBackFast(const T& in)
	{
		if ((this->NumInArray + 1) > this->NumAlloc) // -- Let's throw around with memory like PB does for performance's sake.
			Reallocate(this->NumAlloc * 2);

		this->Array[this->NumInArray++] = in;
	};

	const T& GetItem(const unsigned int pos)
	{
		if ((pos <= this->NumInArray) && (pos <= this->NumAlloc))
			return this->Array[pos];
	};

	unsigned int GetSize(void)
	{
		return (unsigned int)this->NumInArray;
	};

	int Search(const T& item)
	{
		for (size_t i = 0; i < GetSize(); i++)
		{
			if (Array[i] == item)
				return i;
		}

		return -1;
	};

	bool IsInList(const T& item)
	{
		for (size_t i = 0; i < GetSize(); i++)
		{
			if (Array[i] == item)
				return true;
		}

		return false;
	};
};
