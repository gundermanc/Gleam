#pragma once

#include <functional>
#include <memory>
#include <vector>

template <typename TValue>
class Property
{
public:
    Property() { }
    Property(TValue initialValue)
    {
        this->value = initialValue;
    }

    void SubscribeToChange(std::function<void(TValue, TValue)> listener) { this->changeSubscribers.push_back(listener); }
    const std::vector<std::function<void(TValue, TValue)>>& GetChangeSubscribers() { return this->changeSubscribers; };
    TValue GetValue() const { return this->value; }
    void SetValue(TValue value)
    {
        auto oldValue = this->GetValue();

        this->value = value;

        // Alert our subscribers.
        for (auto subscriber : this->GetChangeSubscribers())
        {
            subscriber(oldValue, value);
        }
    }

protected:
    TValue value;
    std::vector<std::function<void(TValue, TValue)>> changeSubscribers;
};
