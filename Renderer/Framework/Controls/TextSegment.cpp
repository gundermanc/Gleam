#include "TextSegment.h"

void TextSegment::Render(std::shared_ptr<AbstractGraphicsContext> graphicsContext)
{
    AbstractControl::Render(graphicsContext);

    // Write out split lines.
    size_t start = 0;
    size_t verticalOffset = 0;
    for (size_t index : this->textClippingIndicies)
    {
        // Get subspan for the current broken line.
        auto text = this->GetText().GetValue().substr(start, index);

        // Append ellipsis if needed.
        if (index < this->GetText().GetValue().length() &&
            this->GetTextClippingMode().GetValue() == TextClippingMode::Ellipsis)
        {
            text.append("...");
        }

        // Draw line.
        graphicsContext->DrawText(
            text,
            this->GetForeground().GetValue(),
            this->GetFontSize().GetValue(),
            this->GetX().GetValue() + this->textXOffset,
            this->GetY().GetValue() + this->textYOffset + verticalOffset);

        // Advance to the next line.
        start = index + 1;
        verticalOffset += this->wrappingFontHeight;
    }
}

void TextSegment::ComputeTextClipping(std::shared_ptr<AbstractGraphicsContext> graphicsContext)
{
    std::tuple<unsigned int, unsigned int, size_t> dimensions;
    std::string text = this->GetText().GetValue();
    unsigned int y = 0;

    // Clear any previous line breaks and find the new ones.
    this->textClippingIndicies.clear();
    do
    {
        // Check if there's enough space for the current text..
        dimensions = graphicsContext->ComputeTextDimensions(
            text,
            this->GetFontSize().GetValue(),
            this->GetWidth().GetValue());

        // In ellipsis mode, allocate space for the dots.
        size_t consumed = std::get<2>(dimensions);
        if (consumed < this->GetText().GetValue().length() &&
            this->GetTextClippingMode().GetValue() == TextClippingMode::Ellipsis)
        {
            consumed -= 3;
        }

        // Break if we run out of vertical space.
        this->wrappingFontHeight = std::get<1>(dimensions);
        if ((y + this->wrappingFontHeight) >= this->GetHeight().GetValue())
        {
            break;
        }

        // Store the index of the place where we ran out of room.
        this->textClippingIndicies.push_back(consumed);
        y += this->wrappingFontHeight;

        // In clip mode, we only care about the first one.
        if (this->GetTextClippingMode().GetValue() == TextClippingMode::Clip)
        {
            break;
        }

        // Advance.
        text = text.substr(consumed, text.length() - consumed);
    } while (text.length() > 0);
}

void TextSegment::ComputeTextAlignment(std::shared_ptr<AbstractGraphicsContext> graphicsContext)
{
    switch (this->GetTextAlignment().GetValue())
    {
    case AlignmentMode::Left:
        this->textXOffset = 0;
        this->textYOffset = 0;
        return;
    case AlignmentMode::Right:
    {
        auto dimensions = graphicsContext->ComputeTextDimensions(
            this->GetText().GetValue(),
            this->GetFontSize().GetValue(),
            this->GetWidth().GetValue());
        this->textXOffset = this->GetWidth().GetValue() - std::get<0>(dimensions);
        return;
    }
    case AlignmentMode::Center:
    {
        auto dimensions = graphicsContext->ComputeTextDimensions(
            this->GetText().GetValue(),
            this->GetFontSize().GetValue(),
            this->GetWidth().GetValue());
        this->textXOffset = (this->GetWidth().GetValue() - std::get<0>(dimensions)) / 2.0;
        return;
    }
    }
}

void TextSegment::Position(
    std::shared_ptr<AbstractGraphicsContext> graphicsContext,
    unsigned int maxWidth,
    unsigned int maxHeight)
{
    this->ComputeTextClipping(graphicsContext);
    this->ComputeTextAlignment(graphicsContext);
}
