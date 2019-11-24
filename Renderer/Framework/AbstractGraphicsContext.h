#pragma once

#include <string>
#include "Color.h"

class AbstractGraphicsContext
{
public:
    virtual void DrawText(const std::string& text, const Color& color, unsigned int size, unsigned int x, unsigned int y) = 0;
    virtual void DrawRect(const Color& color, unsigned int x, unsigned int y, unsigned int width, unsigned int height) = 0;
};
