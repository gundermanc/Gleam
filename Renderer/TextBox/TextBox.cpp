#include "TextBox.h"

void TextBox::Position(
    std::shared_ptr<AbstractGraphicsContext> graphicsContext,
    unsigned int maxWidth,
    unsigned int maxHeight)
{
    // TODO: incrementally.
    this->GetChildren().clear();

    auto fontSize = this->GetFontSize().GetValue();
    auto dimensions = graphicsContext->ComputeTextDimensions("", fontSize, this->GetWidth().GetValue());

    auto lineHeight = std::get<1>(dimensions);

    auto lines = this->textDocument->GetLines();
    auto initialLine = this->GetScrollLine().GetValue();

    int y = this->GetY().GetValue();
    for (size_t lineNumber = initialLine; lineNumber < lines.size(); lineNumber++)
    {
        const std::string& line = lines[lineNumber];

        // Draw only the visible portion.
        if (y >= this->GetHeight().GetValue())
        {
            break;
        }

        // TODO: position relative to parent?
        auto textSegment = new TextSegment(
            line,
            fontSize,
            colors::Transparent,
            this->GetForeground().GetValue(),
            this->GetX().GetValue(),
            y,
            this->GetWidth().GetValue(),
            lineHeight);

        this->GetChildren().push_back(std::shared_ptr<AbstractControl>(textSegment));
        y += lineHeight;
    }

    AbstractControl::Position(
        graphicsContext,
        maxWidth,
        maxHeight);
}

void TextBox::Scroll(int x, int y, int scrollX, int scrollY)
{
    auto& scrollLineProperty = this->GetScrollLine();
    auto scrollLinePropertyValue = scrollLineProperty.GetValue();
    int updatedScrollLinePropertyValue = scrollLinePropertyValue - (scrollY * this->GetScrollRate().GetValue());
    auto totalLines = this->textDocument->GetLines().size();

    // Ensure updatedScrollLinePropertyValue is within bounds.
    if (updatedScrollLinePropertyValue < 0)
    {
        updatedScrollLinePropertyValue = 0;
    }
    else if (updatedScrollLinePropertyValue >= totalLines)
    {
        updatedScrollLinePropertyValue = totalLines - 1;
    }


    scrollLineProperty.SetValue(updatedScrollLinePropertyValue);
}
