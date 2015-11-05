#pragma once
#include "stdafx.h"
#include "Application.h"

bool Application::OnInit()
{
    MainFrame* frame = new MainFrame();
    frame->Show(true);
    return true;
}

IMPLEMENT_APP(Application)