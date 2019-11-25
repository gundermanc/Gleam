#include "OpenGLViewport/OpenGLViewport.h"
#include "Framework/Controls/TextSegment.h"

/* program entry */
int main(int argc, char *argv[])
{
    // Create viewport.
    OpenGLViewport viewport("Gleam - hello world", 700, 500);

    // Layout UX.
    auto segment0 = new TextSegment(std::string("Hello there"), 10, colors::Red, colors::Black, 0, 0, 100, 20);
    auto segmentPointer0 = std::shared_ptr<AbstractControl>(segment0);
    segment0->GetTextAlignment().SetValue(AlignmentMode::Left);


    auto segment1 = new TextSegment(std::string("This is an.."), 20, colors::Blue, colors::White, 20, 20, 200, 30);
    auto segmentPointer1 = std::shared_ptr<AbstractControl>(segment1);
    segment1->GetTextAlignment().SetValue(AlignmentMode::Right);


    auto segment2 = new TextSegment(std::string("Untitled Christian Gunderman project"), 30, colors::Green, colors::Black, 30, 50, 700, 40);
    auto segmentPointer2 = std::shared_ptr<AbstractControl>(segment2);
    segment2->GetTextAlignment().SetValue(AlignmentMode::Center);


    viewport.GetChildren().push_back(segmentPointer0);
    viewport.GetChildren().push_back(segmentPointer1);
    viewport.GetChildren().push_back(segmentPointer2);

    // Create window and start handling events.
    viewport.BeginEventLoop();

    return 0;
}
