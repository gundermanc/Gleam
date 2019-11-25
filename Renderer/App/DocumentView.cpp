#include "DocumentView.h"

#include <fstream>
#include "../Framework/Controls/Border.h"
#include "../Framework/Controls/TextSegment.h"

void ShowDocumentView(AbstractViewport& viewport, const std::string& filePath)
{
    // Clear all child controls from the current view.
    viewport.GetChildren().clear();

    viewport.GetBackground().SetValue(colors::Gray);

    // Document view border --------------------------------
    auto documentBorder = new Border(
        colors::White,
        colors::Black,
        2,
        15,
        15,
        viewport.GetWidth().GetValue() - 30,
        viewport.GetHeight().GetValue() - 30);

    // Add resize handler.
    viewport.GetWidth().SubscribeToChange([documentBorder](unsigned int oldValue, unsigned int newValue)
    {
        documentBorder->GetWidth().SetValue(newValue - 30);
    });
    viewport.GetHeight().SubscribeToChange([documentBorder](unsigned int oldValue, unsigned int newValue)
    {
        documentBorder->GetHeight().SetValue(newValue - 30);
    });

    auto border = std::shared_ptr<AbstractControl>(documentBorder);

    // Clear old lines.
    viewport.GetChildren().clear();

    // Add the border.
    viewport.GetChildren().push_back(border);

    // Read and populate new lines.
    std::ifstream infile(filePath);
    std::string line;
    int y = 20;
    while (std::getline(infile, line) && y < (viewport.GetHeight().GetValue() - 40))
    {
        auto currentLine = new TextSegment(
            line,
            12,
            colors::White,
            colors::Black,
            20,
            y,
            viewport.GetWidth().GetValue() - 40,
            20);

        y += 20;

        viewport.GetChildren().push_back(std::shared_ptr<AbstractControl>(currentLine));
    }
}
