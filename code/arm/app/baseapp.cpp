#include "baseapp.h"
#include "task.h"
int16 BaseApp::Heart()
{
    g_task->Heart ( Id );
    return 1;
}
