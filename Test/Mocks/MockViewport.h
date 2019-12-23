#pragma once

#include "../../Renderer/Framework/Controls/AbstractControl.h"

class MockViewport : public AbstractControl
{
public:
    MockViewport(unsigned int width, unsigned int height)
        : AbstractControl(colors::Black, colors::White, 0, 0, width, height)
    {
    }

    void Render(std::shared_ptr<AbstractGraphicsContext> graphicsContext)
    {
        AbstractControl::Render(graphicsContext);
    }

    void Position(
        std::shared_ptr<AbstractGraphicsContext> graphicsContext)
    {
        AbstractControl::Position(
            graphicsContext,
            this->GetWidth().GetValue(),
            this->GetHeight().GetValue());
    }
};
