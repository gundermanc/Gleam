#pragma once

#include <string>

class AbstractGraphicsContext
{
public:
    virtual void DrawText(std::string& text, unsigned int x, unsigned int y) = 0;
};
