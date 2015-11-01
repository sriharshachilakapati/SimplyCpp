#include "stdafx.h"
#include "MainFrame.h"

MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, _T("SimplyCpp"), wxDefaultPosition, wxSize(800, 600))
{
	m_config = new wxConfig(_("SimplyCpp-2.0.1"));

	SetIcon(wxICON(ID_ICON));
	m_mgr.SetManagedWindow(this);

	CreateMenuBar();
	CreateStatusBar(2);

	wxTextCtrl* text1 = new wxTextCtrl(this, wxID_ANY, _T(""),
		wxDefaultPosition, wxSize(200, 150),
		wxNO_BORDER);
	wxTextCtrl* text2 = new wxTextCtrl(this, wxID_ANY, _T(""),
		wxDefaultPosition, wxSize(200, 150),
		wxNO_BORDER);

	TerminalWidget* terminal = new TerminalWidget(this);
	
	m_notebook = new wxAuiNotebook(this, wxID_ANY);
	m_notebook->AddPage(new EditorWidget(this), _("Untitled"));

	text1->SetEditable(false);
	text2->SetEditable(false);

	m_mgr.AddPane(text1, wxAuiPaneInfo().Name(_("pane_project")).Caption(_("Project Explorer")).Left());
	m_mgr.AddPane(text2, wxAuiPaneInfo().Name(_("pane_props")).Caption(_("Properties")).Right());
	m_mgr.AddPane(terminal, wxAuiPaneInfo().Name(_("pane_output")).Caption(_("Output")).Bottom());
	m_mgr.AddPane(m_notebook, wxAuiPaneInfo().Name(_("pane_notebook")).CaptionVisible(false).Center());

	m_notebook->SetFocus();

	m_mgr.Update();

	SetStatusText(_("SimplyCpp 2.0.1 is ready"), 0);

	wxString message = _("Written by Sri Harsha Chilakapati");
	int widths[2];
	widths[0] = -1;
	widths[1] = GetTextExtent(message).GetWidth();

	SetStatusWidths(2, widths);
	SetStatusText(message, 1);

	if (m_config->Exists(_("Layer.Perspective")))
	{
		wxString str = m_config->Read(_("Layer.Perspective"));
		m_mgr.LoadPerspective(str);
	}
}

MainFrame::~MainFrame()
{
	m_config->Write(_("Layer.Perspective"), m_mgr.SavePerspective());

	m_mgr.UnInit();
	delete m_config;
}

void MainFrame::CreateMenuBar()
{
	wxMenuBar* menuBar = new wxMenuBar();

	wxMenu* menuFile    = new wxMenu();
	wxMenu* menuEdit    = new wxMenu();
	wxMenu* menuProject = new wxMenu();
	wxMenu* menuCompile = new wxMenu();
	wxMenu* menuHelp    = new wxMenu();

	menuFile->Append(ID_NEW, _("New\tCtrl-N"), _("Create a new file"));
	menuFile->Append(ID_OPEN, _("Open\tCtrl-O"), _("Open an existing file into the IDE"));
	menuFile->Append(ID_SAVE, _("Save\tCtrl-S"), _("Save the current file"));
	menuFile->Append(ID_SAVE_AS, _("Save As\tCtrl-Shift-S"), _("Save a copy of the current file"));
	menuFile->AppendSeparator();
	menuFile->Append(ID_EXIT, _("Exit\tAlt-F4"), _("Exit the IDE"));

	menuEdit->Append(ID_COPY, _("Copy\tCtrl-C"), _("Copy the selected content"));
	menuEdit->Append(ID_CUT, _("Cut\tCtrl-X"), _("Cut the selected content"));
	menuEdit->Append(ID_PASTE, _("Paste\tCtrl-V"), _("Paste the copied content"));
	menuEdit->AppendSeparator();
	menuEdit->Append(ID_SELECT_ALL, _("Select All\tCtrl-A"), _("Select all the content"));

	menuBar->Append(menuFile, _("&File"));
	menuBar->Append(menuEdit, _("&Edit"));
	menuBar->Append(menuProject, _("&Project"));
	menuBar->Append(menuCompile, _("&Compile"));
	menuBar->Append(menuHelp, _("&Help"));

	this->SetMenuBar(menuBar);
}

void MainFrame::OnMenuNew(wxCommandEvent& WXUNUSED(e))
{
	m_notebook->AddPage(new EditorWidget(this),
		wxString::Format(_("Untitled%d"), m_notebook->GetPageCount() + 1));
}

void MainFrame::OnMenuOpen(wxCommandEvent& WXUNUSED(e))
{
	wxFileDialog* dialog = new wxFileDialog(this, _("Open File"), wxEmptyString, wxEmptyString,
		_("C++ Source Files (*.cpp, *.cxx)|*.cpp;*.cxx|	C Source files(*.c) | *.c | C header files(*.h) | *.h"), wxFD_OPEN);
	
	if (dialog->ShowModal() == wxID_OK)
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
		m_mgr.Update();
	}

	dialog->Destroy();
}

void MainFrame::OnMenuSave(wxCommandEvent& e)
{
	if (m_notebook->GetPageCount() == 0)
		return;

	EditorWidget* currentEditor = static_cast<EditorWidget*>(m_notebook->GetCurrentPage());

	if (currentEditor->SavedOnce())
		currentEditor->SaveFile();
	else
		OnMenuSaveAs(e);
}

void MainFrame::OnMenuSaveAs(wxCommandEvent& WXUNUSED(e))
{
	wxFileDialog* dialog = new wxFileDialog(this, wxFileSelectorPromptStr, wxEmptyString, wxEmptyString,
		_("C++ Source Files(*.cpp, *.cxx) | *.cpp; *.cxx | C Source files(*.c) | *.c | C header files(*.h) | *.h"), wxFD_SAVE);

	if (dialog->ShowModal() == wxID_OK)
	{
		wxString file = dialog->GetFilename();
		wxString path = dialog->GetPath();

		EditorWidget* currentEditor = static_cast<EditorWidget*>(m_notebook->GetCurrentPage());
		currentEditor->SaveFile(path);

		m_notebook->SetPageText(m_notebook->GetPageIndex(currentEditor), file);
		m_mgr.Update();
	}

	dialog->Destroy();
}

void MainFrame::OnMenuExit(wxCommandEvent& e)
{
	for (int i = 0; i < m_notebook->GetPageCount(); i++)
	{
		EditorWidget* editorPage = static_cast<EditorWidget*>(m_notebook->GetPage(i));

		if (!editorPage->SavedOnce() || editorPage->CodeChanged())
		{
			wxMessageDialog* dialog = new wxMessageDialog(this,
				wxString::Format(_("Do you want to save %s? If not, all your work will be lost!"), m_notebook->GetPageText(i)),
				_("SimplyCpp"), wxYES_NO | wxCANCEL);

			switch (dialog->ShowModal())
			{
				case wxID_YES:
					OnMenuSave(e);

					// Only break if the file is not saved
					// The user must have clicked on cancel button in save as box
					if (!editorPage->CodeChanged())
						break;

				case wxID_CANCEL:
					dialog->Destroy();
					return;
			}

			dialog->Destroy();
		}
	}

	Destroy();
}

void MainFrame::OnMenuCut(wxCommandEvent& WXUNUSED(e))
{
}

void MainFrame::OnMenuCopy(wxCommandEvent& WXUNUSED(e))
{
}

void MainFrame::OnMenuPaste(wxCommandEvent& WXUNUSED(e))
{
}

void MainFrame::OnMenuSelectAll(wxCommandEvent& WXUNUSED(e))
{
}

void MainFrame::OnMenuProjectClean(wxCommandEvent& WXUNUSED(e))
{
}

void MainFrame::OnMenuProjectCreate(wxCommandEvent& WXUNUSED(e))
{
}

void MainFrame::OnMenuCompile(wxCommandEvent& WXUNUSED(e))
{
}

void MainFrame::OnMenuRun(wxCommandEvent& WXUNUSED(e))
{
}

void MainFrame::OnMenuCompileAndRun(wxCommandEvent& e)
{
	OnMenuCompile(e);
	OnMenuRun(e);
}

void MainFrame::OnMenuAbout(wxCommandEvent& WXUNUSED(e))
{
}

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(ID_NEW, MainFrame::OnMenuNew)
	EVT_MENU(ID_OPEN, MainFrame::OnMenuOpen)
	EVT_MENU(ID_SAVE, MainFrame::OnMenuSave)
	EVT_MENU(ID_SAVE_AS, MainFrame::OnMenuSaveAs)
	EVT_MENU(ID_EXIT, MainFrame::OnMenuExit)
END_EVENT_TABLE()