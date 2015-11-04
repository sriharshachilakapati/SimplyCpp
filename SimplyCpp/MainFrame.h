#pragma once
#include "EditorWidget.h"
#include "TerminalWidget.h"

class MainFrame : public wxFrame
{
public:
    MainFrame();
    ~MainFrame();

private:
    DECLARE_EVENT_TABLE()

    void CreateMenuBar();

    void OnMenuNew(wxCommandEvent& e);
    void OnMenuOpen(wxCommandEvent& e);
    void OnMenuSave(wxCommandEvent& e);
    void OnMenuSaveAs(wxCommandEvent& e);
    void OnMenuExit(wxCommandEvent& e);
    void OnMenuCut(wxCommandEvent& e);
    void OnMenuCopy(wxCommandEvent& e);
    void OnMenuPaste(wxCommandEvent& e);
    void OnMenuSelectAll(wxCommandEvent& e);
    void OnMenuUndo(wxCommandEvent& e);
    void OnMenuRedo(wxCommandEvent& e);
    void OnMenuProjectExplorer(wxCommandEvent& e);
    void OnMenuProperties(wxCommandEvent& e);
    void OnMenuOutput(wxCommandEvent& e);
    void OnMenuTerminal(wxCommandEvent& e);
    void OnMenuProjectClean(wxCommandEvent& e);
    void OnMenuProjectCreate(wxCommandEvent& e);
    void OnMenuCompile(wxCommandEvent& e);
    void OnMenuRun(wxCommandEvent& e);
    void OnMenuCompileAndRun(wxCommandEvent& e);
    void OnMenuAbout(wxCommandEvent& e);

    wxAuiManager  m_mgr;
    wxAuiNotebook *m_notebook;
    wxConfig      *m_config;
};

enum MenuIDs
{
    ID_NEW = 100,
    ID_OPEN,
    ID_SAVE,
    ID_SAVE_AS,
    ID_EXIT,

    ID_CUT,
    ID_COPY,
    ID_PASTE,
    ID_SELECT_ALL,
    ID_UNDO,
    ID_REDO,

    ID_PROJECT_EXPLORER,
    ID_PROPERTIES,
    ID_OUTPUT,
    ID_TERMINAL,

    ID_PROJECT_CLEAN,
    ID_PROJECT_CREATE,

    ID_COMPILE,
    ID_RUN,
    ID_COMPILE_AND_RUN,

    ID_ABOUT
};