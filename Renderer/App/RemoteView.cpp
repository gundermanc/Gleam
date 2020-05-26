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

    bool TryGetFloat(
        nlohmann::json& json,
        const std::string& key,
        float& value)
    {
        auto valueJson = json[key];
        if (valueJson != nullptr &&
            (valueJson.is_number_integer() || valueJson.is_number_float()))
        {
            value = valueJson;
            return true;
        }

        return false;
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

    bool TryGetColor(
        nlohmann::json& json,
        const std::string& key,
        Color& color)
    {
        nlohmann::json objJson;
        float r;
        float g;
        float b;
        float a;
        if (TryGetObjectJson(json, key, objJson) &&
            TryGetFloat(objJson, "R", r) &&
            TryGetFloat(objJson, "G", g) &&
            TryGetFloat(objJson, "B", b) &&
            TryGetFloat(objJson, "A", a))
        {
            color = Color(r, g, b, a);
            return true;
        }

        return false;
    }

    // Must be kept in sync with compute process.
    enum class RenderInstructionType
    {
        Text,
        Rectangle,
        RectangleOutline
    };

    void RenderTextInstruction(
        AbstractLogger& logger,
        std::shared_ptr<AbstractGraphicsContext> graphicsContext,
        nlohmann::json& instructionParamsJson,
        unsigned int x,
        unsigned int y,
        unsigned int width,
        unsigned int height)
    {
        Color color = colors::Black;
        std::string text;
        unsigned int size = 0;

        if (!TryGetColor(instructionParamsJson, "color", color) ||
            !TryGetString(instructionParamsJson, "text", text) ||
            !TryGetUint(instructionParamsJson, "size", size))
        {
            logger.LogAndFailFast("RenderTextInstruction is missing mandatory parameter.");
        }

        graphicsContext->DrawText(text, color, size, x, y);
    }

    void RenderRectangleInstruction(
        AbstractLogger& logger,
        std::shared_ptr<AbstractGraphicsContext> graphicsContext,
        nlohmann::json& instructionParamsJson,
        unsigned int x,
        unsigned int y,
        unsigned int width,
        unsigned int height)
    {
        Color color = colors::Black;

        if (!TryGetColor(instructionParamsJson, "color", color))
        {
            logger.LogAndFailFast("RenderRectangleInstruction is missing mandatory parameter 'color'.");
        }

        graphicsContext->DrawRect(color, x, y, width, height);
    }

    void RenderRectangleOutlineInstruction(
        AbstractLogger& logger,
        std::shared_ptr<AbstractGraphicsContext> graphicsContext,
        nlohmann::json& instructionParamsJson,
        unsigned int x,
        unsigned int y,
        unsigned int width,
        unsigned int height)
    {
        Color color = colors::Black;
        float thickness = 0;

        if (!TryGetColor(instructionParamsJson, "color", color) ||
            !TryGetFloat(instructionParamsJson, "thickness", thickness))
        {
            logger.LogAndFailFast("RenderRectangleOutlineInstruction is missing mandatory parameter.");
        }

        graphicsContext->DrawRectOutline(color, thickness, x, y, width, height);
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

                if (!TryGetUint(*it, "type", type))
                {
                    logger.LogAndFailFast("RenderInstruction has no type.");
                }

                // Decode mandatory parameters.
                unsigned int x = 0;
                unsigned int y = 0;
                unsigned int width = 0;
                unsigned int height = 0;
                nlohmann::json instructionParamsJson;
                if (!TryGetUint(*it, "x", x) ||
                    !TryGetUint(*it, "y", y) ||
                    !TryGetUint(*it, "width", width) ||
                    !TryGetUint(*it, "height", height) ||
                    !TryGetObjectJson(*it, "params", instructionParamsJson))
                {
                    logger.LogAndFailFast("RenderInstruction is missing mandatory parameter.");
                }

                switch (static_cast<RenderInstructionType>(type))
                {
                case RenderInstructionType::Text:
                    RenderTextInstruction(logger, graphicsContext, instructionParamsJson, x, y, width, height);
                    break;
                case RenderInstructionType::Rectangle:
                    RenderRectangleInstruction(logger, graphicsContext, instructionParamsJson, x, y, width, height);
                    break;
                case RenderInstructionType::RectangleOutline:
                    RenderRectangleOutlineInstruction(logger, graphicsContext, instructionParamsJson, x, y, width, height);
                    break;
                default:
                    logger.LogAndFailFast("RenderInstruction provided unrecognized instruction type.");
                }
            }
        }
    }

    void ProvideTextDimensions(
        AbstractLogger& logger,
        ChildProcess& childProcess,
        nlohmann::json requestJson,
        std::shared_ptr<AbstractGraphicsContext> graphicsContext)
    {
        // Validate request message.
        std::string name;
        if (!TryGetString(requestJson, "name", name) || name != "gleam/render/textDimensions")
        {
            logger.LogAndFailFast("ProvideTextDimensions called without proper JSON request body.");
        }

        // Decode mandatory parameters.
        nlohmann::json instructionParamsJson;
        unsigned int size = 0;
        std::string text;
        if (!TryGetObjectJson(requestJson, "params", instructionParamsJson) ||
            !TryGetUint(instructionParamsJson, "size", size) ||
            !TryGetString(instructionParamsJson, "text", text))
        {
            logger.LogAndFailFast("ProvideTextDimensions is missing mandatory parameter.");
        }

        auto dimensions = graphicsContext->ComputeTextDimensions(text, size, 999999);

        // Issue response.
        nlohmann::json responseJson;
        responseJson["name"] = std::string("gleam/render/textDimensions");
        responseJson["logString"] = std::string("Response to dimensions request");

        nlohmann::json paramsJson;
        paramsJson["width"] = std::get<0>(dimensions);
        paramsJson["height"] = std::get<1>(dimensions);

        responseJson["params"] = paramsJson;

        WriteJsonRequest(logger, childProcess, responseJson);
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

    // Handle any requests for additional rendering information.
    std::string name;
    while (TryGetString(response, "name", name) &&
        (name == "gleam/render/textDimensions"))
    {
        ProvideTextDimensions(this->logger, *this->serverProcess, response, graphicsContext);
        response = ReadJsonResponse(this->logger, *this->serverProcess);
    }

    // Validate response message.
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

bool RemoteView::SendKey(Key key, KeyAction action, char character)
{
    nlohmann::json requestJson;
    requestJson["name"] = std::string("gleam/input/key");
    requestJson["logString"] = std::string("Sending key");

    nlohmann::json paramsJson;
    paramsJson["key"] = key;
    paramsJson["action"] = action;
    paramsJson["character"] = character;

    requestJson["params"] = paramsJson;
    WriteJsonRequest(this->logger, *this->serverProcess, requestJson);
    auto response = ReadJsonResponse(this->logger, *this->serverProcess);

    std::string name;
    if (!TryGetString(response, "name", name) || name != "gleam/ack")
    {
        this->logger.LogAndFailFast("Failed to send keystroke to server. Unrecognized response.");
    }

    // TODO: what do if not handled?
    return true;
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
        0,
        0,
        viewport.GetWidth().GetValue(),
        viewport.GetHeight().GetValue());

    // Add resize handler.
    viewport.GetWidth().SubscribeToChange([remoteView](unsigned int oldValue, unsigned int newValue)
    {
        remoteView->GetWidth().SetValue(newValue);
        remoteView->SendDimensions();
    });
    viewport.GetHeight().SubscribeToChange([remoteView](unsigned int oldValue, unsigned int newValue)
    {
        remoteView->GetHeight().SetValue(newValue);
        remoteView->SendDimensions();
    });

    viewport.GetChildren().push_back(std::shared_ptr<AbstractControl>(remoteView));
}
