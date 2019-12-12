#pragma once

#include <functional>
#include <memory>
#include <vector>

template <typename TValue>
class Property
{
public:
    Property() { }

    Property(TValue initialValue) : Property(initialValue, [](TValue newValue) { return true; })
    {
    }

    Property(TValue initialValue, std::function<bool(TValue)> isValidValueFunc)
        : value(initialValue), isValidValueFunc(isValidValueFunc)
    {
    }

    void SubscribeToChange(std::function<void(TValue, TValue)> listener) { this->changeSubscribers.push_back(listener); }
    const std::vector<std::function<void(TValue, TValue)>>& GetChangeSubscribers() { return this->changeSubscribers; };
    TValue GetValue() const { return this->value; }
    void SetValue(TValue value)
    {
        // Ensure value is valid. If the function returns false, don't update the value.
        // The validator can also optionally fail fast or throw if they want to.
        if (!this->isValidValueFunc(value))
        {
            return;
        }

        // Update the value.
        auto oldValue = this->GetValue();

        this->value = value;

        // Alert our subscribers.
        for (auto subscriber : this->GetChangeSubscribers())
        {
            subscriber(oldValue, value);
        }
    }

protected:
    const std::function<bool(TValue)> isValidValueFunc;
    std::vector<std::function<void(TValue, TValue)>> changeSubscribers;
    TValue value;
};
