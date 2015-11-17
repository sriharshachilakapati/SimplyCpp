#pragma once
#include "../stdafx.h"

namespace SimplyCpp
{
    namespace UI
    {
        class EditorWidget : public wxStyledTextCtrl
        {
        public:
            EditorWidget(wxWindow* window);

            bool CodeChanged();
            bool SavedOnce();

            void LoadFile(const wxString& file);
            void SaveFile();
            void SaveFile(const wxString& file);

            wxString GetFileName() const
            {
                return m_sFileName;
            }

        private:
            void InitEditor();
            void OnTextChange(wxStyledTextEvent& e);
            void OnCharAdded(wxStyledTextEvent& e);
            void OnMarginClick(wxStyledTextEvent& e);
            void OnUpdateUI(wxStyledTextEvent& e);

            inline bool IsBrace(const int& c)
            {
                switch (c)
                {
                case '(':
                case ')':
                case '[':
                case ']':
                case '{':
                case '}':
                case '<':
                case '>':
                    return true;
                }

                return false;
            }

            int m_nMaxLines;

            wxString m_sOriginalCode;
            wxString m_sFileName;

            DECLARE_EVENT_TABLE()
        };
    }
}