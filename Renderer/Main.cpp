#include "OpenGLViewport/OpenGLViewport.h"
#include "Framework/Controls/TextSegment.h"

/* program entry */
int main(int argc, char *argv[])
{
    // Create viewport.
    OpenGLViewport viewport("Gleam - hello world", 700, 500);
    viewport.GetBackground().SetValue(colors::Gray);

    // Layout UX.
    auto segment0 = new TextSegment(std::string("\"Hello there\"   --Obi Wan Kenobi"), 10, colors::Red, colors::Black, 0, 0, 100, 20);
    auto segmentPointer0 = std::shared_ptr<AbstractControl>(segment0);
    segment0->GetTextAlignment().SetValue(AlignmentMode::Left);
    segment0->GetTextClippingMode().SetValue(TextClippingMode::Ellipsis);
    segment0->GetBorderColor().SetValue(colors::Green);
    segment0->GetBorderThickness().SetValue(1);


    auto segment1 = new TextSegment(std::string("This is an.."), 20, colors::Blue, colors::White, 20, 30, 200, 40);
    auto segmentPointer1 = std::shared_ptr<AbstractControl>(segment1);
    segment1->GetTextAlignment().SetValue(AlignmentMode::Right);


    auto segment2 = new TextSegment(std::string("Untitled Christian Gunderman project"), 30, colors::Green, colors::Black, 30, 90, 500, 200);
    auto segmentPointer2 = std::shared_ptr<AbstractControl>(segment2);
    segment2->GetTextAlignment().SetValue(AlignmentMode::Center);
    segment2->GetTextClippingMode().SetValue(TextClippingMode::Wrap);
    segment2->GetBorderColor().SetValue(colors::White);
    segment2->GetBorderThickness().SetValue(3);


    viewport.GetChildren().push_back(segmentPointer0);
    viewport.GetChildren().push_back(segmentPointer1);
    viewport.GetChildren().push_back(segmentPointer2);

    // Create window and start handling events.
    viewport.BeginEventLoop();

    return 0;
}
