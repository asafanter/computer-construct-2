#ifndef REF_H
#define REF_H

template <typename T>
class Ref
{
public:
    Ref(T* ptr) :
        _ptr(ptr) {}

    Ref() :
        _ptr(nullptr){}

    Ref(T &ptr) :
        _ptr(&ptr) {}

    bool isNull() const
    {
        return _ptr == nullptr;
    }
    T* operator->()
    {
        return _ptr;
    }
    T &get()
    {
        return *_ptr;
    }

private:
    T* _ptr;
};

#endif // REF_H
