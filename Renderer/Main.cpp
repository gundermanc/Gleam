#include "OpenGLViewport/OpenGLViewport.h"
#include "Framework/Controls/TextSegment.h"

/* program entry */
int main(int argc, char *argv[])
{
    // Create viewport.
    OpenGLViewport viewport("Gleam - hello world", 700, 500);

    // Layout UX.
    auto segment0 = std::shared_ptr<AbstractControl>(
        new TextSegment(std::string("Hello there"), 10, colors::Red, colors::Black, 0, 0, 100, 20));

    auto segment1 = std::shared_ptr<AbstractControl>(
        new TextSegment(std::string("This is an.."), 20, colors::Blue, colors::White, 20, 20, 200, 30));

    auto segment2 = std::shared_ptr<AbstractControl>(
        new TextSegment(std::string("Untitled Christian Gunderman project"), 30, colors::Green, colors::Black, 30, 50, 700, 40));

    viewport.GetChildren().push_back(segment0);
    viewport.GetChildren().push_back(segment1);
    viewport.GetChildren().push_back(segment2);

    // Create window and start handling events.
    viewport.BeginEventLoop();

    return 0;
}
