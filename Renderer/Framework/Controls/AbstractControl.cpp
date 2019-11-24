#include "AbstractControl.h"

void AbstractControl::Render(std::shared_ptr<AbstractGraphicsContext> graphicsContext)
{
    for (std::shared_ptr<AbstractControl> control : this->GetChildren())
    {
        control->Render(graphicsContext);
    }
}
