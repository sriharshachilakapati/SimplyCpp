#include "../stdafx.h"
#include "Application.h"

using namespace SimplyCpp::UI;
using namespace SimplyCpp::Core;

bool Application::OnInit()
{
    AppConfig = new wxConfig(_("SimplyCpp-2.0.1"));
    MainFrame* frame = new MainFrame();
    frame->Show(true);
    return true;
}

IMPLEMENT_APP(Application)
