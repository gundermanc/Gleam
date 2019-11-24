#pragma once

class Color
{
public:
    Color(float red, float green, float blue, float alpha)
        : Red(red), Green(green), Blue(blue), Alpha(alpha) { }

    Color() : Color(0.0, 0.0, 0.0, 1.0) { /* default constructor is black */ }

    float Red;
    float Green;
    float Blue;
    float Alpha;
};

namespace colors
{
    const Color Black(0.0, 0.0, 0.0, 1.0);
    const Color White(1.0, 1.0, 1.0, 1.0);
}
