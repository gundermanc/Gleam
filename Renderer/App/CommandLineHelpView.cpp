#include "CommandLineHelpView.h"

#include "../Framework/Controls/TextSegment.h"

const char* CommandLineHelpText =
"Usage: gleam [filename]\n"

"Call with a file name to a open a document for viewing.";

void ShowCommandLineHelpView(AbstractViewport& viewport)
{
    // Clear all child controls from the current view.
    viewport.GetChildren().clear();

    viewport.GetBackground().SetValue(colors::Gray);

    // Add title section --------------------------------
    auto nameTextSegment = new TextSegment(
        std::string("Untitled Christian Gunderman project"),
        30,
        colors::Black,
        colors::Green,
        30,
        30,
        viewport.GetWidth().GetValue() - 60,
        50);
    nameTextSegment->GetTextAlignment().SetValue(AlignmentMode::Center);
    nameTextSegment->GetBorderColor().SetValue(colors::Black);
    nameTextSegment->GetBorderThickness().SetValue(2);

    // Add resize handler.
    viewport.GetWidth().SubscribeToChange([nameTextSegment](unsigned int oldValue, unsigned int newValue)
    {
        nameTextSegment->GetWidth().SetValue(newValue - 60);
    });

    viewport.GetChildren().push_back(std::shared_ptr<AbstractControl>(nameTextSegment));

    // Add help text section --------------------------------
    auto helpTextSegment = new TextSegment(
        std::string(CommandLineHelpText),
        20,
        colors::Black,
        colors::Green,
        30,
        110,
        viewport.GetWidth().GetValue() - 60,
        viewport.GetHeight().GetValue() - 110 - 30);
    helpTextSegment->GetTextAlignment().SetValue(AlignmentMode::Left);
    helpTextSegment->GetTextClippingMode().SetValue(TextClippingMode::Wrap);
    helpTextSegment->GetBorderColor().SetValue(colors::Black);
    helpTextSegment->GetBorderThickness().SetValue(2);

    // Add resize handler.
    viewport.GetWidth().SubscribeToChange([helpTextSegment](unsigned int oldValue, unsigned int newValue)
    {
        helpTextSegment->GetWidth().SetValue(newValue - 60);
    });
    viewport.GetHeight().SubscribeToChange([helpTextSegment](unsigned int oldValue, unsigned int newValue)
    {
        helpTextSegment->GetHeight().SetValue(newValue - 110 - 30);
    });

    viewport.GetChildren().push_back(std::shared_ptr<AbstractControl>(helpTextSegment));
}
