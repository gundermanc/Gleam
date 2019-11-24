#pragma once

#include "AbstractControl.h"
#include "../AbstractGraphicsContext.h"
#include "../Property.h"

class TextSegment : public AbstractControl
{
public:
    TextSegment(
        const std::string& text,
        unsigned int fontSize,
        const Color& background,
        const Color& foreground,
        unsigned int x,
        unsigned int y,
        unsigned int width,
        unsigned int height)
        : fontSize(fontSize), text(text), AbstractControl(background, foreground, x, y, width, height) { }

    Property<unsigned int>& GetFontSize() { return this->fontSize; }
    Property<std::string>& GetText() { return this->text; }

    // Overrides.
    void Render(std::shared_ptr<AbstractGraphicsContext> graphicsContext);
    void Position(unsigned int maxWidth, unsigned int maxHeight);

private:
    Property<unsigned int> fontSize;
    Property<std::string> text;
};
