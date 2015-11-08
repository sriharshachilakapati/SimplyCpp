#pragma once
#include "stdafx.h"

class EditorWidget : public wxStyledTextCtrl
{
public:
    EditorWidget(wxWindow* window);
    EditorWidget(wxWindow* window, const wxString& code);

    bool CodeChanged();
    bool SavedOnce();

    void LoadFile(const wxString& file, int fileType = 0);
    void SaveFile();
    void SaveFile(const wxString& file, int fileType = 0);

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
