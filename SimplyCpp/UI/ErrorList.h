#pragma once
#include "../stdafx.h"

namespace SimplyCpp
{
    namespace UI
    {
        enum ListID;

        class ErrorList : public wxListCtrl
        {
        public:
            ErrorList(wxWindow* parent);

            void Clear();
            void Insert(const wxString& file, int line, ListID type, const wxString& message);
        };

        enum ListID
        {
            FILE,
            LINE,
            TYPE,
            DESCRIPTION
        };
    }
}