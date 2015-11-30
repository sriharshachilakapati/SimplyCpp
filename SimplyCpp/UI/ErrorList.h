#pragma once
#include "../stdafx.h"

namespace SimplyCpp
{
    namespace UI
    {
        enum ListID;
        enum ErrorType;

        class ErrorList : public wxListCtrl
        {
        public:
            ErrorList(wxWindow* parent);

            void Clear();
            void Insert(const wxString& file, int line, ErrorType type, const wxString& message);
        };

        enum ListID
        {
            FILE,
            LINE,
            TYPE,
            DESCRIPTION
        };

        enum ErrorType
        {
            TYPE_WARNING,
            TYPE_ERROR,
            TYPE_FATAL
        };
    }
}