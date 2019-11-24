#pragma once

#include <memory>
#include <string>

#include "AbstractControl.h"
#include "AbstractGraphicsContext.h"
#include "Property.h"

class AbstractViewport : AbstractControl {
public:
    AbstractViewport(std::shared_ptr<AbstractGraphicsContext> graphicsContext, unsigned int width, unsigned int height)
        : AbstractControl(colors::Black, colors::White, 500, 500)
    {
        this->graphicsContext = graphicsContext;
    }

    const Property<std::string>& GetTitle() const { return this->title; }

private:
    std::shared_ptr<AbstractGraphicsContext> graphicsContext;
    Property<std::string> title;
};
