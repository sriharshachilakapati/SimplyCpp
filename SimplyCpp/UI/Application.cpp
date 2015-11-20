#pragma once
#include "../stdafx.h"
#include "Application.h"

using namespace SimplyCpp::UI;
using namespace SimplyCpp::Core;

bool Application::OnInit()
{
    MainFrame* frame = new MainFrame();
    frame->Show(true);
    return true;
}

IMPLEMENT_APP(Application)