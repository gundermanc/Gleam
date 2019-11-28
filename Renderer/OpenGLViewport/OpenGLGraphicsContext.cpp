#include "OpenGLGraphicsContext.h"

#include <vector>
#include <gl/glew.h>

#include "Text/OpenGLTextContext.h"

namespace
{
    void MapAbsoluteCoordinatesToUnitOrtho(float& x, float& y, float& width, float& height)
    {
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        GLint viewportWidth = viewport[2];
        GLint viewportHeight = viewport[3];

        // I'm sure there's a more OpenGL-y way to do this but this is effective for now.
        x = (((float)x / viewportWidth) * 2.0) - 1.0;
        y = -((((float)y / viewportHeight) * 2.0) - 1.0);
        width = ((float)width / viewportWidth) * 2.0;
        height = ((float)height / viewportHeight) * 2.0;
    }
}

OpenGLGraphicsContext::OpenGLGraphicsContext() : textContext(new OpenGLTextContext())
{
}

void OpenGLGraphicsContext::BeginDrawing()
{
    this->textContext->Clear();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLGraphicsContext::FinalizeDrawing()
{
    this->textContext->Draw();
}

void OpenGLGraphicsContext::Reshape(unsigned int width, unsigned int height)
{
    this->textContext->Reshape(width, height);
}

// Returns width, height, and number of characters consumed before hitting maxWidth or maxHeight.
std::tuple<unsigned int, unsigned int, size_t> OpenGLGraphicsContext::ComputeTextDimensions(
    const std::string& text,
    unsigned int size,
    unsigned int maxWidth)
{
    return this->textContext->ComputeTextDimensions(text, size, maxWidth);
}

void OpenGLGraphicsContext::DrawText(const std::string& text, const Color& color, unsigned int size, unsigned int x, unsigned int y)
{
    this->textContext->AddText(text, size, color, x, y);
}

void OpenGLGraphicsContext::DrawRect(const Color& color, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
    float adjustedX = x;
    float adjustedY = y;
    float adjustedWidth = width;
    float adjustedHeight = height;
    MapAbsoluteCoordinatesToUnitOrtho(adjustedX, adjustedY, adjustedWidth, adjustedHeight);

    glColor4f(color.Red, color.Green, color.Blue, color.Alpha);
    glRectf(adjustedX, adjustedY, adjustedX + adjustedWidth, adjustedY - adjustedHeight);
}

void OpenGLGraphicsContext::DrawRectOutline(const Color& color, unsigned int thickness, unsigned x, unsigned int y, unsigned int width, unsigned int height)
{
    if (thickness == 0)
    {
        return;
    }

    float adjustedX = x;
    float adjustedY = y;
    float adjustedWidth = width;
    float adjustedHeight = height;
    MapAbsoluteCoordinatesToUnitOrtho(adjustedX, adjustedY, adjustedWidth, adjustedHeight);

    glLineWidth(thickness);
    glColor4f(color.Red, color.Green, color.Blue, color.Alpha);
    glBegin(GL_LINE_LOOP);
        glVertex2f(adjustedX, adjustedY);
        glVertex2f(adjustedX + adjustedWidth, adjustedY);
        glVertex2f(adjustedX + adjustedWidth, adjustedY - adjustedHeight);
        glVertex2f(adjustedX, adjustedY - adjustedHeight);
    glEnd();
}
