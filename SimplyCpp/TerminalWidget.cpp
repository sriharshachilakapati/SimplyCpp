#include "stdafx.h"
#include "TerminalWidget.h"

TerminalWidget::TerminalWidget(wxWindow* parent) : wxPanel(parent, wxID_ANY)
{
    m_inputCtrl = new wxTextCtrl(this, wxID_ANY);
    m_outputCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);

    m_outputCtrl->SetEditable(false);
    m_outputCtrl->SetBackgroundColour(wxColor(255, 255, 255));

    m_inputCtrl->SetWindowStyle(wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB);

    wxFont monospace = wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT, false, _("Consolas"));
    m_inputCtrl->SetFont(monospace);
    m_outputCtrl->SetFont(monospace);
    m_inputCtrl->Disable();

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer* sizer2 = new wxBoxSizer(wxVERTICAL);
    sizer2->Add(m_outputCtrl, 1, wxEXPAND | wxALL);
    sizer2->Add(m_inputCtrl, 0, wxALIGN_BOTTOM | wxEXPAND);

    wxBitmap stop = wxArtProvider::GetBitmap(wxART_CROSS_MARK, wxART_TOOLBAR);
    wxBitmap restart = wxArtProvider::GetBitmap(wxART_REDO, wxART_TOOLBAR);
    
    wxToolBar* toolbar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_VERTICAL);
    toolbar->AddTool(ID_TERMINATE, "", stop, "Terminate current process");
    toolbar->AddTool(ID_RESTART, "", restart, "Restart last process");

    toolbar->Realize();

    sizer->Add(toolbar, 0, wxALL);
    sizer->Add(sizer2, 1, wxEXPAND | wxALL);

    SetSizer(sizer);

    m_timer = new wxTimer(this, wxID_ANY);
    m_inputCtrl->Bind(wxEVT_TEXT_ENTER, &TerminalWidget::OnEnter, this);
}

TerminalWidget::~TerminalWidget()
{
    TerminateProcess();
}

void TerminalWidget::TerminateProcess()
{
    if (m_process)
        if (wxProcess::Exists(m_process->GetPid()))
        {
            wxProcess *process = m_process;
            m_process = NULL;
            process->SetNextHandler(NULL);

            process->CloseOutput();
            wxDELETE(m_process);
        }

    m_inputCtrl->Disable();
    m_timer->Stop();
}

void TerminalWidget::RunCommand(const wxString & command)
{
    TerminateProcess();
    ClearOutput();

    m_sLastCommand = command;
    m_process = new wxProcess(wxEXEC_ASYNC);
    m_process->Redirect();

    wxExecute(command, wxEXEC_ASYNC, m_process);

    m_stdout = m_process->GetInputStream();
    m_stdin = m_process->GetOutputStream();
    m_stderr = m_process->GetErrorStream();

    m_process->Bind(wxEVT_END_PROCESS, &TerminalWidget::OnTerminate, this);
    m_timer->Start(250);

    m_inputCtrl->Enable();
    m_inputCtrl->SetFocus();
}

void TerminalWidget::ClearOutput()
{
    m_outputCtrl->Clear();
}

void TerminalWidget::OnTimer(wxTimerEvent& e)
{
    if (m_process)
    {
        if (!wxProcess::Exists(m_process->GetPid()))
        {
            e.GetTimer().Stop();
            m_inputCtrl->Disable();
            return;
        }

        while (m_stdout->CanRead())
        {
            char buffer[4096];
            buffer[m_stdout->Read(buffer, WXSIZEOF(buffer) - 1).LastRead()] = '\0';
            m_outputCtrl->AppendText(buffer);
        }

        while (m_stderr->CanRead())
        {
            char buffer[4096];
            buffer[m_stderr->Read(buffer, WXSIZEOF(buffer) - 1).LastRead()] = '\0';
            m_outputCtrl->AppendText(buffer);
        }

        m_inputCtrl->Enable();
    }
}

void TerminalWidget::OnEnter(wxCommandEvent& WXUNUSED(e))
{
    if (m_process)
    {
        if (wxProcess::Exists(m_process->GetPid()))
        {
            if (!m_inputCtrl->GetValue().IsEmpty())
            {
                wxTextOutputStream out(*m_stdin);

                out << m_inputCtrl->GetValue() << '\n';
                m_outputCtrl->AppendText(m_inputCtrl->GetValue());
                m_outputCtrl->AppendText("\n");
            }
        }
        else
            m_outputCtrl->AppendText("\nError, program terminated, cannot pass input");
    }

    m_inputCtrl->Clear();
}

void TerminalWidget::OnTerminate(wxProcessEvent& e)
{
    m_outputCtrl->AppendText(wxString::Format("\nProcess finished with exit code %d\n", e.GetExitCode()));
    m_inputCtrl->Disable();
}

void TerminalWidget::OnTerminateClick(wxCommandEvent& WXUNUSED(e))
{
    if (!m_sLastCommand.IsEmpty())
    {
        TerminateProcess();
        m_inputCtrl->Disable();
    }
}

void TerminalWidget::OnRestartClick(wxCommandEvent& WXUNUSED(e))
{
    if (!m_sLastCommand.IsEmpty())
    {
        TerminateProcess();
        RunCommand(m_sLastCommand);
    }
    m_inputCtrl->Enable();
}

BEGIN_EVENT_TABLE(TerminalWidget, wxPanel)
    EVT_TIMER(wxID_ANY, TerminalWidget::OnTimer)

    EVT_TOOL(ID_TERMINATE, TerminalWidget::OnTerminateClick)
    EVT_TOOL(ID_RESTART, TerminalWidget::OnRestartClick)
END_EVENT_TABLE()