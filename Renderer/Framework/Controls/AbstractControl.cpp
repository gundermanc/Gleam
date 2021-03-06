#include "AbstractControl.h"

void AbstractControl::Render(std::shared_ptr<AbstractGraphicsContext> graphicsContext)
{
    // Check if we need to relayout.
    if (this->GetIsPositionInvalid().GetValue())
    {
        this->Position(
            graphicsContext,
            this->GetWidth().GetValue(),
            this->GetHeight().GetValue());

        this->GetIsPositionInvalid().SetValue(true);
    }

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

// TODO: we need some concept of focus eventually instead of just sending to every control.
bool AbstractControl::SendKey(Key key, KeyAction action, char character)
{
    // Find control that handles this key press.
    for (auto& control : this->GetChildren())
    {
        if (control->SendKey(key, action, character))
        {
            return true;
        }
    }

    return false;
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
