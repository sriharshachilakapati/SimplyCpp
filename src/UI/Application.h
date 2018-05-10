#pragma once

#include "../stdafx.h"
#include "MainFrame.h"

#include "../Core/Config.h"

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
