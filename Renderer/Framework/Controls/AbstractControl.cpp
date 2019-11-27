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

void AbstractControl::Scroll(int x, int y, int scrollX, int scrollY)
{
    // Find control under the cursor.
    for (auto& control : this->GetChildren())
    {
        auto controlX0 = control->GetX().GetValue();
        auto controlY0 = control->GetY().GetValue();
        auto controlX1 = control->GetWidth().GetValue() + controlX0;
        auto controlY1 = control->GetHeight().GetValue() + controlY0;

        // Does current control contain this point?
        if (x >= controlX0 &&
            x <= controlX1 &&
            y >= controlY0 &&
            y <= controlY1)
        {
            // Yes, dispatch to child control.
            control->Scroll(x - controlX0, y - controlY0, scrollX, scrollY);
        }
    }
}
