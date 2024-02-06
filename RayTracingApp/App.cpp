#include "App.h"

App::App()
{
    frame = new MainFrame(size);
}

bool App::OnInit()
{
    frame->Show();

    return true;
}
