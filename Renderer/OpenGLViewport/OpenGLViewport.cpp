#include "OpenGLViewport.h"
#include "../Text/mat4.h"

#include <gl/glew.h>
#include <freetype-gl/freetype-gl.h>
#include <freetype-gl/vertex-buffer.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
using namespace ftgl;

// Windows Macro obnoxiously collides with function name here..
#ifdef DrawText
#undef DrawText
#endif // DrawText

namespace
{
    // Unavoidable (I think) globals 🤮
    OpenGLViewport* viewportSingleton = nullptr;

    void reshape(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        viewportSingleton->Reshape(width, height);
    }

    void key(GLFWwindow* window, int k, int s, int action, int mods)
    {

    }
}

OpenGLViewport::OpenGLViewport(std::string title, int width, unsigned int height)
    : AbstractViewport(title, std::shared_ptr<AbstractGraphicsContext>(), width, height)
{
    if (!glfwInit())
    {
        // TODO: logging and exit strategy?
        fprintf(stderr, "Failed to initialize GLFW\n" );
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_DEPTH_BITS, 16);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

    auto window = glfwCreateWindow(width, height, this->GetTitle().GetValue().c_str(), NULL, NULL);
    this->window = window;
    if (!window)
    {
        fprintf(stderr, "Failed to open GLFW window\n" );
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Set callback functions
    glfwSetFramebufferSizeCallback(window, reshape);
    glfwSetKeyCallback(window, key);

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    int fbWidth = 0;
    int fbHeight = 0;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

    glewInit();

    // Parse command-line options
    this->abstractGraphicsContext = std::shared_ptr<AbstractGraphicsContext>(new OpenGLGraphicsContext());
    viewportSingleton = this;

    reshape(window, fbWidth, fbHeight);
}

void OpenGLViewport::BeginEventLoop()
{
    auto window = static_cast<GLFWwindow*>(this->window);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        Draw();

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // TODO: delete graphics context?

    // Terminate GLFW
    glfwTerminate();
}

void OpenGLViewport::Draw()
{
    auto background = this->GetBackground().GetValue();

    // Instruct graphics context to draw any pending textures:
    auto graphicsContext = std::static_pointer_cast<OpenGLGraphicsContext>(this->abstractGraphicsContext);

    // Compute control positions, if needed.
    this->Position(graphicsContext, this->GetWidth().GetValue(), this->GetHeight().GetValue());

    // Re-render.
    graphicsContext->BeginDrawing();
    this->Render(this->abstractGraphicsContext);
    graphicsContext->FinalizeDrawing();
}

void OpenGLViewport::Reshape(unsigned int width, unsigned int height)
{
    this->GetWidth().SetValue(width);
    this->GetHeight().SetValue(height);

    // Instruct graphics context that we're changing dimensions.
    std::static_pointer_cast<OpenGLGraphicsContext>(this->abstractGraphicsContext)->Reshape(width, height);
}
