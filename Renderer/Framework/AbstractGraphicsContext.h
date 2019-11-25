#pragma once

#include <string>
#include <tuple>
#include "Color.h"

class AbstractGraphicsContext
{
public:
    virtual std::tuple<unsigned int, unsigned int, size_t> ComputeTextDimensions(
        const std::string& text,
        unsigned int size,
        unsigned int maxWidth) = 0;
    virtual void DrawText(const std::string& text, const Color& color, unsigned int size, unsigned int x, unsigned int y) = 0;
    virtual void DrawRect(const Color& color, unsigned int x, unsigned int y, unsigned int width, unsigned int height) = 0;
    virtual void DrawRectOutline(const Color& color, unsigned int thickness, unsigned x, unsigned int y, unsigned int width, unsigned int height) = 0;
};
