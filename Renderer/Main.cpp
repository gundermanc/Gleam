#include "OpenGLViewport/OpenGLViewport.h"
#include "App/CommandLineHelpView.h"
#include "App/DocumentView.h"

/* program entry */
int main(int argc, char *argv[])
{
    // Create viewport.
    OpenGLViewport viewport("Gleam", 700, 500);

    // Show command line help if no argument was provided.
    if (argc <= 1)
    {
        ShowCommandLineHelpView(viewport);
    }
    else
    {
        // Otherwise, show a document.
        ShowDocumentView(viewport, argv[1]);
    }

    // Create window and start handling events.
    viewport.BeginEventLoop();

    return 0;
}
