#pragma once

#include <optional>

#include "idictionary.h"
#include "unique_pointer.h"
#include "dynamic_array.h"
#include "array_sequence.h"



template <typename TKey, typename TValue>
class HashNode
{
public:
    TKey key;
    TValue value;

    HashNode(TKey key, TValue value) : key(key), value(value) {}
};

template <typename TKey, typename TValue, typename Hash = std::hash<TKey>>
class HashTable : public IDictionary<TKey, TValue>
{
private:

    ArraySequence<UniquePointer<HashNode<TKey, TValue>>> array;
    int capacity;
    int size;
    Hash hashFunction;

    void Resize(int newCapacity)
    {
        int oldCapacity = capacity;
        capacity = newCapacity;
        ArraySequence<UniquePointer<HashNode<TKey, TValue>>> newArray(capacity);

        for (int i = 0; i < oldCapacity; i++)
        {
            if (array[i].Get())
            {
                int hashIndex = HashCode(array[i].Get()->key);

                while (newArray[hashIndex].Get())
                    hashIndex = (hashIndex + 1) % capacity;

                newArray[hashIndex] = std::move(array[i]);
            }
        }

        array = std::move(newArray);
    }

public:

    HashTable(int capacity = 20)
    {
        int thisCapacity;

        if (capacity <= 0)
        {
            thisCapacity = 20;
        }
        else
        {
            thisCapacity = capacity;
        }

        this->capacity = thisCapacity;
        size = 0;
        array = ArraySequence<UniquePointer<HashNode<TKey, TValue>>>(thisCapacity);
    }

    HashTable(HashTable&& other) noexcept
            : array(std::move(other.array)),
              capacity(other.capacity),
              size(other.size),
              hashFunction(std::move(other.hashFunction))
    {
        other.capacity = 0;
        other.size = 0;
    }

    HashTable& operator=(const HashTable& other)
    {
        if (this != &other)
        {
            array = std::move(other.array);
            capacity = other.capacity;
            size = other.size;
            hashFunction = std::move(other.hashFunction);
        }

        return *this;
    }

    HashTable& operator=(HashTable&& other) noexcept
    {
        if (this != &other)
        {
            array = std::move(other.array);
            capacity = other.capacity;
            size = other.size;
            hashFunction = std::move(other.hashFunction);
        }

        return *this;
    }

    ~HashTable() = default;

    int HashCode(const TKey& key) const
    {
        return hashFunction(key) % capacity;
    }

    void Add(const TKey& key, const TValue& value) override
    {
        if (size >= capacity * 0.7)
            Resize(capacity * 2);

        int hashIndex = HashCode(key);

        while (array[hashIndex].Get())
        {
            if (array[hashIndex].Get()->key == key)
            {
                array[hashIndex].Get()->value = value;
                return;
            }
            else
            {
                hashIndex = (hashIndex + 1) % capacity;
            }
        }

        array[hashIndex] = UniquePointer<HashNode<TKey, TValue>>(new HashNode<TKey, TValue>(key, value));
        size++;
    }

    void Remove(const TKey& key) override
    {
        int hashIndex = HashCode(key);

        while (array[hashIndex].Get())
        {
            if (array[hashIndex].Get()->key == key)
            {
                array[hashIndex].Reset();
                size--;

                int nextIndex = (hashIndex + 1) % capacity;
                while (array[nextIndex].Get())
                {
                    auto node = std::move(array[nextIndex]);
                    array[nextIndex].Reset();
                    size--;
                    int newIndex = HashCode(node->key);

                    while (array[newIndex].Get())
                        newIndex = (newIndex + 1) % capacity;

                    array[newIndex] = std::move(node);
                    size++;
                    nextIndex = (nextIndex + 1) % capacity;
                }

                if (size <= capacity / 4 && capacity > 20)
                    Resize(std::max(20, capacity / 2));

                return;
            }

            hashIndex = (hashIndex + 1) % capacity;
        }
    }


    std::optional<TValue> GetValue(const TKey& key) const override
    {
        int hashIndex = HashCode(key);

        while (array[hashIndex].Get())
        {
            if (array[hashIndex].Get()->key == key)
            {
                return array[hashIndex].Get()->value;
            }

            hashIndex = (hashIndex + 1) % capacity;
        }

        return std::nullopt;
    }

    bool ContainsKey(const TKey& key) const override
    {
        return GetValue(key).has_value();
    }

    int GetCount() const override
    {
        return size;
    }

    int GetCapacity() const override
    {
        return capacity;
    }

    bool IsEmpty() const
    {
        return size == 0;
    }

    bool ConstainsIndex(const int index) const
    {
        if (index < 0 || index > capacity)
        {
            return false;
        }

        return array[index].Get() != nullptr;
    }

    TKey& GetKeyByIndex(const int index) const
    {
        return array[index].Get()->key;
    }

    TValue& GetValueByIndex(const int index) const
    {
        return array[index].Get()->value;
    }
};
