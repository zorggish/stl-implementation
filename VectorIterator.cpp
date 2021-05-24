#include <iterator>

using namespace std;

template<typename T> class VectorIterator : public iterator<random_access_iterator_tag, T>
{
protected:
    T *start;
    T *current;
    typedef iterator<random_access_iterator_tag, T> iterator;
public:
    VectorIterator(T* position): start(position), current(position) {}
    VectorIterator(const VectorIterator &iter): start(iter.start), current(iter.current) {}
    virtual ~VectorIterator(){}
    VectorIterator& operator+=(int n) { current+=n; return *this; }
    VectorIterator& operator-=(int n) { current-=n; return *this; }
    VectorIterator operator-(typename iterator::difference_type n) { VectorIterator temp(*this); return temp -= n; }
    typename iterator::difference_type operator-(VectorIterator iter) { return current-iter.current; }
    VectorIterator& operator++() { current++; return *this; }
    VectorIterator& operator--() { current--; return *this; }
    void operator++(int) { current++; }
    void operator--(int) { current--; }
    bool operator==(const VectorIterator &iter) { return current==iter.current; }
    bool operator!=(const VectorIterator &iter) { return current!=iter.current; }
    typename iterator::reference operator*() { return *current; }
    typename iterator::reference operator[](typename iterator::difference_type n) { return start[n]; }
};

template <typename T> VectorIterator<T> operator+ (VectorIterator<T> iter, int n)
{
    return VectorIterator<T>(iter += n);
}

template <typename T> VectorIterator<T> operator+ (int n, VectorIterator<T> iter)
{
    return VectorIterator<T>(iter += n);
}

template <typename T> bool operator< (VectorIterator<T> first, VectorIterator<T> second)
{
    return second - first > 0;
}

template <typename T> bool operator> (VectorIterator<T> first, VectorIterator<T> second)
{
    return first - second > 0;
}

template <typename T> bool operator<= (VectorIterator<T> first, VectorIterator<T> second)
{
    return !(first > second);
}

template <typename T> bool operator>= (VectorIterator<T> first, VectorIterator<T> second)
{
    return !(first < second);
}
