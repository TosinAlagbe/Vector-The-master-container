template<typename T>
class Allocator
{
public:
	T * allocate(int n);  //allocate space for n objects of type T 
	void deallocate(T* p, int n);  //deallocate n objects of type T starting at p

	void construct(T* p, const T& v);  //construct a T with the value v in p
	void destroy(T* p);  //destroy the T in p
};

template<typename T>
T* Allocator<T>::allocate(int n)
{
	T* new_mem = (T*)malloc(n * sizeof(T));
	return new_mem;
}

template<typename T>
void Allocator<T>::construct(T* p, const T& v)
{
	T* constructed_object = new(p) T{ v };
}

template<typename T>
void Allocator<T>::deallocate(T* p, int n)
{
	free(p);
}

template<typename T>
void Allocator<T>::destroy(T* p)
{
	p->~T();
}
