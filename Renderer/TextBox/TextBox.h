#pragma once

#include <memory>
#include <vector>
#include "../Framework/Controls/AbstractControl.h"
#include "../Framework/Controls/TextSegment.h"
#include "../Utilities/Property.h"
#include "../Text/TextDocument.h"

class TextBox : public AbstractControl
{
public:
    TextBox(
        std::shared_ptr<TextDocument> textDocument,
        Color background,
        Color foreground,
        unsigned int x,
        unsigned int y,
        unsigned int width,
        unsigned int height);

    Property<Color>& GetCaretColor() { return this->caretColor; }
    Property<unsigned int>& GetCaretPosition() { return this->caretPosition; }
    Property<unsigned int>& GetCaretThickness() { return this->caretThickness; }
    Property<unsigned int>& GetFontSize() { return this->fontSize; }
    Property<unsigned int>& GetScrollLine() { return this->scrollLine; }
    Property<unsigned int>& GetScrollRate() { return this->scrollRate; }

protected:
    void Position(
        std::shared_ptr<AbstractGraphicsContext> graphicsContext,
        unsigned int maxWidth,
        unsigned int maxHeight);

    // Overrides.
    bool SendKey(Key key, KeyAction action, char character);
    void Scroll(int x, int y, int scrollX, int scrollY);

private:
    Property<Color> caretColor;
    Property<unsigned int> caretPosition;
    Property<unsigned int> caretThickness;
    Property<unsigned int> fontSize;
    Property<unsigned int> scrollLine;
    Property<unsigned int> scrollRate;
    std::shared_ptr<TextDocument> textDocument;
};
