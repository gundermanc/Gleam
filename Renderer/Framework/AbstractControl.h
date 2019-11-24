#pragma once

#include <memory>
#include <vector>
#include "AbstractGraphicsContext.h"
#include "Color.h"
#include "Property.h"

class AbstractControl
{
public:
    AbstractControl(Color background, Color foreground, unsigned int width, unsigned int height)
        : background(background), foreground(foreground), width(width), height(height) { }

    Property<Color>& GetBackground() { return this->background; }
    Property<Color>& GetForeground() { return this->foreground; }
    Property<unsigned int>& GetWidth() { return this->width; }
    Property<unsigned int>& GetHeight() { return this->height; }

    std::vector<std::shared_ptr<AbstractControl>>& GetChildren() { return this->children; }

protected:
    virtual void Render(std::shared_ptr<AbstractGraphicsContext> graphicsContext) = 0;
    virtual void Position(unsigned int maxWidth, unsigned int maxHeight) = 0;

private:
    Property<Color> background;
    Property<Color> foreground;
    Property<unsigned int> width;
    Property<unsigned int> height;

    std::vector<std::shared_ptr<AbstractControl>> children;
};