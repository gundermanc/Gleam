/* Portions of this program make use of open source samples:
 * -------------------------------------------
 * GLFW 'Gears' sample.
 * 
 * https://github.com/glfw/glfw/blob/master/examples/gears.c
 * -------------------------------------------
 * Freetype GL - A C OpenGL Freetype engine
 *
 * Distributed under the OSI-approved BSD 2-Clause License.  See accompanying
 * file `LICENSE` for more details.
 */

#include "OpenGLViewport/OpenGLViewport.h"

//#include <cstdio>
//#include <cstring>
//
//#include <gl/glew.h>
//#include <freetype-gl/freetype-gl.h>
//#include <freetype-gl/vertex-buffer.h>
//
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>
//
//#include "Text/mat4.h"
//#include "Text/shader.h"
//
//texture_atlas_t* atlas = nullptr;
//vertex_buffer_t* buffer = nullptr;
//GLuint textShader;
//
//using namespace ftgl;
//
//mat4  model, view, projection;
//
//typedef struct {
//    float x, y, z;    // position
//    float s, t;       // texture
//    float r, g, b, a; // color
//} vertex_t;
//
//void add_text(vertex_buffer_t* buffer, texture_font_t* font,
//    char* text, vec4* color, vec2* pen)
//{
//    size_t i;
//    float r = color->red, g = color->green, b = color->blue, a = color->alpha;
//    for (i = 0; i < strlen(text); ++i)
//    {
//        texture_glyph_t* glyph = texture_font_get_glyph(font, text + i);
//        if (glyph != NULL)
//        {
//            float kerning = 0.0f;
//            if (i > 0)
//            {
//                kerning = texture_glyph_get_kerning(glyph, text + i - 1);
//            }
//
//            pen->x += kerning;
//            float x0 = (int)(pen->x + glyph->offset_x);
//            float y0 = (int)(pen->y - glyph->offset_y);
//            float x1 = (int)(x0 + glyph->width);
//            float y1 = (int)(y0 + glyph->height);
//            float s0 = glyph->s0;
//            float t0 = glyph->t0;
//            float s1 = glyph->s1;
//            float t1 = glyph->t1;
//            GLuint indices[6] = { 0,1,2, 0,2,3 };
//            vertex_t vertices[4] = { { x0,y0,0,  s0,t0,  r,g,b,a },
//                                     { x0,y1,0,  s0,t1,  r,g,b,a },
//                                     { x1,y1,0,  s1,t1,  r,g,b,a },
//                                     { x1,y0,0,  s1,t0,  r,g,b,a } };
//            vertex_buffer_push_back(buffer, vertices, 4, indices, 6);
//            pen->x += glyph->advance_x;
//        }
//    }
//}
//
//void key( GLFWwindow* window, int k, int s, int action, int mods )
//{
//
//}
//
//void reshape( GLFWwindow* window, int width, int height )
//{
//    glViewport(0, 0, width, height);
//    mat4_set_orthographic(&projection, 0, width, height, 0, 1, -1);
//}
//
//
//void fillRectangle(float x, float y, float width, float height, vec4 color) {
//    glColor4f(color.r, color.g, color.b, color.a);
//    glRectf(x, y, width, height);
//}
//
//void draw()
//{
//    glClearColor(0.5, 05, 0.5, 1);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//    glUseProgram(textShader);
//    {
//        glUniform1i(glGetUniformLocation(textShader, "texture"),
//            0);
//
//        glUniformMatrix4fv(glGetUniformLocation(textShader, "model"),
//            1, 0, model.data);
//
//        glUniformMatrix4fv(glGetUniformLocation(textShader, "view"),
//            1, 0, view.data);
//
//        glUniformMatrix4fv(glGetUniformLocation(textShader, "projection"),
//            1, 0, projection.data);
//
//        vertex_buffer_render(buffer, GL_TRIANGLES);
//    }
//    glUseProgram(0);
//    
//    fillRectangle(0, 0, 100, 100, {{1, 0, 0, 1}});
//}
//
///* program & OpenGL initialization */
//static void init(void)
//{
//    size_t i;
//    texture_font_t* font = 0;
//    atlas = texture_atlas_new(512, 512, 1);
//    const char* filename = "Content/Cascadia-Code/Cascadia.ttf";
//    char* text = "Hello world";
//    buffer = vertex_buffer_new("vertex:3f,tex_coord:2f,color:4f");
//    vec2 pen = { {0,70} };
//    vec4 black = { {0,0,0,1} };
//    //for (i = 7; i < 27; ++i)
//    {
//        font = texture_font_new_from_file(atlas, 27, filename);
//        pen.x = 5;
//        pen.y -= font->height;
//        black.a *= .9;
//        texture_font_load_glyphs(font, text);
//        add_text(buffer, font, text, &black, &pen);
//        texture_font_delete(font);
//    }
//
//    glGenTextures(1, &atlas->id);
//    glBindTexture(GL_TEXTURE_2D, atlas->id);
//    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlas->width, atlas->height,
//        0, GL_RED, GL_UNSIGNED_BYTE, atlas->data);
//
//    textShader = shader_load("Content/FreeType-GL/v3f-t2f-c4f.vert", "Content/FreeType-GL/v3f-t2f-c4f.frag");
//
//    mat4_set_identity(&projection);
//    mat4_set_identity(&model);
//    mat4_set_identity(&view);
//}

/* program entry */
int main(int argc, char *argv[])
{
    OpenGLViewport viewport(500, 500);
    //GLFWwindow* window;
    //int width, height;

    //if(!glfwInit())
    //{
    //    fprintf( stderr, "Failed to initialize GLFW\n" );
    //    exit( EXIT_FAILURE );
    //}

    //glfwWindowHint(GLFW_DEPTH_BITS, 16);
    //glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    //glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

    //window = glfwCreateWindow(900, 900, "Gleam - Hello World", NULL, NULL);
    //if (!window)
    //{
    //    fprintf(stderr, "Failed to open GLFW window\n" );
    //    glfwTerminate();
    //    exit(EXIT_FAILURE);
    //}

    //// Set callback functions
    //glfwSetFramebufferSizeCallback(window, reshape);
    //glfwSetKeyCallback(window, key);

    //glfwMakeContextCurrent(window);

    //glfwSwapInterval( 1 );

    //glfwGetFramebufferSize(window, &width, &height);

    //glewInit();

    //// Parse command-line options
    //init();
    //reshape(window, width, height);

    //// Main loop
    //while( !glfwWindowShouldClose(window) )
    //{
    //    // Draw gears
    //    draw();

    //    // Swap buffers
    //    glfwSwapBuffers(window);
    //    glfwPollEvents();
    //}

    //glDeleteTextures(1, &atlas->id);
    //atlas->id = 0;
    //texture_atlas_delete(atlas);

    //// Terminate GLFW
    //glfwTerminate();

    //// Exit program
    //exit( EXIT_SUCCESS );
}
