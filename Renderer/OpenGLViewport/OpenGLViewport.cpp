#include "OpenGLViewport.h"
#include "Text/mat4.h"

#include <gl/glew.h>
#include <freetype-gl/freetype-gl.h>
#include <freetype-gl/vertex-buffer.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
using namespace ftgl;

namespace
{
    // Unavoidable (I think) global 🤮
    OpenGLViewport* viewportSingleton = nullptr;

    void character(GLFWwindow* window, unsigned int character)
    {
        viewportSingleton->SendKey(Key::None, KeyAction::TypeCharacter, character);
    }

    void key(GLFWwindow* window, int key, int scanCode, int action, int mods)
    {
        // We can cast the GLFW key to our framework key because
        // they are the same, by convention.
        viewportSingleton->SendKey((Key)key, (KeyAction)action, '/0');
    }

    void reshape(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        viewportSingleton->Reshape(width, height);
    }

    void scroll(GLFWwindow* window, double scrollX, double scrollY)
    {
        double x;
        double y;
        glfwGetCursorPos(window, &x, &y);

        viewportSingleton->Scroll(x, y, scrollX, scrollY);
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
    glfwSetCharCallback(window, character);
    glfwSetKeyCallback(window, key);
    glfwSetScrollCallback(window, scroll);

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
    // Perform initial layout.
    this->Reshape(
        this->GetWidth().GetValue(),
        this->GetHeight().GetValue());

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        Draw();

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();
}

void OpenGLViewport::Draw()
{
    auto background = this->GetBackground().GetValue();

    // Instruct graphics context to draw any pending textures:
    auto graphicsContext = std::static_pointer_cast<OpenGLGraphicsContext>(this->abstractGraphicsContext);

    // Re-render.
    graphicsContext->BeginDrawing();
    this->Render(this->abstractGraphicsContext);
    graphicsContext->FinalizeDrawing();
}

void OpenGLViewport::Reshape(unsigned int width, unsigned int height)
{
    this->GetWidth().SetValue(width);
    this->GetHeight().SetValue(height);

    auto graphicsContext = std::static_pointer_cast<OpenGLGraphicsContext>(this->abstractGraphicsContext);

    // Instruct graphics context that we're changing dimensions.
    graphicsContext->Reshape(width, height);
    this->Position(graphicsContext, width, height);
}
