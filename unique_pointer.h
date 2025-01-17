#pragma once



template<typename T>
class UniquePointer {

private:

    T* pointer;

public:

    UniquePointer() : pointer(nullptr) {}

    UniquePointer(T* p) : pointer(p) {}

    UniquePointer(const UniquePointer&) = delete;
    UniquePointer& operator = (const UniquePointer&) = delete;

    UniquePointer(UniquePointer&& other) noexcept : pointer(other.pointer)
    {
        other.pointer = nullptr;
    }

    UniquePointer& operator = (UniquePointer&& other) noexcept
    {
        if (this != &other) {
            delete pointer;
            pointer = other.pointer;
            other.pointer = nullptr;
        }

        return *this;
    }

    T& operator*() const
    {
        return *pointer;
    }

    T* operator->() const
    {
        return pointer;
    }

    T* Get() const
    {
        return pointer;
    }

    T* Release()
    {
        T* oldPtr = pointer;
        pointer = nullptr;
        return oldPtr;
    }

    void Reset(T* newPtr = nullptr)
    {
        delete pointer;
        pointer = newPtr;
    }

    ~UniquePointer()
    {
        delete pointer;
    }
};
