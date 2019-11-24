#pragma once

#include "Property.h"

class Color
{
public:
    Color(float red, float green, float blue, float alpha)
        : Red(red), Green(green), Blue(blue), Alpha(alpha) { }

    Color() : Color(0.0, 0.0, 0.0, 1.0) { /* default constructor is black*/ }

    Color(const Color& original) : Red(original.Red), Green(original.Green), Blue(original.Blue), Alpha(original.Alpha) { }

    Color& operator=(const Color& otherColor) { return *this; }

    const Property<float> Red;
    const Property<float> Green;
    const Property<float> Blue;
    const Property<float> Alpha;
};

namespace colors
{
    const Color Black(0.0, 0.0, 0.0, 1.0);
    const Color White(1.0, 1.0, 1.0, 1.0);
}
