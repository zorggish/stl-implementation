#include "VectorIterator.cpp"

template<class T> class Vector
{
protected:
    typedef VectorIterator<T> iterator;
    typedef VectorIterator<const T> const_iterator;
    typedef reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    static const size_t additionalCapacity = 16;

    size_t length;
    size_t storageLength;
    T *storage;

    class Exception {};
    class Range : public Exception {};
    class Size : public Exception {};
    void resizeStorage(size_t newSize);
public:
    Vector(size_t size=0): storage(new T[size+additionalCapacity]), length(size), storageLength(size+additionalCapacity) {}
    Vector(const Vector &another);
    virtual ~Vector();
    Vector& operator=(const Vector &another);

    //Capacity
    size_t size() const noexcept { return length; }
    size_t capacity() const noexcept { return storageLength; }
    bool empty() const noexcept { return length==0; }
    void shrink_to_fit() noexcept;

    //Element access
    T& operator[](size_t index) noexcept;
    T& at(size_t index);
    T* data() { return storage; }
    const T* data() const { return storage; }
    T& front() { if(length==0) throw Range(); return storage[0]; }
    T& back() { if(length==0) throw Range(); return storage[length-1]; }

    //Modifiers
    void clear() noexcept;
    void push_back(T value);
    T pop_back();
    void resize(size_t newSize) noexcept;
    void insert(size_t newIndex, T value);
    void insert(size_t index, const T* array, size_t length);
    void insert(size_t index, const Vector<T> &another);
    void erase(size_t index);
    void erase(size_t index, size_t length);
    void swap(Vector& other);

    //Iterators
    iterator begin();
    iterator end();
    const_iterator cbegin() const;
    const_iterator cend() const;
    reverse_iterator rbegin();
    reverse_iterator rend();
    const_reverse_iterator crbegin() const;
    const_reverse_iterator crend() const;
};
template<class T>  void Vector<T>::resizeStorage(size_t newSize)
{
    T *newStorage = new T[storageLength = newSize];
    length = min(length, newSize);
    for(size_t i = 0; i < length; ++i)
        newStorage[i] = storage[i];
    delete[] storage;
    storage=newStorage;
}
template<class T>  Vector<T>::Vector(const Vector &another): length(another.length), storageLength(another.storageLength), storage(new T[storageLength])
{
    for(int i = 0; i < length; ++i)
        storage[i] = another.storage[i];
}
template<class T>  Vector<T>::~Vector()
{
    delete[] storage;
}
template<class T>  Vector<T>& Vector<T>::operator=(const Vector &another)
{
    resizeStorage(another.length);
    for(size_t i = 0; i < length; ++i)
        storage[i] = another.storage[i];
    return *this;
}
template<class T> T& Vector<T>::operator[](size_t index) noexcept
{
    return storage[index];
}
template<class T> T& Vector<T>::at(size_t index)
{
    if(index >= length)
        throw Range();
    return storage[index];
}
template<class T> void Vector<T>::push_back(T value)
{
    if(length==storageLength)
        resizeStorage(storageLength+additionalCapacity);
    storage[length++] = value;
}
template<class T> T Vector<T>::pop_back()
{
    if(length == 0) throw Size();
    return storage[--length];
}
template<class T> void Vector<T>::shrink_to_fit() noexcept
{
    resizeStorage(length);
}
template<class T> void Vector<T>::clear() noexcept
{
    delete[] storage;
    storage = new T[storageLength = additionalCapacity];
    length=0;
}
template<class T> void Vector<T>::resize(size_t newSize) noexcept
{
    if(newSize == length) return;
    if(newSize < length)
        length=newSize;
    else //newSize > length
    {
        if(newSize <= storageLength)
            length=newSize;
        else // newsize > storageLength
            resizeStorage((length = newSize)+additionalCapacity);
    }
}
template<class T> void Vector<T>::insert(size_t newIndex, T value)
{

    if(newIndex > length) throw Range();
    if(newIndex == length) push_back(value);
    else
    {
        if(length == storageLength)
            resizeStorage(storageLength+additionalCapacity);
        length++;
        for(size_t i = length-1; i > newIndex; --i)
            storage[i] = storage[i-1];
        storage[newIndex] = value;
    }
}
template<class T> void Vector<T>::insert(size_t index, const T* array, size_t insertionLength)
{
    if(index > length) throw Range();
    if(length+insertionLength > storageLength)
            resizeStorage(length+insertionLength+additionalCapacity);
    if(index == length)
        for(int i = 0; i < insertionLength; ++i)
            storage[length + i] = array[i];
    else
    {
        for(size_t i = length+insertionLength-1; i >= index+insertionLength; --i)
            storage[i]=storage[i-insertionLength];

        for(size_t i = 0; i < insertionLength; ++i)
            storage[index+i] = array[i];

    }
    length+=insertionLength;
}
template<class T> void Vector<T>::insert(size_t index, const Vector<T> &another)
{
    insert(index, another.data(), another.size());
}
template<class T> void Vector<T>::erase(size_t index)
{
    if(index > length) throw Range();
    if(index == length) pop_back();
    else
    {
        for(int i = index; i < length; ++i)
            storage[i]=storage[i+1];
        length--;
    }
}
template<class T> void Vector<T>::erase(size_t index, size_t eraseLength)
{
    if(index + eraseLength > length) throw Range();
    if(eraseLength == 0) return;
    for(int i = index; i < length - eraseLength; ++i)
        storage[i] = storage[i+eraseLength];
    length -= eraseLength;
}
template<class T> void Vector<T>::swap(Vector &another)
{
    T* temp = storage;
    storage = another.storage;
    another.storage = temp;

    size_t t = length;
    length = another.length;
    another.length = t;

    t = storageLength;
    storageLength = another.storageLength;
    another.storageLength = t;
}
template<class T> typename Vector<T>::iterator Vector<T>::begin()
{
    return iterator(storage);
}
template<class T> typename Vector<T>::iterator Vector<T>::end()
{
    return iterator(&storage[length]);
}
template<class T> typename Vector<T>::const_iterator Vector<T>::cbegin() const
{
    return const_iterator(storage);
}
template<class T> typename Vector<T>::const_iterator Vector<T>::cend() const
{
    return const_iterator(&storage[length]);
}
template<class T> typename Vector<T>::reverse_iterator Vector<T>::rbegin()
{
    return reverse_iterator(&storage[length]);
}
template<class T> typename Vector<T>::reverse_iterator Vector<T>::rend()
{
    return reverse_iterator(storage);
}
template<class T> typename Vector<T>::const_reverse_iterator Vector<T>::crbegin() const
{
    return const_reverse_iterator(&storage[length]);
}
template<class T> typename Vector<T>::const_reverse_iterator Vector<T>::crend() const
{
    return const_reverse_iterator(storage);
}
