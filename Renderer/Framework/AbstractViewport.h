#pragma once

#include <memory>
#include <string>

#include "Controls/AbstractControl.h"
#include "AbstractGraphicsContext.h"
#include "Property.h"

class AbstractViewport : public AbstractControl {
public:
    AbstractViewport(std::string title, std::shared_ptr<AbstractGraphicsContext> graphicsContext, unsigned int width, unsigned int height)
        : title(title), graphicsContext(graphicsContext), AbstractControl(colors::Black, colors::White, 0, 0, 500, 500) { }

    Property<std::string>& GetTitle() { return this->title; }

private:
    std::shared_ptr<AbstractGraphicsContext> graphicsContext;
    Property<std::string> title;
};
