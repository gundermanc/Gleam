#include "TextSegment.h"

void TextSegment::Render(std::shared_ptr<AbstractGraphicsContext> graphicsContext)
{
    graphicsContext->DrawText(
        this->GetText().GetValue(),
        this->GetForeground().GetValue(),
        this->GetFontSize().GetValue(),
        this->GetX().GetValue(),
        this->GetY().GetValue());
}

void TextSegment::Position(unsigned int maxWidth, unsigned int maxHeight)
{

}
