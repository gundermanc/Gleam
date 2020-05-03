#pragma once

#include "../Framework/AbstractViewport.h"
#include "../Framework/Controls/AbstractControl.h"
#include "../Utilities/AbstractLogger.h"
#include "../Utilities/ChildProcess.h"

#include <memory>

void ShowRemoteView(AbstractLogger& logger, AbstractViewport& viewport);

class RemoteView : public AbstractControl
{
public:
    RemoteView(
        AbstractLogger& logger,
        Color background,
        Color foreground,
        unsigned int x,
        unsigned int y,
        unsigned int width,
        unsigned int height);
    ~RemoteView();

    void Render(std::shared_ptr<AbstractGraphicsContext> graphicsContext);

private:
    AbstractLogger& logger;
    ChildProcess* serverProcess;
};