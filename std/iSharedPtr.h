#pragma once

#include "iType.h"

template<class T>
class iSharedPtr
{
public:
	iSharedPtr();
	iSharedPtr(T* ptr);	
	iSharedPtr(const iSharedPtr<T>& sharedPtr);
	virtual ~iSharedPtr();
	
	iSharedPtr<T>& operator=(T* ptr);
	iSharedPtr<T>& operator=(const iSharedPtr<T>& sharedPtr);
	T& operator[](const int idx);
		
	T* get();
	
private:
	uint32* referenceCount;
	T* ptr;
};

template<class T>
iSharedPtr<T>::iSharedPtr()
{
	ptr = NULL;
	referenceCount = new uint32;
	*referenceCount = 1;
}

template<class T>
iSharedPtr<T>::iSharedPtr(T* p)
{
	ptr = p;
	referenceCount = new uint32;
	*referenceCount = 1;
}

template<class T>
iSharedPtr<T>::iSharedPtr(const iSharedPtr<T>& sp)
{
	referenceCount = sp.referenceCount;
	(*referenceCount)++;
	ptr = sp.ptr;
}

template<class T>
iSharedPtr<T>::~iSharedPtr()
{
	(*referenceCount)--;
	if (*referenceCount == 0)
	{
		delete referenceCount;
		if (ptr) delete ptr;
	}
}

template<class T>
iSharedPtr<T>& iSharedPtr<T>::operator=(T* p)
{
	(*referenceCount)--;
	if (*referenceCount == 0)
	{
		*referenceCount = 1;
		if (ptr) delete ptr;
	}
	else
	{
		referenceCount = new uint32;
		*referenceCount = 1;
	}

	ptr = p;

	return *this;
}

template<class T>
iSharedPtr<T>& iSharedPtr<T>::operator=(const iSharedPtr<T>& sp)
{
	(*referenceCount)--;
	if (*referenceCount == 0)
	{
		delete referenceCount;
		if (ptr) delete ptr;
	}

	referenceCount = sp.referenceCount;
	(*referenceCount)++;
	ptr = sp.ptr;

	return *this;
}

template<class T>
T& iSharedPtr<T>::operator[](const int idx)
{
	return ptr[idx];
}

template<class T>
T* iSharedPtr<T>::get()
{
	return ptr;
}

template<class T>
class iSharedPtr<T[]>
{
public:
	iSharedPtr();
	iSharedPtr(T* ptr);
	iSharedPtr(const iSharedPtr<T[]>& sharedPtr);
	virtual ~iSharedPtr();

	iSharedPtr<T[]>& operator=(T* ptr);
	iSharedPtr<T[]>& operator=(const iSharedPtr<T[]>& sharedPtr);
	T* operator[](const int idx);

	T* get();

private:
	uint32* referenceCount;
	T* ptr;
};

template<class T>
iSharedPtr<T[]>::iSharedPtr()
{
	ptr = NULL;
	referenceCount = new uint32;
	*referenceCount = 1;
}

template<class T>
iSharedPtr<T[]>::iSharedPtr(T* p)
{
	ptr = p;
	referenceCount = new uint32;
	*referenceCount = 1;
}

template<class T>
iSharedPtr<T[]>::iSharedPtr(const iSharedPtr<T[]>& sp)
{
	referenceCount = sp.referenceCount;
	(*referenceCount)++;
	ptr = sp.ptr;
}

template<class T>
iSharedPtr<T[]>::~iSharedPtr()
{
	(*referenceCount)--;
	if (*referenceCount == 0)
	{
		delete referenceCount;
		if (ptr) delete[] ptr;
	}
}

template<class T>
iSharedPtr<T[]>& iSharedPtr<T[]>::operator=(T* p)
{
	(*referenceCount)--;
	if (*referenceCount == 0)
	{
		*referenceCount = 1;
		if (ptr) delete[] ptr;
	}
	else
	{
		referenceCount = new uint32;
		*referenceCount = 1;
	}

	ptr = p;

	return *this;
}

template<class T>
iSharedPtr<T[]>& iSharedPtr<T[]>::operator=(const iSharedPtr<T[]>& sp)
{
	(*referenceCount)--;
	if (*referenceCount == 0)
	{
		delete referenceCount;
		if (ptr) delete[] ptr;
	}

	referenceCount = sp.referenceCount;
	(*referenceCount)++;
	ptr = sp.ptr;

	return *this;
}

template<class T>
T* iSharedPtr<T[]>::operator[](const int idx)
{
	return &ptr[idx];
}

template<class T>
T* iSharedPtr<T[]>::get()
{
	return ptr;
}


