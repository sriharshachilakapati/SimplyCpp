#pragma once

#include "stdafx.h"
#include "MainFrame.h"

class Application : public wxApp
{
public:
    virtual bool OnInit();
};

wxDECLARE_APP(Application);
wxIMPLEMENT_APP(Application);