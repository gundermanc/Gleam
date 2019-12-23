#pragma once

#include <algorithm>
#include "../../Renderer/Framework/AbstractGraphicsContext.h"

namespace
{
    const unsigned int MaxCharacterSize = 10;
}

class MockGraphicsContext : public AbstractGraphicsContext
{

public:
    virtual std::tuple<unsigned int, unsigned int, size_t> ComputeTextDimensions(
        const std::string& text,
        unsigned int size,
        unsigned int maxWidth)
    {
        unsigned int fullSizeWidth = text.size() * MaxCharacterSize;
        unsigned int rightEdgeOfText = std::min(fullSizeWidth, maxWidth);
        size_t textLength = rightEdgeOfText / MaxCharacterSize;

        return std::tuple<unsigned int, unsigned int, size_t>(
            textLength * MaxCharacterSize,
            MaxCharacterSize, textLength);
    }

    virtual void DrawText(
        const std::string& text,
        const Color& color,
        unsigned int size,
        unsigned int x,
        unsigned int y)
    {
    }

    virtual void DrawRect(
        const Color& color,
        unsigned int x,
        unsigned int y,
        unsigned int width,
        unsigned int height)
    {
    }

    virtual void DrawRectOutline(
        const Color& color,
        unsigned int thickness,
        unsigned x,
        unsigned int y,
        unsigned int width,
        unsigned int height)
    {
    }
};
