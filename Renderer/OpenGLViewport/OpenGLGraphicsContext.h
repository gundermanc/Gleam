#pragma once

#include <string>
#include "../Framework/AbstractGraphicsContext.h"
#include "../Framework/Color.h"

class OpenGLGraphicsContext : public AbstractGraphicsContext
{
public:
    OpenGLGraphicsContext();
    ~OpenGLGraphicsContext();

    void BeginDrawing();
    void FinalizeDrawing();
    void Reshape(unsigned int width, unsigned int height);

    // Overrides.
    std::tuple<unsigned int, unsigned int> ComputeTextDimensions(const std::string& text, unsigned int size);

    void DrawText(const std::string& text, const Color& color, unsigned int size, unsigned int x, unsigned int y);

    void DrawRect(const Color& color, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
};
