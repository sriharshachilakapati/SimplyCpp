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
    SetMarginWidth(1, 10);
    SetScrollWidth(100);
    SetScrollWidthTracking(true);

    SetLexer(wxSTC_LEX_CPP);
    SetStyleBits(5);
    SetTabWidth(4);
    SetUseTabs(false);
    SetIndent(4);

    SetTabIndents(true);
    SetBackSpaceUnIndents(true);

    SetCaretLineVisible(true);
    SetCaretLineBackground(wxColor(250, 250, 255));

    // Code folding on comments, and between blocks
    SetMarginType(2, wxSTC_MARGIN_SYMBOL);
    SetMarginWidth(2, 20);
    SetMarginMask(2, wxSTC_MASK_FOLDERS);
    StyleSetBackground(2, wxColor(200, 200, 200));
    SetMarginSensitive(2, true);

    SetProperty("fold", "2");
    SetProperty("fold.comment", "2");
    SetProperty("fold.compact", "2");

    for (int i = 25; i <= 31; i++)
    {
        MarkerSetForeground(i, wxColor(255, 255, 255));
        MarkerSetBackground(i, wxColor(200, 200, 200));
    }

    MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS);
    MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS);
    MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUSCONNECTED);
    MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_TCORNER);
    MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUSCONNECTED);
    MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_VLINE);
    MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_LCORNER);

    // Indentation guide
    StyleSetBackground(wxSTC_STYLE_INDENTGUIDE, wxColor(240, 240, 240));
    SetIndentationGuides(wxSTC_IV_LOOKBOTH);

    // Fonts and styling
    wxFont monospace(11, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Consolas");
    StyleSetFont(wxSTC_STYLE_DEFAULT, monospace);
    StyleSetFont(wxSTC_C_WORD, monospace);

    StyleClearAll();

    static const wxString keywords =
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
        "wchar_t while override final";

    static const wxString keywords1 =
        // Extended
        "__asm __asume __based __box __cdecl __declspec "
        "__delegate delegate depreciated dllexport dllimport "
        "event __event __except __fastcall __finally __forceinline "
        "__int8 __int16 __int32 __int64 __int128 __interface "
        "interface __leave naked noinline __noop noreturn "
        "nothrow novtable nullptr safecast __stdcall "
        "__try __except __finally __unaligned uuid __uuidof "
        "__virtual_inheritance";

    SetKeyWords(0, keywords);
    SetKeyWords(1, keywords1);

    StyleSetForeground(wxSTC_C_COMMENT, wxColor(_("#008000")));
    StyleSetForeground(wxSTC_C_COMMENTLINE, wxColor(_("#008000")));
    StyleSetForeground(wxSTC_C_WORD, wxColor(_("#0000F0")));
    StyleSetForeground(wxSTC_C_WORD2, wxColor(_("#008080")));
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
    StyleSetItalic(wxSTC_C_WORD2, true);
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

void EditorWidget::OnMarginClick(wxStyledTextEvent& e)
{
    if (e.GetMargin() == 2)
    {
        int lineClick = LineFromPosition(e.GetPosition());
        int levelClick = GetFoldLevel(lineClick);

        if ((levelClick & wxSTC_FOLDLEVELHEADERFLAG) > 0)
            ToggleFold(lineClick);
    }
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
    EVT_STC_MARGINCLICK(wxID_ANY, EditorWidget::OnMarginClick)
END_EVENT_TABLE()