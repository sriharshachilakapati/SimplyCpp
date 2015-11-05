#include "stdafx.h"
#include "EditorWidget.h"

EditorWidget::EditorWidget(wxWindow* window) : wxStyledTextCtrl(window, wxID_ANY)
{
    InitEditor();
    m_sOriginalCode = GetText();
}

EditorWidget::EditorWidget(wxWindow* window, const wxString& code) : wxStyledTextCtrl(window, wxID_ANY)
{
    InitEditor();
    SetText(code);
    m_sOriginalCode = code;
}

void EditorWidget::InitEditor()
{
    m_sFileName = _("");

    SetMarginWidth(0, 20);
    SetMarginWidth(1, 20);
    SetScrollWidth(100);
    SetScrollWidthTracking(true);

    SetLexer(wxSTC_LEX_CPP);
    SetStyleBits(5);
    SetTabWidth(4);
    SetUseTabs(false);
    SetIndent(4);

    SetTabIndents(true);
    SetBackSpaceUnIndents(true);

    //scintilla.AutomaticFold = (AutomaticFold.Show | AutomaticFold.Click | AutomaticFold.Change);

    wxFont monospace(11, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, _("Consolas"));
    StyleSetFont(wxSTC_STYLE_DEFAULT, monospace);
    StyleSetFont(wxSTC_C_WORD, monospace);
    SetCaretLineBackground(wxColor(0, 0, 255));

    StyleClearAll();

    static const wxString keywords = _(
        // Standard
        "asm auto bool break case catch char class const "
        "const_cast continue default delete do double "
        "dynamic_cast else enum explicit extern false finally "
        "float for friend goto if inline int long mutable "
        "namespace new operator private protected public "
        "register reinterpret_cast register return short signed "
        "sizeof static static_cast struct switch template "
        "this throw true try typedef typeid typename "
        "union unsigned using virtual void volatile "
        "wchar_t while "

        // Extended
        "__asm __asume __based __box __cdecl __declspec "
        "__delegate delegate depreciated dllexport dllimport "
        "event __event __except __fastcall __finally __forceinline "
        "__int8 __int16 __int32 __int64 __int128 __interface "
        "interface __leave naked noinline __noop noreturn "
        "nothrow novtable nullptr safecast __stdcall "
        "__try __except __finally __unaligned uuid __uuidof "
        "__virtual_inheritance"
    );

    SetKeyWords(0, keywords);

    StyleSetForeground(wxSTC_C_COMMENT, wxColor(_("#008000")));
    StyleSetForeground(wxSTC_C_COMMENTLINE, wxColor(_("#008000")));
    StyleSetForeground(wxSTC_C_WORD, wxColor(_("#0000F0")));
    StyleSetForeground(wxSTC_C_IDENTIFIER, wxColor(_("#5c2699")));
    StyleSetForeground(wxSTC_C_STRING, wxColor("#800000"));
    StyleSetForeground(wxSTC_C_STRINGEOL, wxColor("#800000"));
    StyleSetForeground(wxSTC_C_STRINGRAW, wxColor(_("#800000")));
    StyleSetForeground(wxSTC_C_CHARACTER, wxColor(_("#800000")));
    StyleSetForeground(wxSTC_C_NUMBER, wxColor(_("#660E7A")));
    StyleSetForeground(wxSTC_C_PREPROCESSOR, wxColor(_("#643820")));
    StyleSetForeground(wxSTC_C_PREPROCESSORCOMMENT, wxColor(_("#808080")));
    StyleSetForeground(wxSTC_C_COMMENTDOC, wxColor(_("#808080")));
    StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORD, wxColor(_("#808080")));
    StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORDERROR, wxColor(_("#808080")));

    StyleSetBold(wxSTC_C_COMMENTDOCKEYWORD, true);
    StyleSetBold(wxSTC_C_COMMENTDOCKEYWORDERROR, true);
    StyleSetUnderline(wxSTC_C_COMMENTDOCKEYWORDERROR, true);
}

void EditorWidget::OnCharAdded(wxStyledTextEvent& e)
{
    const wxChar& chr = e.GetKey();
    const int currentLine = GetCurrentLine();

    if (chr == '\n' || chr == '\r')
    {
        int lineInd = 0;

        if (currentLine > 0)
        {
            lineInd = GetLineIndentation(currentLine - 1);

            if (GetLine(currentLine - 1).Contains("{"))
                lineInd += GetIndent();
        }

        if (lineInd != 0)
        {
            SetLineIndentation(currentLine, lineInd);
            GotoPos(PositionFromLine(currentLine) + lineInd);
        }
    }

    else if (chr == '}')
        SetLineIndentation(currentLine, GetLineIndentation(currentLine) - GetIndent());
}

void EditorWidget::OnTextChange(wxStyledTextEvent& WXUNUSED(e))
{
    int numLines = this->GetLineCount();

    if (m_nMaxLines == numLines)
        return;

    m_nMaxLines = numLines;
    const int padding = 15;

    SetMarginWidth(0, TextWidth(MarginGetStyle(0), wxString::Format("%d", numLines)) + padding);
}

bool EditorWidget::CodeChanged()
{
    return m_sOriginalCode.compare(GetText()) != 0;
}

bool EditorWidget::SavedOnce()
{
    return !m_sFileName.IsEmpty();
}

void EditorWidget::LoadFile(const wxString& file, int fileType)
{
    wxTextAreaBase::LoadFile(file, fileType);
    m_sOriginalCode = GetText();
    m_sFileName = file;
}

void EditorWidget::SaveFile()
{
    SaveFile(m_sFileName);
}

void EditorWidget::SaveFile(const wxString& file, int fileType)
{
    wxTextAreaBase::SaveFile(file, fileType);
    m_sOriginalCode = GetText();
    m_sFileName = file;
}

BEGIN_EVENT_TABLE(EditorWidget, wxStyledTextCtrl)
    EVT_STC_CHANGE(wxID_ANY, EditorWidget::OnTextChange)
    EVT_STC_CHARADDED(wxID_ANY, EditorWidget::OnCharAdded)
END_EVENT_TABLE()