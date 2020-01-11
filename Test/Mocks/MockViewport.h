#pragma once

#include "../../Renderer/Framework/Controls/AbstractControl.h"

class MockViewport : public AbstractControl
{
public:
    MockViewport(unsigned int width, unsigned int height)
        : AbstractControl(colors::Black, colors::White, 0, 0, width, height),
        mockGraphicsContext(new MockGraphicsContext())
    {
    }

    void Render()
    {
        AbstractControl::Render(this->mockGraphicsContext);
    }

    void Position()
    {
        AbstractControl::Position(
            this->mockGraphicsContext,
            this->GetWidth().GetValue(),
            this->GetHeight().GetValue());
    }
private:
    std::shared_ptr<MockGraphicsContext> mockGraphicsContext;
};
