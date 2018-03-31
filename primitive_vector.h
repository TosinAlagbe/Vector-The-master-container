#include "primitive_allocator.h"
#include <initializer_list>
using std::initializer_list;


template<typename T, typename A = Allocator<T>>
class vector
{
private:
	A alloc;
	T* elem;
	int sz;
	int space;

public:
	vector() :sz{ 0 }, elem{ nullptr }, space{ 0 } {}
	explicit vector(int s)
		:sz{ s }, elem{ new T[s] }, space{ s }
	{
		for (int i = 0; i < sz; ++i)elem[i] = T();
	}
	vector(initializer_list<T>lst)
		:sz{ int(lst.size()) }, elem{ alloc.allocate(lst.size()) }, space{ 8 * int(lst.size()) }
	{
		int i = 0;
		for (auto c : lst) alloc.construct(&elem[i++], c);
		if (i == sz)return;
	}

	vector& operator=(const vector&);

	vector(vector&& a);
	vector& operator=(vector&&);

	vector(const vector&);
	~vector() { delete[] elem; }

	T& at(int);
	const T& at(int) const;

	T& operator[](int n) { return elem[n]; }
	const T& operator[](int n) const { return elem[n]; }

	int size()const { return sz; }


	void reserve(int newalloc);
	int capacity()const { return space; }
	void resize(int newsize, T def = T());
	void push_back(const T& d);
};

template<typename T, typename A>
T& vector<T, A>::at(int n)
{
	if (n < 0 || sz <= n)throw out_of_range("s");
	if (!n)throw out_of_range("t");
	return elem[n];
}


template<typename T, typename A>
void vector<T, A>::push_back(const T& val)
{
	if (space == 0)
		reserve(8);
	else if (space == sz)
		reserve(2 * space);
	alloc.construct(&elem[sz], val);
	++sz;
}

template<typename T, typename A>
void vector<T, A>::resize(int newsize, T val)
{
	reserve(newsize);
	for (int i = sz; i < newsize; ++i)alloc.construct(&elem[i], val);
	for (int i = newsize; i < sz; ++i)alloc.destroy(&elem[[i]]);
	sz = newsize;
}

template<typename T, typename A>
void vector<T, A>::reserve(int newalloc)
{
	if (newalloc <= space)return;
	T* p = alloc.allocate(newalloc);
	for (int i = 0; i < sz; ++i)alloc.construct(&p[i], elem[i]);
	for (int i = 0; i < sz; ++i)alloc.destroy(&elem[i]);
	alloc.deallocate(elem, sz);
	elem = p;
	space = newalloc;
}

template<typename T, typename A>
vector<T, A>::vector(const vector& arg)
	:sz{ arg.sz }, elem{ new T[arg.sz] }
{
	copy(arg.elem, arg.elem + sz, elem);
}

template<typename T, typename A>
vector<T, A>& vector<T, A>::operator=(const vector& a)
{
	if (this == &a) return *this;

	if (a.sz <= space)
	{
		for (int i = 0; i < a.sz; ++i)alloc.construct(&elem[i], a.elem[i]);
		sz = a.sz;
		return *this;
	}
	T* p = alloc.allocate(a.sz);
	for (int i = 0; i < a.sz; ++i)alloc.construct(&p[i], a.elem[i]);
	for (int i = 0; i < sz; ++i)alloc.destroy(&elem[i]);
	alloc.deallocate(elem, sz);
	space = sz = a.sz;
	elem = p;
	return *this;
}

template<typename T, typename A>
vector<T, A>::vector(vector&& a)
	:sz{ a.sz }, elem{ a.elem }
{
	a.sz = 0;
	a.elem = nullptr;
}

template<typename T, typename A>
vector<T, A>& vector<T, A>::operator=(vector&& a)
{
	delete[] elem;
	elem = a.elem;
	sz = a.sz;
	a.elem = nullptr;
	a.sz = 0;
	return *this;
}
