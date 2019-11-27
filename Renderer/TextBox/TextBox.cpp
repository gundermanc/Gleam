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

    int y = this->GetY().GetValue();
    for (auto& line : this->textDocument->GetLines())
    {
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
