#pragma once

#include <memory>
#include <vector>
#include "../Framework/Controls/AbstractControl.h"
#include "../Framework/Controls/TextSegment.h"
#include "../Framework/Property.h"
#include "../Text/TextDocument.h"

class TextBox : public AbstractControl
{
public:
    TextBox(std::shared_ptr<TextDocument> textDocument, Color background, Color foreground, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
        : fontSize(12),
        AbstractControl(background, foreground, x, y, width, height)
    {
        this->textDocument = textDocument;
    }

    Property<unsigned int> GetFontSize() { return this->fontSize; }

protected:
    void Position(
        std::shared_ptr<AbstractGraphicsContext> graphicsContext,
        unsigned int maxWidth,
        unsigned int maxHeight);

private:
    Property<unsigned int> fontSize;
    std::shared_ptr<TextDocument> textDocument;
    unsigned int scrollLine = 0;
};
