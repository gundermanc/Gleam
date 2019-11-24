#include "OpenGLViewport/OpenGLViewport.h"

/* program entry */
int main(int argc, char *argv[])
{
    OpenGLViewport viewport("Gleam - hello world", 500, 500);

    viewport.BeginEventLoop();

    return 0;
}
