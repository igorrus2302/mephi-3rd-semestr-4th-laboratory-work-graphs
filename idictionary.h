#pragma once

#include <optional>



template <typename TKey, typename TValue>
class IDictionary {

public:

    virtual ~IDictionary() = default;

    virtual int GetCount() const = 0;
    virtual int GetCapacity() const = 0;

    virtual std::optional<TValue> GetValue(const TKey& key) const = 0;
    virtual bool ContainsKey(const TKey& key) const = 0;
    virtual void Add(const TKey& key, const TValue& element) = 0;
    virtual void Remove(const TKey& key) = 0;
};
