#pragma once

#include "AbstractControl.h"

class Border : public AbstractControl
{
public:
    Border(Color background, Color borderColor, unsigned int borderThickness, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
        : AbstractControl(background, colors::Black, x, y, width, height)
    {
        this->GetBorderColor().SetValue(borderColor);
        this->GetBorderThickness().SetValue(borderThickness);
    }
};
