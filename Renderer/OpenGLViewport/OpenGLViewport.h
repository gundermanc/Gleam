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

private:
    // Store GLFWwindow* as void* to avoid leakage of OpenGL into caller.
    void* window = nullptr;

    std::shared_ptr<AbstractGraphicsContext> abstractGraphicsContext;
};
