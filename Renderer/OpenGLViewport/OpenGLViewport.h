#pragma once

#include <memory>

#include "../Framework/AbstractViewport.h"
#include "OpenGLGraphicsContext.h"

class OpenGLViewport : public AbstractViewport
{
public:
    OpenGLViewport(std::string title, int width, unsigned int height);

    void BeginEventLoop();
    void Draw();
    void Reshape(unsigned int width, unsigned int height);

protected:
    void Render(std::shared_ptr<AbstractGraphicsContext> graphicsContext);
    void Position(unsigned int maxWidth, unsigned int maxHeight);

private:
    // Store GLFWwindow* as void* to avoid leakage of OpenGL into caller.
    void* window = nullptr;

    std::shared_ptr<AbstractGraphicsContext> abstractGraphicsContext;
};
