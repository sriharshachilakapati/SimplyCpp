#pragma once
#include "EditorWidget.h"
#include "TerminalWidget.h"
#include "ProjectExplorer.h"
#include "ErrorList.h"

#include "../Core/Config.h"

namespace SimplyCpp
{
    namespace UI
    {
        class MainFrame : public wxFrame
        {
        public:
            MainFrame();
            ~MainFrame();

        private:
            DECLARE_EVENT_TABLE()

            void ParseCompilerOutput();

            void CreateMenuBar();
            void CreateToolBar();

            void OnEditorClose(wxAuiNotebookEvent& e);
            void OnWindowClose(wxCloseEvent& e);
            void OnMenuNew(wxCommandEvent& e);
            void OnMenuOpen(wxCommandEvent& e);
            void OnMenuSave(wxCommandEvent& e);
            void OnMenuSaveAs(wxCommandEvent& e);
            void OnMenuClose(wxCommandEvent& e);
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

            void DoCompile(Callback&& callback);

            wxAuiManager  m_mgr;
            wxAuiNotebook *m_notebook;
        };

        enum MenuIDs
        {
            ID_NEW = 100,
            ID_OPEN,
            ID_SAVE,
            ID_SAVE_AS,
            ID_CLOSE,
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
    }
}