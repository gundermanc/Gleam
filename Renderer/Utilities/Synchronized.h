#pragma once

#include <functional>
#include <mutex>

template <typename TValue>
class Synchronized
{
public:
    Synchronized(TValue value) : value(value) { }

    void Access(std::function<void(TValue&)> action)
    {
        std::lock_guard<std::mutex>(this->mutex);
        action(this->value);
    }

private:
    TValue value;
    std::mutex mutex;
};
