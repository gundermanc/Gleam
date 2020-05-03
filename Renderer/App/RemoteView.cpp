#include "RemoteView.h"

#include "../Framework/Controls/TextSegment.h"

#include <nlohmann/json.hpp>

namespace
{
    nlohmann::json ReadJsonResponse(AbstractLogger& logger, ChildProcess& childProcess)
    {
        const std::string line = childProcess.ReadStdOutLine();

        try
        {
            return nlohmann::json::parse(line);
        }
        catch (const std::exception& ex)
        {
            logger.Log(LogSeverity::Warning, "Failed to deserialize JSON response " + std::string(ex.what()));
        }
    }

    void WriteJsonRequest(
        AbstractLogger& logger,
        ChildProcess& childProcess,
        nlohmann::json requestJson)
    {
        try
        {
            std::string serializedResponse = requestJson.dump();
            childProcess.WriteStdInLine(serializedResponse);
        }
        catch (const std::exception& ex)
        {
            logger.Log(LogSeverity::Warning, "Failed to serialize JSON request " + std::string(ex.what()));
        }
    }

    bool TryGetString(
        nlohmann::json json,
        const std::string& key,
        std::string& value)
    {
        if (json != nullptr)
        {
            auto valueJson = json[key];
            if (valueJson != nullptr && valueJson.is_string())
            {
                value = valueJson;
                return true;
            }
        }

        return false;
    }
}

RemoteView::RemoteView(
    AbstractLogger& logger,
    Color background,
    Color foreground,
    unsigned int x,
    unsigned int y,
    unsigned int width,
    unsigned int height)
    : logger(logger),
    AbstractControl(background, foreground, x, y, width, height)
{
    this->serverProcess = new ChildProcess(logger, "..\\..\\..\\Compute\\Compute.exe");
    this->serverProcess->Start();

    // Check for an 'initialized' message.
    auto response = ReadJsonResponse(this->logger, *this->serverProcess);
    std::string name;
    if (!TryGetString(response, "name", name) || name != "gleam/editor/initialize")
    {
        this->logger.LogAndFailFast("Failed to initialize server. Unrecognized response.");
    }
}

RemoteView::~RemoteView()
{
    delete this->serverProcess;
}

void RemoteView::Render(std::shared_ptr<AbstractGraphicsContext> graphicsContext)
{
    // Issue render command.
    nlohmann::json requestJson;
    requestJson["name"] = std::string("gleam/render/layout");
    requestJson["logString"] = std::string("Request render layout");
    WriteJsonRequest(this->logger, *this->serverProcess, requestJson);
    auto response = ReadJsonResponse(this->logger, *this->serverProcess);

    // Call base renderer.
    AbstractControl::Render(graphicsContext);
}

void ShowRemoteView(AbstractLogger& logger, AbstractViewport& viewport)
{
    // Clear all child controls from the current view.
    viewport.GetChildren().clear();

    viewport.GetBackground().SetValue(colors::Gray);

    auto remoteView = new RemoteView(
        logger,
        colors::Black,
        colors::Green,
        30,
        30,
        viewport.GetWidth().GetValue() - 60,
        50);

    // Add resize handler.
    viewport.GetWidth().SubscribeToChange([remoteView](unsigned int oldValue, unsigned int newValue)
    {
        remoteView->GetWidth().SetValue(newValue - 60);
    });

    viewport.GetChildren().push_back(std::shared_ptr<AbstractControl>(remoteView));
}
