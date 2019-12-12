#include "OpenGLViewport/OpenGLViewport.h"
#include "App/CommandLineHelpView.h"
#include "App/DocumentView.h"
#include "Utilities/FileLogger.h"

namespace
{
    const char* LogFileName = "Gleam.log";
}

/* program entry */
int main(int argc, char *argv[])
{
    // Create logger. For now, log everything.
    // TODO: log only critical and add verbosity command line switches.
    FileLogger logger(LogFileName, LogSeverity::Information);

    logger.Log(LogSeverity::Information, "Creating viewport...");

    // Create viewport.
    OpenGLViewport viewport("Gleam", 700, 500);

    logger.Log(LogSeverity::Information, "Evaluating command line arguments...");

    // Show command line help if no argument was provided.
    if (argc <= 1)
    {
        logger.Log(LogSeverity::Information, "Too few arguments, displaying command line help.");
        ShowCommandLineHelpView(viewport);
    }
    else
    {
        // Otherwise, show a document.
        logger.Log(LogSeverity::Information, "Document name given. Switching to document view.");
        ShowDocumentView(viewport, argv[1]);
    }

    logger.Log(LogSeverity::Information, "Beginning OpenGL event loop...");

    // Create window and start handling events.
    viewport.BeginEventLoop();

    return 0;
}
