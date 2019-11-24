#pragma once

#include <string>
#include "../AbstractControl.h"
#include "../Property.h"

class TextSegment : public AbstractControl
{
public:
    TextSegment(std::string text, Color background, Color foreground, unsigned int width, unsigned int height)
        : AbstractControl(background, foreground, width, height), text(text) { }

    Property<std::string>& GetText() { return this->text; }

private:
    Property<std::string> text;
};
