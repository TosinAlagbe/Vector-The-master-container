#include<utility>
using std::swap;

template<typename T>
class unique_ptr
{
private:
	T * tracker;

public:
	unique_ptr() :tracker{ nullptr } {}
	explicit unique_ptr(T* trck) : tracker{ trck } {}
	T* release();
	~unique_ptr()
	{
		delete[] tracker;
	}
	T& operator*() const;
	T* operator->() const
	{
		return tracker;
	}

};

template<typename T>
T* unique_ptr<T>::release()
{
	T* result = nullptr;
	std::swap(result, tracker);
	return tracker;
}

template<typename T>
T& unique_ptr<T>::operator*()const
{
	return(*tracker);
}


