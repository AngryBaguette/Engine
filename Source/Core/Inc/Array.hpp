#pragma once

#include "Common.hpp"
#include "Memory.hpp"
#include "MemTpl.hpp"
#include "Math.hpp"

#include <initializer_list>

/**
 * Contiguous array
 */
template<typename T>
class Array
{
public:
	/** Iterator are simple pointer and can be invalidate if a reallocation happen */
	typedef T* Iterator;
	typedef const T* ConstIterator;
	typedef Iterator iterator;
	typedef ConstIterator const_iterator;

	/** Default constructor */
	Array() : mCount(0), mCapacity(0)
	{
		if (mCapacity NEQ 0)
		{
			mData = (T*)Memory::Malloc(sizeof(T) * mCapacity);
		}
		else
		{
			mData = nullptr;
		}
	}

	/** Preallocated constructor */
	Array(uint32_t pCount) : mCount(pCount), mCapacity(pCount)
	{
		if (mCount NEQ 0)
		{
			mData = (T*)Memory::Malloc(sizeof(T) * mCapacity);
		}
		else
		{
			mData = nullptr;
		}
	}

	/** initializer list constructor */
	Array(std::initializer_list<T> pList) 
	{
		mCount = mCapacity = pList.size();
		assert(mCapacity NEQ 0);	// Not expected, empty initializer list should call default constructor
		mData = (T*)Memory::Malloc(sizeof(T) * mCapacity);
		CopyAssignItems(mData, pList.begin(), pList.size());
	}

	/** Copy constructor */
	Array(const Array<T>& pOther) : mCount(0), mCapacity(0), mData(nullptr)
	{ 
		operator=(pOther);
	}

	/** Copy operator */
	Array<T>& operator=(const Array<T>& pOther)
	{
		assert(this NEQ &pOther);
		if
			(pOther.mCount >= mCount)
		{
			addUninitialized(pOther.mCount - mCount);
		}
		else
		{
			DestructItems(mData + pOther.mCount, mCount - pOther.mCount);
			mCount = pOther.mCount;
			shrink();
		}

		assert(pOther.mCount EQ mCount);
		ConstructItems(mData, pOther.mData, pOther.mCount);
		return *this;
	}

	/** Move constructor */
	Array(Array<T>&& pOther) : mCount(0), mCapacity(0), mData(nullptr)
	{
		mCount = pOther.mCount;
		mCapacity = pOther.mCapacity;
		mData = pOther.mData;

		pOther.mCount = 0;
		pOther.mCapacity = 0;
		pOther.mData = nullptr;
	}

	/** Destructor */
	~Array()
	{
		DestructItems(mData, mCount);
		Memory::Free(mData);
	}

	/** Number of elements */
	FORCEINLINE size_t count() const { return mCount;  }

	/** Size in memory */
	FORCEINLINE size_t dataSize() const { return mCount * sizeof(T); };

	/** random accessor */
	FORCEINLINE T& operator[](size_t pIndex)
	{
		assert(pIndex < mCount); // Invalid access
		return ((T*)mData)[pIndex];
	}

	/** Resize the array **/
	FORCEINLINE void resize(size_t pCount)
	{
		if
			(pCount < mCount)
		{
			DestructItems(&mData[pCount], mCount - pCount);
			mCount = pCount;
		}
		else if
			(pCount > mCount)
		{
			addUninitialized(pCount - mCount);
		}
	}

	FORCEINLINE void reserve(size_t pCapacity)
	{
		if
			(pCapacity > mCapacity)
		{
			mCapacity = pCapacity;
			mData = (T*)Memory::Realloc(mData, mCapacity * sizeof(T));
		}
	}

	/** Add an element */
	FORCEINLINE void add(const T& pElement)
	{
		const size_t lIndex = addUninitialized(1);
		ConstructItems(&mData[lIndex], &pElement, 1);
	}

	/** Insert an element at desired position */
	FORCEINLINE void insert(const T& pElement, size_t pIndex = ~0)
	{
		if
			(pIndex EQ ~0 OR pIndex EQ mCount)
		{
			const size_t lIndex = addUninitialized(1);
			// Insert at the end
			ConstructItems(&mData[lIndex], &pElement, 1);
		}
		else
		{
			assert(pIndex <= mCount);
			if (mCount + 1 > mCapacity)
			{
				mCapacity = (size_t)((mCount + 1) * 1.5);
				T* lOdData = mData;
				mData = (T*)Memory::Malloc(mCapacity * sizeof(T));
				// Copy data before pIndex
				Memory::Memcpy(mData, lOdData, pIndex);
				// Copy data after pIndex
				Memory::Memcpy(mData + pIndex + 1, lOdData + pIndex, mCount - pIndex);
				Memory::Free(lOdData);
			}
			else
			{
				// Shift to the right the data after the pIndex
				Memory::Memmove(mData + pIndex + 1, mData + pIndex, mCount - pIndex);
			}

			// Finally assign the new
			ConstructItems(&mData[pIndex], &pElement, 1);
			++mCount;
		}
	}

	/** Force to freed the unused memory */
	FORCEINLINE void shrink()
	{
		if (mCount < mCapacity)
		{
			mCapacity = mCount;
			if(mCapacity EQ 0)
			{
				Memory::Free(mData);
				mData = nullptr;
			}
			else
			{
				mData = (T*)Memory::Realloc(mData, mCapacity);
			}
		}
	}

	/** STL begin iterator */
	FORCEINLINE Iterator begin() const
	{
		return mData;
	}

	/** STL end iterator */
	FORCEINLINE Iterator end() const
	{
		return mData + mCount;
	}

	/** Last element of the container */
	FORCEINLINE Iterator back() const
	{
		if (mCount NEQ 0)
		{
			return mData + mCount - 1;
		}
		return end();
	}

	/* Realloc if necessary in order to store 'pCount' new entries */
	FORCEINLINE size_t addUninitialized(size_t pCount)
	{
		const size_t lCount = mCount;
		if (mCount + pCount > mCapacity)
		{
			mCapacity = (size_t)((mCount + pCount) * 1.5);
			mData = (T*)Memory::Realloc(mData, mCapacity * sizeof(T));
		}
		mCount += pCount;
		return lCount;
	}

	/** the allocated buffer data */
	FORCEINLINE T* data() { return mData; }

private:
	T*			mData;
	size_t		mCapacity;
	size_t		mCount;

private:
};

