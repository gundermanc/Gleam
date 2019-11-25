#include "TextSegment.h"

void TextSegment::Render(std::shared_ptr<AbstractGraphicsContext> graphicsContext)
{
    AbstractControl::Render(graphicsContext);

    graphicsContext->DrawText(
        this->GetText().GetValue(),
        this->GetForeground().GetValue(),
        this->GetFontSize().GetValue(),
        this->GetX().GetValue() + this->textXOffset,
        this->GetY().GetValue() + this->textYOffset);
}

void TextSegment::Position(
    std::shared_ptr<AbstractGraphicsContext> graphicsContext,
    unsigned int maxWidth,
    unsigned int maxHeight)
{
    switch (this->GetTextAlignment().GetValue())
    {
    case AlignmentMode::Left:
        this->textXOffset = 0;
        this->textYOffset = 0;
        return;
    case AlignmentMode::Right:
    {
        auto dimensions = graphicsContext->ComputeTextDimensions(this->GetText().GetValue(), this->GetFontSize().GetValue());
        this->textXOffset = this->GetWidth().GetValue() - std::get<0>(dimensions);
        return;
    }
    case AlignmentMode::Center:
    {
        auto dimensions = graphicsContext->ComputeTextDimensions(this->GetText().GetValue(), this->GetFontSize().GetValue());
        this->textXOffset = (this->GetWidth().GetValue() - std::get<0>(dimensions)) / 2.0;
        return;
    }
    }
}
