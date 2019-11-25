#include "OpenGLGraphicsContext.h"

#include <vector>
#include <gl/glew.h>
#include <freetype-gl/freetype-gl.h>
#include <freetype-gl/vertex-buffer.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// Include copied sources from Freetype-GL samples. ❤
#include "Text/mat4.h"
#include "Text/shader.h"

using namespace ftgl;

// Windows Macro obnoxiously collides with function name here..
#ifdef DrawText
#undef DrawText
#endif // DrawText

namespace
{
    // Seemingly unavoidable globals.
    GLuint textShader;
    mat4  model;
    mat4 view;
    mat4 projection;
    texture_atlas_t* atlas = nullptr;
    vertex_buffer_t* buffer = nullptr;
    std::vector<texture_font_t*> fontCache;

    typedef struct {
        float x, y, z;    // position
        float s, t;       // texture
        float r, g, b, a; // color
    } vertex_t;

    /*------------------------------------------
     * Function adapted from Freetype GL - A C OpenGL Freetype engine
     *
     * Distributed under the OSI - approved BSD 2 - Clause License. See accompanying
     * file `LICENSE` for more details.
     */
    void AddTextToBuffer(vertex_buffer_t* buffer, texture_font_t* font, const char* text, vec4* color, vec2* pen)
    {
        size_t i;
        float r = color->red, g = color->green, b = color->blue, a = color->alpha;
        for (i = 0; i < strlen(text); ++i)
        {
            texture_glyph_t* glyph = texture_font_get_glyph(font, text + i);
            if (glyph != NULL)
            {
                float kerning = 0.0f;
                if (i > 0)
                {
                    kerning = texture_glyph_get_kerning(glyph, text + i - 1);
                }

                pen->x += kerning;
                float x0 = (int)(pen->x + glyph->offset_x);
                float y0 = (int)(pen->y - glyph->offset_y);
                float x1 = (int)(x0 + glyph->width);
                float y1 = (int)(y0 + glyph->height);
                float s0 = glyph->s0;
                float t0 = glyph->t0;
                float s1 = glyph->s1;
                float t1 = glyph->t1;
                GLuint indices[6] = { 0,1,2, 0,2,3 };
                vertex_t vertices[4] = { { x0,y0,0,  s0,t0,  r,g,b,a },
                                         { x0,y1,0,  s0,t1,  r,g,b,a },
                                         { x1,y1,0,  s1,t1,  r,g,b,a },
                                         { x1,y0,0,  s1,t0,  r,g,b,a } };
                vertex_buffer_push_back(buffer, vertices, 4, indices, 6);
                pen->x += glyph->advance_x;
            }
        }
    }

    texture_font_t* GetOrCreateFont(unsigned int size)
    {
        const char* fontFileName = "Content/Cascadia-Code/Cascadia.ttf";

        texture_font_t* font = nullptr;

        // Check cache for the given font.
        if (size < fontCache.size())
        {
            auto font = fontCache[size];
            if (font != nullptr)
            {
                return font;
            }
        }
        else
        {
            // If cache is too small for the chosen font size, make it larger.
            fontCache.resize(size + 1);
        }

        // Not there.. create it.
        font = texture_font_new_from_file(atlas, size, fontFileName);

        // Add it to the cache.
        fontCache[size] = font;

        return font;
    }

    void CleanupFonts()
    {
        for (texture_font_t* font : fontCache)
        {
            if (font != nullptr)
            {
                texture_font_delete(font);
            }
        }
    }

    void DrawTextFull(const char* text, const Color& color, unsigned int size, unsigned int x, unsigned int y)
    {
        // Load necessary glyphs.
        // TODO: this is probably exceptionally inefficient and should instead use a list of unique glyphs?
        auto font = GetOrCreateFont(size);

        vec2 pen = { {static_cast<float>(x), static_cast<float>(y) + font->height} };
        vec4 black = { {color.Red, color.Green, color.Blue, color.Alpha} };

        AddTextToBuffer(buffer, font, text, &black, &pen);

        // Generate and bind textures.
        // TODO: this should probably not be done each render pass.
        glBindTexture(GL_TEXTURE_2D, atlas->id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlas->width, atlas->height,
            0, GL_RED, GL_UNSIGNED_BYTE, atlas->data);
    }

    std::tuple<unsigned int, unsigned int, size_t> ComputeTextDimensionsFull(const char* text, unsigned int size, unsigned int maxWidth)
    {
        // Load necessary glyphs.
        // TODO: this is probably exceptionally inefficient and should instead use a list of unique glyphs?
        auto font = GetOrCreateFont(size);

        vec2 originalPen = { 0, 0 };
        vec2 pen = { 0, 0 };

        size_t i;
        for (i = 0; i < strlen(text); ++i)
        {
            texture_glyph_t* glyph = texture_font_get_glyph(font, text + i);
            if (glyph != NULL)
            {
                float kerning = 0.0f;
                if (i > 0)
                {
                    kerning = texture_glyph_get_kerning(glyph, text + i - 1);
                }

                auto nextX = pen.x + kerning + glyph->advance_x;
                if (nextX > maxWidth || text[i] == '\n')
                {
                    break;
                }

                pen.x = nextX;
            }
        }

        return std::make_tuple(
            static_cast<unsigned int>(pen.x - originalPen.x),
            static_cast<unsigned int>(font->height),
            i);
    }

    void InitializeText()
    {
        size_t i;
        atlas = texture_atlas_new(512, 512, 1);
        glGenTextures(1, &atlas->id);
        buffer = vertex_buffer_new("vertex:3f,tex_coord:2f,color:4f");

        textShader = shader_load("Content/FreeType-GL/v3f-t2f-c4f.vert", "Content/FreeType-GL/v3f-t2f-c4f.frag");

        mat4_set_identity(&projection);
        mat4_set_identity(&model);
        mat4_set_identity(&view);
    }

    void CleanupText()
    {
        CleanupFonts();
        glDeleteTextures(1, &atlas->id);
        atlas->id = 0;
        texture_atlas_delete(atlas);
    }

    void MapAbsoluteCoordinatesToUnitOrtho(float& x, float& y, float& width, float& height)
    {
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        GLint viewportWidth = viewport[2];
        GLint viewportHeight = viewport[3];

        // I'm sure there's a more OpenGL-y way to do this but this is effective for now.
        x = (((float)x / viewportWidth) * 2.0) - 1.0;
        y = -((((float)y / viewportHeight) * 2.0) - 1.0);
        width = ((float)width / viewportWidth) * 2.0;
        height = ((float)height / viewportHeight) * 2.0;
    }
}

OpenGLGraphicsContext::OpenGLGraphicsContext()
{
    InitializeText();
}

OpenGLGraphicsContext::~OpenGLGraphicsContext()
{
    CleanupText();
}

void OpenGLGraphicsContext::BeginDrawing()
{
    vertex_buffer_clear(buffer);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLGraphicsContext::FinalizeDrawing()
{
    glUseProgram(textShader);
    {
        glUniform1i(glGetUniformLocation(textShader, "texture"),
            0);

        glUniformMatrix4fv(glGetUniformLocation(textShader, "model"),
            1, 0, model.data);

        glUniformMatrix4fv(glGetUniformLocation(textShader, "view"),
            1, 0, view.data);

        glUniformMatrix4fv(glGetUniformLocation(textShader, "projection"),
            1, 0, projection.data);

        vertex_buffer_render(buffer, GL_TRIANGLES);
    }
    glUseProgram(0);
}

void OpenGLGraphicsContext::Reshape(unsigned int width, unsigned int height)
{
    mat4_set_orthographic(&projection, 0, width, height, 0, 1, -1);
}

// Returns width, height, and number of characters consumed before hitting maxWidth or maxHeight.
std::tuple<unsigned int, unsigned int, size_t> OpenGLGraphicsContext::ComputeTextDimensions(
    const std::string& text,
    unsigned int size,
    unsigned int maxWidth)
{
    return ComputeTextDimensionsFull(text.c_str(), size, maxWidth);
}

void OpenGLGraphicsContext::DrawText(const std::string& text, const Color& color, unsigned int size, unsigned int x, unsigned int y)
{
    // TODO: need to look a bit more closely at how to use textures.
    // Having one for the whole window is probably sub-optimal.
    DrawTextFull(text.c_str(), color, size, x, y);
}

void OpenGLGraphicsContext::DrawRect(const Color& color, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
    float adjustedX = x;
    float adjustedY = y;
    float adjustedWidth = width;
    float adjustedHeight = height;
    MapAbsoluteCoordinatesToUnitOrtho(adjustedX, adjustedY, adjustedWidth, adjustedHeight);

    glColor4f(color.Red, color.Green, color.Blue, color.Alpha);
    glRectf(adjustedX, adjustedY, adjustedX + adjustedWidth, adjustedY - adjustedHeight);
}

void OpenGLGraphicsContext::DrawRectOutline(const Color& color, unsigned int thickness, unsigned x, unsigned int y, unsigned int width, unsigned int height)
{
    if (thickness == 0)
    {
        return;
    }

    float adjustedX = x;
    float adjustedY = y;
    float adjustedWidth = width;
    float adjustedHeight = height;
    MapAbsoluteCoordinatesToUnitOrtho(adjustedX, adjustedY, adjustedWidth, adjustedHeight);

    glLineWidth(thickness);
    glColor4f(color.Red, color.Green, color.Blue, color.Alpha);
    glBegin(GL_LINE_LOOP);
        glVertex2f(adjustedX, adjustedY);
        glVertex2f(adjustedX + adjustedWidth, adjustedY);
        glVertex2f(adjustedX + adjustedWidth, adjustedY - adjustedHeight);
        glVertex2f(adjustedX, adjustedY - adjustedHeight);
    glEnd();
}
