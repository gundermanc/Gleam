#include "DocumentView.h"

#include <fstream>
#include "../Framework/Controls/Border.h"
#include "../Framework/Controls/TextSegment.h"
#include "../Text/TextDocument.h"
#include "../TextBox/TextBox.h"

std::shared_ptr<TextDocument> CreateTextDocument(const std::string& filePath)
{
    auto document = new TextDocument();

    // Read and populate new lines.
    std::ifstream infile(filePath);
    std::string line;
    while (std::getline(infile, line))
    {
        document->AppendLine(line);
    }

    return std::shared_ptr<TextDocument>(document);
}

void ShowDocumentView(AbstractViewport& viewport, const std::string& filePath)
{
    // Clear all child controls from the current view.
    viewport.GetChildren().clear();

    viewport.GetBackground().SetValue(colors::Gray);

    auto document = CreateTextDocument(filePath);

    auto textBox = new TextBox(
        document,
        colors::Black,
        colors::Green,
        20,
        20,
        viewport.GetWidth().GetValue() - 40,
        viewport.GetHeight().GetValue() - 40);

    textBox->GetFontSize().SetValue(12);

    viewport.GetChildren().push_back(std::shared_ptr<AbstractControl>(textBox));

    // Add resize handler.
    viewport.GetWidth().SubscribeToChange([textBox](unsigned int oldValue, unsigned int newValue)
    {
        textBox->GetWidth().SetValue(newValue - 40);
    });
    viewport.GetHeight().SubscribeToChange([textBox](unsigned int oldValue, unsigned int newValue)
    {
        textBox->GetHeight().SetValue(newValue - 40);
    });
}
