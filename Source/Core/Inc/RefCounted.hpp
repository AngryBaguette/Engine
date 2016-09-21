#pragma once

#include "Common.hpp"

#include <atomic>

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/** Intrusive reference counter **/
class RefCounted
{
public:
	/** Add a reference **/
	FORCEINLINE void addRef() const { ++mRef; }

	/** Remove a reference **/
	FORCEINLINE void removeRef() const { --mRef; }

	/** Get number of references **/
	FORCEINLINE int32_t countRef() const { int32_t lRef = --mRef; if (lRef EQ 0) delete this;  return lRef; }

protected:
	/** Default construtor **/
	FORCEINLINE RefCounted() : mRef(0) {}

	/** Default destructor **/
	virtual ~RefCounted() { assert(mRef EQ 0); }

private:
	/** The number of references **/
	mutable std::atomic<int32_t> mRef;
};

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/**
* Intrusive pointer that work on RefCounted object
* For non intrusive use smart_ptr
*/
template<typename T>
class RefPointer
{
	static_assert(std::is_base_of<RefCounted, T>::value, "RefPointer works only on RefCounted object");
public:
	/** Constructor **/
	FORCEINLINE RefPointer() : mObject(nullptr) {}
	FORCEINLINE RefPointer(T* pObject) : mObject(pObject) { if (mObject) mObject->addRef(); }
	FORCEINLINE RefPointer(const T* pObject) : mObject(pObject) { if (mObject) mObject->addRef(); }
	FORCEINLINE RefPointer(RefPointer<T>& pRef) : mObject(pRef.mObject) { if (mObject) mObject->addRef(); }

	/** Assigment operator **/
	FORCEINLINE RefPointer<T>& operator=(T *pObject) 
	{ 
		// Call addRef before removeRef to manage case of pObject equal to mObject
		T* lOldObject = mObject;
		mObject = pObject;
		if (mObject) mObject->addRef();
		if (lOldObject) lOldObject->removeRef();
		return *this;
	};

	/** Assigment operator **/
	FORCEINLINE RefPointer<T>& operator=(const RefPointer<T>& pObject) { return *this = pObject.mObject; }

	/** Null test operator **/
	FORCEINLINE operator bool() const { return mObject NEQ nullptr; }

	/** Not operator. **/
	FORCEINLINE bool operator!() const { return mObject EQ nullptr; }

	/** Dereferencing operator **/
	FORCEINLINE T* operator->() const { return mObject; }

	/** Dereferencing operator. **/
	FORCEINLINE T& operator*() const { return *mObject; }

	/** **/
	//FORCEINLINE T* operator()() const { return mObject;  }

	/** get raw pointer **/
	FORCEINLINE T* get() { return mObject; }
	FORCEINLINE T* get() const { return mObject; }

private:
	/** The object **/
	T* mObject;
};