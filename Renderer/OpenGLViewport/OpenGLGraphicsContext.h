#pragma once

#include <string>
#include "../Framework/AbstractGraphicsContext.h"
#include "../Framework/Color.h"

class OpenGLGraphicsContext : public AbstractGraphicsContext
{
public:
    OpenGLGraphicsContext();
    ~OpenGLGraphicsContext();

    void FinalizeDrawing();
    void Reshape(unsigned int width, unsigned int height);

    // Overrides.
    void DrawText(const std::string& text, const Color& color, unsigned int x, unsigned int y);
};
