#include "OpenGLTextContext.h"

namespace
{
    typedef struct {
        float x, y, z;    // position
        float s, t;       // texture
        float r, g, b, a; // color
    } vertex_t;
}

OpenGLTextContext::OpenGLTextContext()
{
    size_t i;
    this->atlas = texture_atlas_new(512, 512, 1);
    glGenTextures(1, &atlas->id);
    this->buffer = vertex_buffer_new("vertex:3f,tex_coord:2f,color:4f");

    this->textShader = shader_load("Content/FreeType-GL/v3f-t2f-c4f.vert", "Content/FreeType-GL/v3f-t2f-c4f.frag");

    mat4_set_identity(&this->projection);
    mat4_set_identity(&this->model);
    mat4_set_identity(&this->view);
}

OpenGLTextContext::~OpenGLTextContext()
{
    // Cleanup font cache.
    for (texture_font_t* font : fontCache)
    {
        if (font != nullptr)
        {
            texture_font_delete(font);
        }
    }

    // Delete open GL textures.
    glDeleteTextures(1, &atlas->id);
    atlas->id = 0;
    texture_atlas_delete(atlas);
}

/*------------------------------------------
 * Function adapted from Freetype GL - A C OpenGL Freetype engine
 *
 * Distributed under the OSI - approved BSD 2 - Clause License. See accompanying
 * file `LICENSE` for more details.
 */
void OpenGLTextContext::AddText(
    const std::string& text,
    unsigned int fontSize,
    const Color& color,
    unsigned int x,
    unsigned int y)
{
    auto font = GetOrCreateFont(fontSize);

    y += font->height;

    size_t i;
    float r = color.Red;
    float g = color.Green;
    float b = color.Blue;
    float a = color.Alpha;

    for (i = 0; i < text.size(); ++i)
    {
        texture_glyph_t* glyph = texture_font_get_glyph(font, &text.at(i));
        if (glyph != nullptr)
        {
            float kerning = 0.0f;
            if (i > 0)
            {
                kerning = texture_glyph_get_kerning(glyph, &text.at(i - 1));
            }

            x += kerning;
            float x0 = (int)(x + glyph->offset_x);
            float y0 = (int)(y - glyph->offset_y);
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
            x += glyph->advance_x;
        }
    }
}

std::tuple<unsigned int, unsigned int, size_t> OpenGLTextContext::ComputeTextDimensions(
    const std::string& text,
    unsigned int size,
    unsigned int maxWidth)
{
    auto font = GetOrCreateFont(size);

    vec2 originalPen = { 0, 0 };
    vec2 pen = { 0, 0 };

    size_t i;
    for (i = 0; i < text.size(); ++i)
    {
        texture_glyph_t* glyph = texture_font_get_glyph(font, &text.at(i));
        if (glyph != nullptr)
        {
            float kerning = 0.0f;
            if (i > 0)
            {
                kerning = texture_glyph_get_kerning(glyph, &text.at(i - 1));
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

void OpenGLTextContext::Clear()
{
    vertex_buffer_clear(buffer);
}

void OpenGLTextContext::Draw()
{
    // Generate and bind texture.
    glBindTexture(GL_TEXTURE_2D, atlas->id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlas->width, atlas->height,
        0, GL_RED, GL_UNSIGNED_BYTE, atlas->data);

    // Render text from buffer.
    glUseProgram(this->textShader);
    {
        glUniform1i(glGetUniformLocation(this->textShader, "texture"),
            0);

        glUniformMatrix4fv(glGetUniformLocation(this->textShader, "model"),
            1, 0, model.data);

        glUniformMatrix4fv(glGetUniformLocation(this->textShader, "view"),
            1, 0, view.data);

        glUniformMatrix4fv(glGetUniformLocation(this->textShader, "projection"),
            1, 0, projection.data);

        vertex_buffer_render(buffer, GL_TRIANGLES);
    }
    glUseProgram(0);
}

void OpenGLTextContext::Reshape(unsigned int width, unsigned int height)
{
    mat4_set_orthographic(&projection, 0, width, height, 0, 1, -1);
}

texture_font_t* OpenGLTextContext::GetOrCreateFont(unsigned int size)
{
    const char* fontFileName = "Content/Cascadia-Code/Cascadia.ttf";

    texture_font_t* font = nullptr;

    // Check cache for the given font.
    if (size < this->fontCache.size())
    {
        auto font = this->fontCache[size];
        if (font != nullptr)
        {
            return font;
        }
    }
    else
    {
        // If cache is too small for the chosen font size, make it larger.
        this->fontCache.resize(size + 1);
    }

    // Not there.. create it.
    font = texture_font_new_from_file(atlas, size, fontFileName);

    // Add it to the cache.
    this->fontCache[size] = font;

    return font;
}
