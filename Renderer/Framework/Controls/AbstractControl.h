#pragma once

#include <memory>
#include <vector>
#include "../AbstractGraphicsContext.h"
#include "../Color.h"
#include "../Keys.h"
#include "../../Utilities/Property.h"

class AbstractControl
{
public:
    AbstractControl(Color background, Color foreground, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
        : background(background),
        foreground(foreground),
        borderColor(colors::Black),
        borderThickness(0),
        x(x),
        y(y),
        width(width),
        height(height),
        isPositionInvalid(false) { }

    Property<Color>& GetBackground() { return this->background; }
    Property<Color>& GetForeground() { return this->foreground; }
    Property<Color>& GetBorderColor() { return this->borderColor; }
    Property<unsigned int>& GetBorderThickness() { return this->borderThickness; }
    Property<unsigned int>& GetX() { return this->x; }
    Property<unsigned int>& GetY() { return this->y; }
    Property<unsigned int>& GetWidth() { return this->width; }
    Property<unsigned int>& GetHeight() { return this->height; }
    Property<bool>& GetIsPositionInvalid() { return this->isPositionInvalid; }

    std::vector<std::shared_ptr<AbstractControl>>& GetChildren() { return this->children; }

    // TODO: implement support for accelerators, focus, etc.
    // TODO: implement support for multi-char characters.
    virtual bool SendKey(Key key, KeyAction action, char character);
    virtual void Scroll(int x, int y, int scrollX, int scrollY);

protected:
    virtual void Render(std::shared_ptr<AbstractGraphicsContext> graphicsContext);
    virtual void Position(
        std::shared_ptr<AbstractGraphicsContext> graphicsContext,
        unsigned int maxWidth,
        unsigned int maxHeight);

private:
    Property<Color> background;
    Property<Color> foreground;
    Property<Color> borderColor;
    Property<unsigned int> borderThickness;
    Property<unsigned int> x;
    Property<unsigned int> y;
    Property<unsigned int> width;
    Property<unsigned int> height;
    Property<bool> isPositionInvalid;

    std::vector<std::shared_ptr<AbstractControl>> children;
};
