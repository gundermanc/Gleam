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
    const Color Transparent(0.0, 0.0, 0.0, 0.0);
    const Color Black(0.0, 0.0, 0.0, 1.0);
    const Color Gray(0.5, 0.5, 0.5, 1.0);
    const Color Red(1.0, 0.0, 0.0, 1.0);
    const Color Green(0.0, 1.0, 0.0, 1.0);
    const Color Blue(0.0, 0.0, 1.0, 1.0);
    const Color White(1.0, 1.0, 1.0, 1.0);
}
