#include "TextBox.h"

TextBox::TextBox(
    std::shared_ptr<TextDocument> textDocument,
    Color background,
    Color foreground,
    unsigned int x,
    unsigned int y,
    unsigned int width,
    unsigned int height)
    : fontSize(12, [](unsigned int newFontSize) { return newFontSize > 0; }),
    scrollLine(0, [this](unsigned int newScrollLine) { return newScrollLine >= 0 && newScrollLine < this->textDocument->GetLines().size(); }),
    scrollRate(3, [](unsigned int newScrollRate) { return newScrollRate > 0; }),
    textDocument(textDocument),
    AbstractControl(background, foreground, x, y, width, height)
{
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

    AbstractControl::Position(
        graphicsContext,
        maxWidth,
        maxHeight);
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
