#pragma once

#include <string>
#include <vector>

#include "../../Framework/Color.h"

#include <freetype-gl/freetype-gl.h>
#include <freetype-gl/vertex-buffer.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// Include copied sources from Freetype-GL samples. ❤
#include "mat4.h"
#include "shader.h"

using namespace ftgl;

// Windows Macro obnoxiously collides with function name here..
#ifdef DrawText
#undef DrawText
#endif // DrawText

class OpenGLTextContext
{
public:
    OpenGLTextContext();
    ~OpenGLTextContext();

    void AddText(
        const std::string& text,
        unsigned int fontSize,
        const Color& color,
        unsigned int x,
        unsigned int y);

    std::tuple<unsigned int, unsigned int, size_t> ComputeTextDimensions(
        const std::string& text,
        unsigned int size,
        unsigned int maxWidth);

    void Clear();
    void Draw();
    void Reshape(unsigned int width, unsigned int height);

private:
    texture_font_t* GetOrCreateFont(unsigned int size);

    GLuint textShader;
    mat4  model;
    mat4 view;
    mat4 projection;
    texture_atlas_t* atlas = nullptr;
    vertex_buffer_t* buffer = nullptr;
    std::vector<texture_font_t*> fontCache;
};
