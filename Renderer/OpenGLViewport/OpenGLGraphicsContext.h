#pragma once

#include <string>
#include "../Framework/AbstractGraphicsContext.h"

class OpenGLGraphicsContext : public AbstractGraphicsContext
{
public:
    OpenGLGraphicsContext();
    ~OpenGLGraphicsContext();

    void FinalizeDrawing();
    void Reshape(unsigned int width, unsigned int height);

    // Overrides.
    void DrawText(std::string& text, unsigned int x, unsigned int y);
};
