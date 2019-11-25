#pragma once

#include "AbstractControl.h"
#include "../AbstractGraphicsContext.h"
#include "../Property.h"

enum class AlignmentMode
{
    Left,
    Right,
    Center
};

enum class TextClippingMode
{
    Clip,
    Ellipsis,
    Wrap
};

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
        : fontSize(fontSize),
        text(text),
        textAlignment(AlignmentMode::Left),
        textClippingMode(TextClippingMode::Ellipsis),
        AbstractControl(background, foreground, x, y, width, height) { }

    Property<unsigned int>& GetFontSize() { return this->fontSize; }
    Property<std::string>& GetText() { return this->text; }
    Property<AlignmentMode>& GetTextAlignment() { return this->textAlignment; }
    Property<TextClippingMode>& GetTextClippingMode() { return this->textClippingMode; }

    // Overrides.
    void Render(std::shared_ptr<AbstractGraphicsContext> graphicsContext);
    void Position(
        std::shared_ptr<AbstractGraphicsContext> graphicsContext,
        unsigned int maxWidth,
        unsigned int maxHeight);

private:
    void ComputeTextAlignment(std::shared_ptr<AbstractGraphicsContext> graphicsContext);
    void ComputeTextClipping(std::shared_ptr<AbstractGraphicsContext> graphicsContext);

    Property<unsigned int> fontSize;
    Property<std::string> text;
    Property<AlignmentMode> textAlignment;
    Property<TextClippingMode> textClippingMode;
    int textXOffset = 0;
    int textYOffset = 0;
    std::vector<size_t> textClippingIndicies;
    int wrappingFontHeight = 0;
};
