#pragma once

#include <memory>

template <typename TValue>
class Property
{
public:
    Property() { }
    Property(TValue initialValue)
    {
        this->value = initialValue;
    }

    TValue GetValue() const { return this->value; }
    void SetValue(TValue value) { this->value = value; }

protected:
    TValue value;
};
