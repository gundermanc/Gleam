#include "../Framework/Controls/Border.h"
#include "TextBox.h"
#include <tuple>

TextBox::TextBox(
    std::shared_ptr<TextDocument> textDocument,
    Color background,
    Color foreground,
    unsigned int x,
    unsigned int y,
    unsigned int width,
    unsigned int height)
    : caretColor(foreground),
    caretPosition(0, [textDocument](unsigned int newCaretPosition) { return newCaretPosition <= textDocument->GetLength(); }),
    caretThickness(1),
    fontSize(12, [](unsigned int newFontSize) { return newFontSize > 0; }),
    scrollLine(0, [this](unsigned int newScrollLine) { return newScrollLine >= 0 && newScrollLine < this->textDocument->GetLines().size(); }),
    scrollRate(3, [](unsigned int newScrollRate) { return newScrollRate > 0; }),
    textDocument(textDocument),
    AbstractControl(background, foreground, x, y, width, height)
{
    // Invalid positions when caret position changes.
    this->GetCaretPosition().SubscribeToChange([this](unsigned int oldValue, unsigned int newValue)
    {
        this->GetIsPositionInvalid().SetValue(true);
    });

    // Invalidate position when scroll line changes.
    this->GetScrollLine().SubscribeToChange([this](unsigned int oldValue, unsigned int newValue)
    {
        this->GetIsPositionInvalid().SetValue(true);
    });
}

void TextBox::Position(
    std::shared_ptr<AbstractGraphicsContext> graphicsContext,
    unsigned int maxWidth,
    unsigned int maxHeight)
{
    // TODO: incrementally.
    this->GetChildren().clear();

    auto fontSize = this->GetFontSize().GetValue();
    auto dimensions = graphicsContext->ComputeTextDimensions("", fontSize, this->GetWidth().GetValue());

    auto lineHeight = std::get<1>(dimensions);

    auto lines = this->textDocument->GetLines();
    auto initialLine = this->GetScrollLine().GetValue();

    int y = this->GetY().GetValue();
    for (size_t lineNumber = initialLine; lineNumber < lines.size(); lineNumber++)
    {
        const std::string& line = lines[lineNumber];

        // Draw only the visible portion.
        if (y >= this->GetHeight().GetValue())
        {
            break;
        }

        // TODO: position relative to parent?
        auto textSegment = new TextSegment(
            line,
            fontSize,
            colors::Transparent,
            this->GetForeground().GetValue(),
            this->GetX().GetValue(),
            y,
            this->GetWidth().GetValue(),
            lineHeight);

        this->GetChildren().push_back(std::shared_ptr<AbstractControl>(textSegment));
        y += lineHeight;
    }

    auto caretPosition = this->GetCaretPosition().GetValue();

    auto positionAndLineNumber = this->textDocument->GetPositionAndLineNumberOfLineContainingPosition(caretPosition);
    auto position = std::get<0>(positionAndLineNumber);
    auto lineNumber = std::get<1>(positionAndLineNumber);

    // TODO: is this possible?
    if (lineNumber != -1)
    {
        // Get offset from the start of the line.
        // TODO: cache some of this.
        std::string line = this->textDocument->GetLines().at(lineNumber);
        std::tuple<unsigned int, unsigned int, size_t> linePrefixDimensions = graphicsContext->ComputeTextDimensions(
            line.substr(0, caretPosition - position),
            this->GetFontSize().GetValue(),
            this->GetWidth().GetValue());

        auto linePrefixWidth = std::get<0>(linePrefixDimensions);

        // Draw primitive caret.
        // TODO: incremental.
        auto border = new Border(
            this->GetCaretColor().GetValue(),
            this->GetCaretColor().GetValue(),
            0,
            this->GetX().GetValue() + linePrefixWidth,
            this->GetY().GetValue() + (lineHeight * lineNumber),
            this->GetCaretThickness().GetValue(),
            lineHeight);

        this->GetChildren().push_back(std::shared_ptr<AbstractControl>(border));
    }

    AbstractControl::Position(
        graphicsContext,
        maxWidth,
        maxHeight);
}

bool TextBox::SendKey(Key key, KeyAction action, char character)
{
    if (action == KeyAction::TypeCharacter)
    {
        this->textDocument->Insert(std::string(1, character), this->GetCaretPosition().GetValue());
        this->GetCaretPosition().SetValue(this->GetCaretPosition().GetValue() + 1);

        // TODO: do as a side effect of document changing.
        this->GetIsPositionInvalid().SetValue(true);
        return true;
    }

    if (action != KeyAction::Press && action != KeyAction::Repeat)
    {
        return false;
    }

    switch (key)
    {
    case Key::Backspace:
    {
        auto caretPosition = this->GetCaretPosition().GetValue();

        if (caretPosition > 0)
        {
            this->textDocument->Remove(this->GetCaretPosition().GetValue() - 1, 1);
            this->GetCaretPosition().SetValue(caretPosition - 1);

            // TODO: do as a side effect of document changing.
            this->GetIsPositionInvalid().SetValue(true);
        }
        return true;
    }
    case Key::Left:
        this->GetCaretPosition().SetValue(this->GetCaretPosition().GetValue() - 1);
        return true;
    case Key::Right:
        this->GetCaretPosition().SetValue(this->GetCaretPosition().GetValue() + 1);
        return true;
    }

    return false;
}

void TextBox::Scroll(int x, int y, int scrollX, int scrollY)
{
    auto& scrollLineProperty = this->GetScrollLine();
    auto scrollLinePropertyValue = scrollLineProperty.GetValue();
    int updatedScrollLinePropertyValue = scrollLinePropertyValue - (scrollY * this->GetScrollRate().GetValue());
    auto totalLines = this->textDocument->GetLines().size();

    // Update scroll position and invalidate.
    scrollLineProperty.SetValue(updatedScrollLinePropertyValue);
}
