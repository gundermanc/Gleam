#include "RemoteView.h"

#include "../Framework/Controls/TextSegment.h"

#include <nlohmann/json.hpp>

namespace
{
    nlohmann::json ReadJsonResponse(AbstractLogger& logger, ChildProcess& childProcess)
    {
        std::string line;

        // TODO: what's going on here? We seem to frequently get back empty lines.
        while (line.size() == 0 ||
            line.size() == 1 && line[0] == '\n')
        {
            line = childProcess.ReadStdOutLine();
        }

        try
        {
            return nlohmann::json::parse(line);
        }
        catch (const std::exception& ex)
        {
            logger.Log(LogSeverity::Warning, "Failed to deserialize JSON response " + std::string(ex.what()));
            return nlohmann::json();
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

    bool TryGetUint(
        nlohmann::json& json,
        const std::string& key,
        unsigned int& value)
    {
        auto valueJson = json[key];
        if (valueJson != nullptr && valueJson.is_number_integer())
        {
            value = valueJson;
            return true;
        }

        return false;
    }

    bool TryGetString(
        nlohmann::json& json,
        const std::string& key,
        std::string& value)
    {
        auto valueJson = json[key];
        if (valueJson != nullptr && valueJson.is_string())
        {
            value = valueJson;
            return true;
        }

        return false;
    }

    bool TryGetObjectJson(
        nlohmann::json& json,
        const std::string& key,
        nlohmann::json& objJson)
    {
        auto valueJson = json[key];
        if (valueJson != nullptr && valueJson.is_object())
        {
            objJson = valueJson;
            return true;
        }

        return false;
    }

    void RenderInstructions(
        AbstractLogger& logger,
        std::shared_ptr<AbstractGraphicsContext> graphicsContext,
        nlohmann::json& responseJson)
    {
        // Validate response message.
        std::string name;
        if (!TryGetString(responseJson, "name", name) || name != "gleam/render/doLayout")
        {
            logger.LogAndFailFast("RenderInstructions called without proper JSON response body.");
        }

        auto paramsJson = responseJson["params"];
        auto instructionsJson = paramsJson["instructions"];
        if (instructionsJson.is_array())
        {
            for (auto it = instructionsJson.begin(); it != instructionsJson.end(); ++it)
            {
                unsigned int type = 0;
                unsigned int x = 0;
                unsigned int y = 0;
                unsigned int width = 0;
                unsigned int height = 0;
                nlohmann::json instructionParamsJson;
                std::string text;
                unsigned int size = 0;

                if (TryGetUint(*it, "type", type) &&
                    TryGetUint(*it, "x", x) &&
                    TryGetUint(*it, "y", y) &&
                    TryGetUint(*it, "width", width) &&
                    TryGetUint(*it, "height", height) &&
                    TryGetObjectJson(*it, "params", instructionParamsJson) &&
                    TryGetString(instructionParamsJson, "text", text) &&
                    TryGetUint(instructionParamsJson, "size", size))
                {
                    graphicsContext->DrawText(text, colors::Green, size, x, y);
                }
            }
        }
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
    if (!TryGetString(response, "name", name) || name != "gleam/initialize")
    {
        this->logger.LogAndFailFast("Failed to initialize server. Unrecognized response.");
    }

    this->SendDimensions();
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

    nlohmann::json paramsJson;
    paramsJson["x"] = this->GetX().GetValue();
    paramsJson["y"] = this->GetY().GetValue();
    paramsJson["width"] = this->GetWidth().GetValue();
    paramsJson["height"] = this->GetHeight().GetValue();

    requestJson["params"] = paramsJson;

    WriteJsonRequest(this->logger, *this->serverProcess, requestJson);
    auto response = ReadJsonResponse(this->logger, *this->serverProcess);

    // Validate response message.
    std::string name;
    if (!TryGetString(response, "name", name) ||
        (name != "gleam/ack" && name != "gleam/render/doLayout"))
    {
        this->logger.LogAndFailFast("Unrecognized response to gleam/render/layout: " + name);
    }

    // Call base renderer.
    AbstractControl::Render(graphicsContext);

    // If drawing was requested, go ahead and do it.
    if (name == "gleam/render/doLayout")
    {
        RenderInstructions(this->logger, graphicsContext, response);
    }
}

// Sends a command to the compute process with updated screen dimensions.
void RemoteView::SendDimensions()
{
    nlohmann::json requestJson;
    requestJson["name"] = std::string("gleam/render/dimensions");
    requestJson["logString"] = std::string("Sending updated dimensions");

    nlohmann::json paramsJson;
    paramsJson["width"] = this->GetWidth().GetValue();
    paramsJson["height"] = this->GetHeight().GetValue();

    requestJson["params"] = paramsJson;
    WriteJsonRequest(this->logger, *this->serverProcess, requestJson);
    auto response = ReadJsonResponse(this->logger, *this->serverProcess);

    std::string name;
    if (!TryGetString(response, "name", name) || name != "gleam/ack")
    {
        this->logger.LogAndFailFast("Failed to update server render dimensions. Unrecognized response.");
    }
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
