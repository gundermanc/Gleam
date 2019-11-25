#include "AbstractControl.h"

void AbstractControl::Render(std::shared_ptr<AbstractGraphicsContext> graphicsContext)
{
    // Shade control background.
    graphicsContext->DrawRect(
        this->GetBackground().GetValue(),
        this->GetX().GetValue(),
        this->GetY().GetValue(),
        this->GetWidth().GetValue(),
        this->GetHeight().GetValue());

    // Draw border.
    graphicsContext->DrawRectOutline(
        this->GetBorderColor().GetValue(),
        this->GetBorderThickness().GetValue(),
        this->GetX().GetValue(),
        this->GetY().GetValue(),
        this->GetWidth().GetValue(),
        this->GetHeight().GetValue());

    for (std::shared_ptr<AbstractControl> control : this->GetChildren())
    {
        control->Render(graphicsContext);
    }
}

void AbstractControl::Position(
    std::shared_ptr<AbstractGraphicsContext> graphicsContext,
    unsigned int maxWidth,
    unsigned int maxHeight)
{
    for (std::shared_ptr<AbstractControl> control : this->GetChildren())
    {
        control->Position(graphicsContext, this->GetWidth().GetValue(), this->GetHeight().GetValue());
    }
}
