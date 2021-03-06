#include "../stdafx.h"
#include "MainFrame.h"
#include "Utils.h"

using namespace SimplyCpp::UI;
using namespace SimplyCpp::Core;

MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, _T("SimplyCpp"), wxDefaultPosition, wxSize(800, 600))
{
    #ifndef __WXMAC__
    SetIcon(wxICON(ID_ICON));
    #endif

    m_mgr.SetManagedWindow(this);

    CreateMenuBar();
    CreateStatusBar(2);

    wxTextCtrl* text2 = new wxTextCtrl(this, wxID_ANY, _T(""),
        wxDefaultPosition, wxSize(200, 150),
        wxNO_BORDER);

    TerminalWidget* terminal = new TerminalWidget(this);
    ErrorList* errorList = new ErrorList(this);

    m_notebook = new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_MIDDLE_CLICK_CLOSE);
    m_notebook->AddPage(new EditorWidget(this), _("Untitled"));

    text2->SetEditable(false);

    ProjectExplorer* pExplorer = new ProjectExplorer(this);

    m_mgr.AddPane(pExplorer, wxAuiPaneInfo().Name(_("pane_project")).Caption(_("Project Explorer")).Left());
    m_mgr.AddPane(text2, wxAuiPaneInfo().Name(_("pane_props")).Caption(_("Properties")).Right());
    m_mgr.AddPane(terminal, wxAuiPaneInfo().Name(_("pane_output")).Caption(_("Output")).Bottom().MaximizeButton(true));
    m_mgr.AddPane(errorList, wxAuiPaneInfo().Name(_("pane_errors")).Caption(_("Errors")).Layer(0).Bottom());
    m_mgr.AddPane(m_notebook, wxAuiPaneInfo().Name(_("pane_notebook")).CaptionVisible(false).Center().PaneBorder(false));

    m_notebook->SetFocus();

    m_mgr.Update();

    SetStatusText(_("SimplyCpp 2.0.1 is ready"), 0);

    wxString message = _("Written by Sri Harsha Chilakapati");
    int widths[2];
    widths[0] = -1;
    widths[1] = GetTextExtent(message).GetWidth();

    SetStatusWidths(2, widths);
    SetStatusText(message, 1);

    if (AppConfig->Exists(_("Layer.Perspective")))
    {
        wxString str = AppConfig->Read(_("Layer.Perspective"));
        m_mgr.LoadPerspective(str);
    }

    CenterOnScreen();
    Maximize();
}

MainFrame::~MainFrame()
{
    // Write to AppConfig
    AppConfig->Write(_("Layer.Perspective"), m_mgr.SavePerspective());
    AppConfig->Flush();

    m_mgr.UnInit();
}

void SimplyCpp::UI::MainFrame::ParseCompilerOutput()
{
    ErrorList* errorsList = static_cast<ErrorList*>(m_mgr.GetPane("pane_errors").window);
    errorsList->Clear();

    TerminalWidget* output = static_cast<TerminalWidget*>(m_mgr.GetPane("pane_output").window);

    // The regex for fatals is (.*):(\d):(\d):\sfatal\\serror:\s(.*)
    // The regex for errors is (.*):(\d):(\d):\serror:\s(.*)
    // The regex for warnings is (.*):(\d):(\d):\swarning:\s(.*)
    wxRegEx error("(.*):(\\d*):(\\d*):\\serror:\\s(.*)", wxRE_ADVANCED + wxRE_ICASE);
    wxRegEx fatal("(.*):(\\d*):(\\d*):\\sfatal\\serror:\\s(.*)", wxRE_ADVANCED + wxRE_ICASE);
    wxRegEx warning("(.*):(\\d*):(\\d*):\\swarning:\\s(.*)", wxRE_ADVANCED + wxRE_ICASE);

    // Insert fatal errors first into the list
    for (int i = 0; i < output->GetNumOutputLines(); i++)
    {
        wxString line = output->GetOutputLine(i);

        if (fatal.Matches(line))
        {
            wxString fileName = fatal.GetMatch(line, 1);
            int lineNo = wxAtoi(fatal.GetMatch(line, 2));
            wxString message = fatal.GetMatch(line, 4);

            errorsList->Insert(fileName, lineNo, ErrorType::TYPE_FATAL, message);
        }
    }

    // Insert normal errors next
    for (int i = 0; i < output->GetNumOutputLines(); i++)
    {
        wxString line = output->GetOutputLine(i);

        if (error.Matches(line))
        {
            wxString fileName = error.GetMatch(line, 1);
            int lineNo = wxAtoi(error.GetMatch(line, 2));
            wxString message = error.GetMatch(line, 4);

            errorsList->Insert(fileName, lineNo, ErrorType::TYPE_ERROR, message);
        }
    }

    // Finally let the warnings get into the list
    for (int i = 0; i < output->GetNumOutputLines(); i++)
    {
        wxString line = output->GetOutputLine(i);

        if (warning.Matches(line))
        {
            wxString fileName = warning.GetMatch(line, 1);
            int lineNo = wxAtoi(warning.GetMatch(line, 2));
            wxString message = warning.GetMatch(line, 4);

            errorsList->Insert(fileName, lineNo, ErrorType::TYPE_WARNING, message);
        }
    }
}

void MainFrame::CreateMenuBar()
{
    wxMenuBar* menuBar = new wxMenuBar();

    wxMenu* menuFile = new wxMenu();
    wxMenu* menuEdit = new wxMenu();
    wxMenu* menuProject = new wxMenu();
    wxMenu* menuCompile = new wxMenu();
    wxMenu* menuWindow = new wxMenu();
    wxMenu* menuHelp = new wxMenu();

    menuFile->Append(ID_NEW, _("New\tCtrl-N"), _("Create a new file"));
    menuFile->Append(ID_OPEN, _("Open\tCtrl-O"), _("Open an existing file into the IDE"));
    menuFile->Append(ID_SAVE, _("Save\tCtrl-S"), _("Save the current file"));
    menuFile->Append(ID_SAVE_AS, _("Save As\tCtrl-Shift-S"), _("Save a copy of the current file"));
    menuFile->AppendSeparator();
    menuFile->Append(ID_CLOSE, _("Close\tCtrl-W"), _("Close the current file"));
    menuFile->Append(ID_EXIT, _("Exit\tAlt-F4"), _("Exit the IDE"));

    menuEdit->Append(ID_COPY, _("Copy\tCtrl-C"), _("Copy the selected content"));
    menuEdit->Append(ID_CUT, _("Cut\tCtrl-X"), _("Cut the selected content"));
    menuEdit->Append(ID_PASTE, _("Paste\tCtrl-V"), _("Paste the copied content"));
    menuEdit->AppendSeparator();
    menuEdit->Append(ID_SELECT_ALL, _("Select All\tCtrl-A"), _("Select all the content"));

    menuCompile->Append(ID_COMPILE, _("&Compile\tAlt-F9"), _("Compile the current file"));
    menuCompile->Append(ID_RUN, _("&Run\tCtrl-F9"), _("Run the program"));
    menuCompile->Append(ID_COMPILE_AND_RUN, _("&Compile and Run\tF9"), _("Compile and Run the program"));

    menuWindow->Append(ID_PROJECT_EXPLORER, "Project Explorer", "Open the project explorer pane");
    menuWindow->Append(ID_PROPERTIES, "File Properties", "See the properties of the current file");
    menuWindow->Append(ID_OUTPUT, "Output", "Open the output pane");
    menuWindow->Append(ID_TERMINAL, "Terminal", "Open a terminal in output pane");

    menuBar->Append(menuFile, _("&File"));
    menuBar->Append(menuEdit, _("&Edit"));
    menuBar->Append(menuProject, _("&Project"));
    menuBar->Append(menuCompile, _("&Compile"));
    menuBar->Append(menuWindow, _("&Window"));
    menuBar->Append(menuHelp, _("&Help"));

    this->SetMenuBar(menuBar);

    // Will re-enable this in future
    menuBar->EnableTop(2, false);
}

void MainFrame::OnEditorClose(wxAuiNotebookEvent& e)
{
    EditorWidget* editorPage = static_cast<EditorWidget*>(m_notebook->GetPage(e.GetSelection()));

    if (!editorPage->SavedOnce() || editorPage->CodeChanged())
    {
        e.Veto();
        
        wxMessageDialog* dialog = new wxMessageDialog(this,
            wxString::Format(_("Do you want to save %s? If not, all your work will be lost!"),
                m_notebook->GetPageText(e.GetSelection())), _("SimplyCpp"), wxYES_NO | wxCANCEL);
        
        SimplyCpp::UI::ShowWindowModalThenDo(dialog, [this, dialog, e, editorPage] (int returnCode)
        {
            dialog->Destroy();
            
            switch (returnCode)
            {
                case wxID_YES:
                {
                    wxCommandEvent nullEvent;
                    int selection = e.GetSelection();
                    
                    OnMenuSave(nullEvent, [this, selection, editorPage]()
                    {
                        // Only close the editor if the file is saved. If not, then the user must have cancelled the
                        // save from the save dialog box. In that case, just do nothing.
                        if (!editorPage->CodeChanged())
                        {
                            m_notebook->DeletePage(selection);
//                            m_notebook->RemovePage(selection);
                        }
                    });
                    break;
                }
                    
                case wxID_NO:
                    m_notebook->DeletePage(e.GetSelection());
//                    m_notebook->RemovePage(e.GetSelection());
                    break;
            }
        });
    }
}

void MainFrame::OnWindowClose(wxCloseEvent& e)
{
    e.Veto();
    
    wxCommandEvent nullEvent;
    OnMenuExit(nullEvent);
}

void MainFrame::OnMenuNew(wxCommandEvent& WXUNUSED(e))
{
    m_notebook->AddPage(new EditorWidget(this),
        wxString::Format(_("Untitled%d"), static_cast<int>(m_notebook->GetPageCount() + 1)));
}

void MainFrame::OnMenuOpen(wxCommandEvent& WXUNUSED(e))
{
    wxFileDialog* dialog = new wxFileDialog(this, FILE_SELECTOR_PROMPT_STRING, wxEmptyString, wxEmptyString,
        _("C++ Source Files (*.cpp, *.cxx)|*.cpp;*.cxx|C Source files(*.c)|*.c|C header files(*.h)|*.h"), wxFD_OPEN);
    
    SimplyCpp::UI::ShowWindowModalThenDo(dialog, [this, dialog] (int returnCode)
    {
        if (returnCode == wxID_OK)
        {
            wxString file = dialog->GetFilename();
            wxString path = dialog->GetPath();
            
            if (m_notebook->GetPageCount() != 0)
            {
                EditorWidget* currentEditor = static_cast<EditorWidget*>(m_notebook->GetCurrentPage());
                
                if (!currentEditor->CodeChanged() && !currentEditor->GetText().compare(_("")))
                {
                    currentEditor->LoadFile(path);
                    m_notebook->SetPageText(m_notebook->GetPageIndex(m_notebook->GetCurrentPage()), file);
                    m_mgr.Update();
                    return;
                }
            }
            
            EditorWidget* editor = new EditorWidget(this);
            editor->LoadFile(path);
            
            m_notebook->AddPage(editor, file, true);
            
            ProjectExplorer* project_explorer = dynamic_cast<ProjectExplorer*>(m_mgr.GetPane("pane_project").window);
            
            Project project;
            project.SetName(dialog->GetFilename().ToStdString());
            
            OpenProjects.push_back(project);
            project_explorer->AppendItem(project_explorer->GetRootItem(), file);
            project_explorer->Update();
            
            m_mgr.Update();
        }
        
        dialog->Destroy();
    });
}

void MainFrame::OnMenuSave(wxCommandEvent& e, const Callback& callback)
{
    if (m_notebook->GetPageCount() == 0)
        return;

    EditorWidget* currentEditor = static_cast<EditorWidget*>(m_notebook->GetCurrentPage());

    if (currentEditor->SavedOnce())
        currentEditor->SaveFile();
    else
        OnMenuSaveAs(e, callback);
}

void MainFrame::OnMenuSaveAs(wxCommandEvent& WXUNUSED(e), const Callback& callback)
{
    wxFileDialog* dialog = new wxFileDialog(this, FILE_SELECTOR_PROMPT_STRING, wxEmptyString, wxEmptyString,
        _("C++ Source Files(*.cpp, *.cxx)|*.cpp;*.cxx|C Source files(*.c)|*.c|C header files(*.h)|*.h"), wxFD_SAVE);

    SimplyCpp::UI::ShowWindowModalThenDo(dialog, [this, dialog, callback] (int returnCode)
    {
        if (returnCode == wxID_OK)
        {
            wxString file = dialog->GetFilename();
            wxString path = dialog->GetPath();

            EditorWidget* currentEditor = static_cast<EditorWidget*>(m_notebook->GetCurrentPage());
            currentEditor->SaveFile(path);

            m_notebook->SetPageText(m_notebook->GetPageIndex(currentEditor), file);
            m_mgr.Update();
            
            callback();
        }

        dialog->Destroy();
    });
}

void MainFrame::OnMenuClose(wxCommandEvent& WXUNUSED(e))
{
    wxAuiNotebookEvent nullEvent;
    nullEvent.SetSelection(m_notebook->GetSelection());
    OnEditorClose(nullEvent);
}

void MainFrame::OnMenuExit(wxCommandEvent& e)
{
    auto exitHandler = [this]()
    {
        static_cast<TerminalWidget*>(m_mgr.GetPane("pane_output").window)->TerminateProcess();
        Destroy();
    };
    
    int unsavedEditors = 0;
    
    for (unsigned int i = 0; i < m_notebook->GetPageCount(); i++)
    {
        EditorWidget* editorPage = static_cast<EditorWidget*>(m_notebook->GetPage(i));

        if (!editorPage->SavedOnce() || editorPage->CodeChanged())
            unsavedEditors++;
    }
    
    if (unsavedEditors > 0)
    {
        wxMessageDialog* dialog = new wxMessageDialog(this,
              wxString::Format(_("You have %d unsaved document(s) open! Are you sure you want to exit without saving?"), unsavedEditors),
              _("SimplyCpp"), wxYES_NO);
        
        SimplyCpp::UI::ShowWindowModalThenDo(dialog, [dialog, exitHandler] (int returnCode)
        {
            dialog->Destroy();
            
            if (returnCode == wxID_YES)
                exitHandler();
        });
    }
    else
        exitHandler();
}

void MainFrame::OnMenuCut(wxCommandEvent& WXUNUSED(e))
{
    if (m_notebook->GetPageCount() == 0)
        return;

    EditorWidget* current = static_cast<EditorWidget*>(m_notebook->GetCurrentPage());
    current->Cut();
}

void MainFrame::OnMenuCopy(wxCommandEvent& WXUNUSED(e))
{
    if (m_notebook->GetPageCount() == 0)
        return;

    EditorWidget* current = static_cast<EditorWidget*>(m_notebook->GetCurrentPage());
    current->Copy();
}

void MainFrame::OnMenuPaste(wxCommandEvent& WXUNUSED(e))
{
    if (m_notebook->GetPageCount() == 0)
        return;

    EditorWidget* current = static_cast<EditorWidget*>(m_notebook->GetCurrentPage());
    current->Paste();
}

void MainFrame::OnMenuSelectAll(wxCommandEvent& WXUNUSED(e))
{
    if (m_notebook->GetPageCount() == 0)
        return;

    EditorWidget* current = static_cast<EditorWidget*>(m_notebook->GetCurrentPage());
    current->SelectAll();
}

void MainFrame::OnMenuUndo(wxCommandEvent& WXUNUSED(e))
{
    if (m_notebook->GetPageCount() == 0)
        return;

    EditorWidget* current = static_cast<EditorWidget*>(m_notebook->GetCurrentPage());
    current->Undo();
}

void MainFrame::OnMenuRedo(wxCommandEvent& WXUNUSED(e))
{
    if (m_notebook->GetPageCount() == 0)
        return;

    EditorWidget* current = static_cast<EditorWidget*>(m_notebook->GetCurrentPage());
    current->Redo();
}

void MainFrame::OnMenuProjectExplorer(wxCommandEvent& WXUNUSED(e))
{
    m_mgr.GetPane("pane_project").Show();
    m_mgr.Update();
}

void MainFrame::OnMenuProperties(wxCommandEvent& WXUNUSED(e))
{
    m_mgr.GetPane("pane_props").Show();
    m_mgr.Update();
}

void MainFrame::OnMenuOutput(wxCommandEvent& WXUNUSED(e))
{
    m_mgr.GetPane("pane_output").Show();
    m_mgr.Update();
}

void MainFrame::OnMenuTerminal(wxCommandEvent& WXUNUSED(e))
{
    m_mgr.GetPane("pane_output").Show();
    m_mgr.Update();
    TerminalWidget* terminalWidget = static_cast<TerminalWidget*>(m_mgr.GetPane("pane_output").window);

#ifdef __WXMSW__
    wxString executablePath = wxStandardPaths::Get().GetExecutablePath();
    wxString executableDir = wxPathOnly(executablePath);

    wxString compilerPath = wxPathOnly(executableDir) + "\\TDM-GCC-32\\bin\\";

    if (!wxFileExists(compilerPath + "g++.exe"))
        compilerPath = executableDir + "\\TDM-GCC-32\\bin\\";

    if (!wxFileExists(compilerPath + "g++.exe"))
    {
        wxMessageBox("Error, cannot find the compiler. Are you sure there is "
            "'TDM-GCC-32' folder in '" + executableDir + "' ?", "SimplyCpp error", wxICON_ERROR, this);
        return;
    }

    wxExecuteEnv env;
    env.cwd = wxPathOnly(((EditorWidget*)m_notebook->GetCurrentPage())->GetFileName());
    env.env.insert(wxStringToStringHashMap_wxImplementation_Pair("PATH", compilerPath + ";" + wxGetenv("PATH")));
#endif

#ifdef __WXMSW__
    terminalWidget->RunCommand("cmd /q", env);
#elif __WXMAC__
    terminalWidget->RunCommand("/bin/sh -c \"exec $SHELL -l -i\"");
#else
    terminalWidget->RunCommand("sh");
#endif
}

void MainFrame::OnMenuProjectClean(wxCommandEvent& WXUNUSED(e))
{
}

void MainFrame::OnMenuProjectCreate(wxCommandEvent& WXUNUSED(e))
{
}

void MainFrame::OnMenuCompile(wxCommandEvent& WXUNUSED(e))
{
    DoCompile([] {});
}

void MainFrame::OnMenuRun(wxCommandEvent& WXUNUSED(e))
{
    TerminalWidget* terminalWidget = static_cast<TerminalWidget*>(m_mgr.GetPane("pane_output").window);

    wxExecuteEnv env;
    env.cwd = wxPathOnly(((EditorWidget*)m_notebook->GetCurrentPage())->GetFileName());

    wxString fileName = ((EditorWidget*)m_notebook->GetCurrentPage())->GetFileName();

    if (fileName.Contains(".h"))
    {
        wxMessageBox("Error, cannot run a header file", "SimplyCpp Error", wxICON_ERROR);
        return;
    }

    wxString exeName = wxString(fileName);
    exeName.Replace(".cpp", ".exe");
    exeName.Replace(".c", ".exe");

    if (!wxFileExists(exeName))
    {
        wxMessageBox("You have to compile the program first!", "SimplyCpp", wxICON_EXCLAMATION);
        return;
    }

    terminalWidget->RunCommand(exeName, env);
}

void MainFrame::OnMenuCompileAndRun(wxCommandEvent& e)
{
    DoCompile([this, &e]()
    {
        OnMenuRun(e);
    });
}

void MainFrame::OnMenuAbout(wxCommandEvent& WXUNUSED(e))
{
}

void SimplyCpp::UI::MainFrame::DoCompile(Callback&& callback)
{
    TerminalWidget* terminalWidget = static_cast<TerminalWidget*>(m_mgr.GetPane("pane_output").window);

    wxString executablePath = wxStandardPaths::Get().GetExecutablePath();
    wxString executableDir = wxPathOnly(executablePath);

    wxString compilerPath = wxPathOnly(executableDir) + "\\TDM-GCC-32\\bin\\";

    if (!wxFileExists(compilerPath + "g++.exe"))
        compilerPath = executableDir + "\\TDM-GCC-32\\bin\\";

    if (!wxFileExists(compilerPath + "g++.exe"))
    {
        wxMessageBox("Error, cannot find the compiler. Are you sure there is "
            "'TDM-GCC-32' folder in '" + executableDir + "' ?", "SimplyCpp error", wxICON_ERROR, this);
        return;
    }

    wxExecuteEnv env;
    env.cwd = wxPathOnly(((EditorWidget*)m_notebook->GetCurrentPage())->GetFileName());
    env.env.insert(wxStringToStringHashMap_wxImplementation_Pair("PATH", compilerPath + ";" + wxGetenv("PATH")));

    wxString fileName = wxFileNameFromPath(((EditorWidget*)m_notebook->GetCurrentPage())->GetFileName());
    wxString exeName = wxString(fileName);
    exeName.Replace(".cpp", ".exe");
    exeName.Replace(".c", ".exe");

    if (fileName.Contains(".h"))
    {
        wxMessageBox("Error, cannot run a header file", "SimplyCpp Error", wxICON_ERROR);
        return;
    }

    terminalWidget->RunCommand(compilerPath + "g++.exe " + fileName + " -Wall -Wextra -s -o " + exeName, env, [this, callback]()
    {
        ParseCompilerOutput();
        callback();
    },
    [this]()
    {
        ParseCompilerOutput();
    });
}

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(ID_NEW, MainFrame::OnMenuNew)
    EVT_MENU(ID_OPEN, MainFrame::OnMenuOpen)
    EVT_MENU(ID_SAVE, MainFrame::OnMenuSave)
    EVT_MENU(ID_SAVE_AS, MainFrame::OnMenuSaveAs)
    EVT_MENU(ID_CLOSE, MainFrame::OnMenuClose)
    EVT_MENU(ID_EXIT, MainFrame::OnMenuExit)

    EVT_MENU(ID_CUT, MainFrame::OnMenuCut)
    EVT_MENU(ID_COPY, MainFrame::OnMenuCopy)
    EVT_MENU(ID_PASTE, MainFrame::OnMenuPaste)
    EVT_MENU(ID_SELECT_ALL, MainFrame::OnMenuSelectAll)
    EVT_MENU(ID_UNDO, MainFrame::OnMenuUndo)
    EVT_MENU(ID_REDO, MainFrame::OnMenuRedo)

    EVT_MENU(ID_COMPILE, MainFrame::OnMenuCompile)
    EVT_MENU(ID_RUN, MainFrame::OnMenuRun)
    EVT_MENU(ID_COMPILE_AND_RUN, MainFrame::OnMenuCompileAndRun)

    EVT_MENU(ID_PROJECT_EXPLORER, MainFrame::OnMenuProjectExplorer)
    EVT_MENU(ID_PROPERTIES, MainFrame::OnMenuProperties)
    EVT_MENU(ID_OUTPUT, MainFrame::OnMenuOutput)
    EVT_MENU(ID_TERMINAL, MainFrame::OnMenuTerminal)
    EVT_CLOSE(MainFrame::OnWindowClose)

    EVT_AUINOTEBOOK_PAGE_CLOSE(wxID_ANY, MainFrame::OnEditorClose)
END_EVENT_TABLE()
