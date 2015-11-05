#pragma once

#include "stdafx.h"
#include "MainFrame.h"

class Application : public wxApp
{
public:
    virtual bool OnInit();
};

DECLARE_APP(Application)