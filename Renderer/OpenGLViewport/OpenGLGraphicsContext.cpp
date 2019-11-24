#include "OpenGLGraphicsContext.h"

#include <gl/glew.h>
#include <freetype-gl/freetype-gl.h>
#include <freetype-gl/vertex-buffer.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// Include copied sources from Freetype-GL samples. ❤
#include "../Text/mat4.h"
#include "../Text/shader.h"

using namespace ftgl;

namespace
{
    // Seemingly unavoidable globals.
    GLuint textShader;
    mat4  model, view, projection;
    texture_atlas_t* atlas = nullptr;
    vertex_buffer_t* buffer = nullptr;

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
    void AddTextToBuffer(vertex_buffer_t* buffer, texture_font_t* font, char* text, vec4* color, vec2* pen)
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

    void InitializeText()
    {
        size_t i;
        texture_font_t* font = 0;
        atlas = texture_atlas_new(512, 512, 1);
        const char* filename = "Content/Cascadia-Code/Cascadia.ttf";
        char* text = "Hello world";
        buffer = vertex_buffer_new("vertex:3f,tex_coord:2f,color:4f");
        vec2 pen = { {0,70} };
        vec4 black = { {0,0,0,1} };
        //for (i = 7; i < 27; ++i)
        {
            font = texture_font_new_from_file(atlas, 27, filename);
            pen.x = 5;
            pen.y -= font->height;
            black.a *= .9;
            texture_font_load_glyphs(font, text);
            AddTextToBuffer(buffer, font, text, &black, &pen);
            texture_font_delete(font);
        }

        glGenTextures(1, &atlas->id);
        glBindTexture(GL_TEXTURE_2D, atlas->id);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlas->width, atlas->height,
            0, GL_RED, GL_UNSIGNED_BYTE, atlas->data);

        textShader = shader_load("Content/FreeType-GL/v3f-t2f-c4f.vert", "Content/FreeType-GL/v3f-t2f-c4f.frag");

        mat4_set_identity(&projection);
        mat4_set_identity(&model);
        mat4_set_identity(&view);
    }

    void CleanupText()
    {
        glDeleteTextures(1, &atlas->id);
        atlas->id = 0;
        texture_atlas_delete(atlas);
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

void OpenGLGraphicsContext::FinalizeDrawing()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

// Windows Macro obnoxiously collides with function name here..
#ifdef DrawText
#undef DrawText
#endif // DrawText

void OpenGLGraphicsContext::DrawText(std::string& text, unsigned int x, unsigned int y)
{

}
