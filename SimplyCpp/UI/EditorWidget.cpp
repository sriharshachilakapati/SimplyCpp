#include "../stdafx.h"
#include "EditorWidget.h"

using namespace SimplyCpp::UI;

EditorWidget::EditorWidget(wxWindow* window) : wxStyledTextCtrl(window, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER)
{
    InitEditor();
    m_sOriginalCode = GetText();
}

void EditorWidget::InitEditor()
{
    SetCodePage(wxSTC_CP_UTF8);

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
    SetProperty("styling.within.preprocessor", "1");

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
        // Standard C++14 (http://en.cppreference.com/w/cpp/keyword)
        "alignas alignof and and_eq asm auto bitand bitor bool break case catch char char16_t char32_t class "
        "compl concept const constexpr const_cast continue decltype default delete do double dynamic_cast "
        "else enum explicit export extern false float for friend goto if inline int long mutable namespace "
        "new noexcept not not_eq nullptr operator or or_eq private protected public register reinterpret_cast "
        "requires return short signed sizeof static static_assert static_cast struct switch template this "
        "thread_local throw true try typedef typeid typename union unsigned using virtual void volatile "
        "wchar_t while xor xor_eq";

    static const wxString keywords1 =
        // Extended Microsoft extensions (https://msdn.microsoft.com/en-us/library/2e6a4at9.aspx)
        "__abstract __alignof __asm __assume __based __box __cdecl __declspec __delegate __event __except "
        "__fastcall __finally __forceinline __gc __hook __identifier __if_exists __if_not_exists __inline "
        "__int16 __int32 __int64 __int8 __interface __leave __m128 __m128d __m128i __m64 "
        "__multiple_inheritance __nogc __noop __pin __property __raise __sealed __single_inheritance "
        "__stdcall __super __thiscall __try __except __finally __try_cast __unaligned __unhook __uuidof "
        "__value __virtual_inheritance __w64 __wchar_t "

        // Other extended keywords (http://docwiki.appmethod.com/appmethod/1.17/topics/en/Keywords,_Alphabetical_Listing_Index)
        "_asm __automated axiom _Bool cdecl _cdecl __classid __classmethod __closure _Complex concept_map "
        "__declspec(allocate(\"SEGNAME\")) __declspec(delphiclass) __declspec(delphirecord) "
        "__declspec(delphireturn) __declspec(delphirtti) __declspec(dllexport) __declspec(dllimport) "
        "__declspec(dynamic) __declspec(hidesbase) __declspec(naked) __declspec(noreturn) __declspec(nothrow) "
        "__declspec(novtable) __declspec(package) __declspec(pascalimplementation) __declspec(property) "
        "__declspec(selectany) __declspec(thread) __declspec(uuid(\"ComObjectGUID\")) __delphirtti __dispid "
        "__emit__ _export __export _fastcall _Imaginary _import __import late_check __msfastcall __msreturn "
        "_pascal __pascal __published __rtti _stdcall __thread __uuidof uuidof";

    SetKeyWords(0, keywords);
    SetKeyWords(1, keywords1);

    StyleSetForeground(wxSTC_C_GLOBALCLASS, wxColor(_("#ef0000")));
    StyleSetForeground(wxSTC_C_COMMENT, wxColor(_("#008000")));
    StyleSetForeground(wxSTC_C_COMMENTLINE, wxColor(_("#008000")));
    StyleSetForeground(wxSTC_C_WORD, wxColor(_("#0040FE")));
    StyleSetForeground(wxSTC_C_WORD2, wxColor(_("#008080")));
    StyleSetForeground(wxSTC_C_IDENTIFIER, wxColor(_("#000050")));
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
    StyleSetForeground(wxSTC_C_OPERATOR, wxColor(_("#000000")));

    StyleSetBold(wxSTC_C_COMMENTDOCKEYWORD, true);
    StyleSetBold(wxSTC_C_COMMENTDOCKEYWORDERROR, true);
    StyleSetItalic(wxSTC_C_WORD2, true);
    StyleSetUnderline(wxSTC_C_COMMENTDOCKEYWORDERROR, true);

    // Brace highlighting
    StyleSetBackground(wxSTC_STYLE_BRACELIGHT, wxColor(230, 230, 230));
    StyleSetForeground(wxSTC_STYLE_BRACELIGHT, wxColor(0, 100, 100));
    StyleSetForeground(wxSTC_STYLE_BRACEBAD, wxColor(200, 0, 0));
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

void EditorWidget::OnUpdateUI(wxStyledTextEvent& e)
{
    static int lastCaretPos = 0;
    int caretPos = GetCurrentPos();

    if (lastCaretPos != caretPos)
    {
        lastCaretPos = caretPos;

        int bracePos1 = -1;
        int bracePos2 = -1;

        // Is there a brace to the left or right?
        if (caretPos > 0 && IsBrace(GetCharAt(caretPos - 1)))
            bracePos1 = (caretPos - 1);
        else if (IsBrace(GetCharAt(caretPos)))
            bracePos1 = caretPos;

        if (bracePos1 >= 0)
        {
            // Find the matching brace
            bracePos2 = BraceMatch(bracePos1);

            if (bracePos2 == wxSTC_INVALID_POSITION)
                BraceBadLight(bracePos1);
            else
                BraceHighlight(bracePos1, bracePos2);
        }
        else
            // Turn off brace matching
            BraceHighlight(wxSTC_INVALID_POSITION, wxSTC_INVALID_POSITION);
    }

    OnTextChange(e);
}

void EditorWidget::OnTextChange(wxStyledTextEvent& WXUNUSED(e))
{
    int numLines = this->GetLineCount();

    if (m_nMaxLines == numLines)
        return;

    m_nMaxLines = numLines;

    const int padding = 15;
    const int zoom = GetZoom() * 2;

    SetMarginWidth(0, std::max(zoom, 0) + TextWidth(MarginGetStyle(0), wxString::Format("%d", numLines)) + padding);
}

bool EditorWidget::CodeChanged()
{
    return m_sOriginalCode.compare(GetText()) != 0;
}

bool EditorWidget::SavedOnce()
{
    return !m_sFileName.IsEmpty();
}

void EditorWidget::LoadFile(const wxString& file)
{
    wxFileInputStream fin(file);
    wxTextInputStream in(fin, _("\x09"), wxConvUTF8);

    while (fin.IsOk() && !fin.Eof())
    {
        wxString line = in.ReadLine();

        SetText(GetText() + line + (fin.Eof() ? "" : "\n"));
    }

    wxString text = GetText();

    text = wxString(text);
    text.Replace("\r", "");

    SetText(text);

    m_sOriginalCode = text;
    m_sFileName = file;
}

void EditorWidget::SaveFile()
{
    SaveFile(m_sFileName);
}

void EditorWidget::SaveFile(const wxString& file)
{
    wxFileOutputStream fout(file);
    wxTextOutputStream out(fout, wxEOL_NATIVE, wxConvUTF8);

    for (int i = 0; i < GetNumberOfLines(); i++)
    {
        wxString line = GetLine(i);

        line.Replace("\r", "");
        line.Replace("\n", "");

        out << line << ((i == GetNumberOfLines() - 1) ? "" : "\n");
    }

    out.Flush();
    fout.Close();

    SetSavePoint();

    m_sOriginalCode = GetText();
    m_sFileName = file;
}

BEGIN_EVENT_TABLE(EditorWidget, wxStyledTextCtrl)
    EVT_STC_CHANGE(wxID_ANY, EditorWidget::OnTextChange)
    EVT_STC_CHARADDED(wxID_ANY, EditorWidget::OnCharAdded)
    EVT_STC_MARGINCLICK(wxID_ANY, EditorWidget::OnMarginClick)
    EVT_STC_UPDATEUI(wxID_ANY, EditorWidget::OnUpdateUI)
END_EVENT_TABLE()