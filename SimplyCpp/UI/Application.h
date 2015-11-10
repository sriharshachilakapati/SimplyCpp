#pragma once

#include "../stdafx.h"
#include "MainFrame.h"

namespace SimplyCpp
{
    namespace UI
    {
        class Application : public wxApp
        {
        public:
            virtual bool OnInit();
        };

        DECLARE_APP(Application)
    }
}