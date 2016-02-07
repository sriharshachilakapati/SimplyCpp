#include "../stdafx.h"
#include "TerminalWidget.h"

using namespace SimplyCpp::UI;

TerminalWidget::TerminalWidget(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(400, 400))
{
    m_inputCtrl = new wxTextCtrl(this, wxID_ANY);
    m_outputCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        wxTE_MULTILINE | wxTE_RICH);

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
    m_inputCtrl->Bind(wxEVT_KEY_DOWN, &TerminalWidget::OnKeyDown, this);
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

            if (wxKill(process->GetPid(), wxSignal::wxSIGTERM) == -1)
                // Try force killing
                wxKill(process->GetPid(), wxSignal::wxSIGKILL);

            wxDELETE(m_process);
        }

    m_inputCtrl->Disable();
    m_timer->Stop();

    m_history.Clear();
}

void TerminalWidget::RunCommand(const wxString& command, const wxExecuteEnv& env)
{
    RunCommand(command, env, [] {});
}

void TerminalWidget::RunCommand(const wxString & command, const wxExecuteEnv & env, Callback && callbackSuccess, Callback && callbackError)
{
    m_callbackSuccess = callbackSuccess;
    m_callbackError = callbackError;

    m_history.Clear();
    m_nHistoryIndex = -1;

    TerminateProcess();
    ClearOutput();

    m_sLastCommand = command;
    m_LastCommandEnv = env;

    m_process = new wxProcess(wxEXEC_ASYNC);
    m_process->Redirect();

    wxExecute(command, wxEXEC_ASYNC, m_process, &env);

    m_stdout = m_process->GetInputStream();
    m_stdin = m_process->GetOutputStream();
    m_stderr = m_process->GetErrorStream();

    m_timer->Start(250);
    PollOutput(*m_timer);

    m_process->Bind(wxEVT_END_PROCESS, &TerminalWidget::OnTerminate, this);

    m_inputCtrl->Enable();
    m_inputCtrl->SetFocus();
}

void TerminalWidget::RunCommand(const wxString& command)
{
    RunCommand(command, wxExecuteEnv());
}

void TerminalWidget::ClearOutput()
{
    m_outputCtrl->Clear();
}

int SimplyCpp::UI::TerminalWidget::GetNumOutputLines()
{
    return m_outputCtrl->GetNumberOfLines();
}

wxString SimplyCpp::UI::TerminalWidget::GetOutputLine(int line)
{
    return m_outputCtrl->GetLineText(line);
}

void TerminalWidget::OnTimer(wxTimerEvent& e)
{
    PollOutput(e.GetTimer());
}

void TerminalWidget::OnEnter(wxCommandEvent& WXUNUSED(e))
{
    m_nHistoryIndex = -1;

    if (m_process)
    {
        if (wxProcess::Exists(m_process->GetPid()))
        {
            wxTextOutputStream out(*m_stdin);

            out << m_inputCtrl->GetValue() << '\n';

            m_outputCtrl->SetDefaultStyle(wxTextAttr(wxColor(0, 100, 50)));
            m_outputCtrl->AppendText(m_inputCtrl->GetValue());
            m_outputCtrl->AppendText("\n");

            if (!m_inputCtrl->GetValue().IsEmpty())
                m_history.Add(m_inputCtrl->GetValue());
        }
        else
        {
            m_outputCtrl->SetDefaultStyle(wxTextAttr(*wxRED));
            m_outputCtrl->AppendText("\nError, program terminated, cannot pass input");
        }
    }

    m_inputCtrl->Clear();
}

void TerminalWidget::OnKeyDown(wxKeyEvent& e)
{
    if (e.GetKeyCode() == wxKeyCode::WXK_UP)
    {
        m_history.Add(m_inputCtrl->GetValue());
        
        if (m_nHistoryIndex == -1)
            m_nHistoryIndex = m_history.GetCount() - 2;
        else
            m_nHistoryIndex--;

        m_nHistoryIndex = m_nHistoryIndex < 0 ? 0 : m_nHistoryIndex;

        m_inputCtrl->SetValue(m_history[m_nHistoryIndex]);
    }

    else if (e.GetKeyCode() == wxKeyCode::WXK_DOWN)
    {
        if (m_nHistoryIndex != -1 && m_nHistoryIndex - 1 < (int) m_history.GetCount())
        {
            m_nHistoryIndex++;

            if (m_nHistoryIndex + 1 == (int) m_history.GetCount())
            {
                m_nHistoryIndex = -1;
                m_inputCtrl->Clear();
            }
            else
                m_inputCtrl->SetValue(m_history[m_nHistoryIndex]);
        }
    }

    e.Skip();
}

void TerminalWidget::OnTerminate(wxProcessEvent& e)
{
    // Read all the remaining messages
    m_outputCtrl->SetDefaultStyle(wxTextAttr(*wxRED));
    while (m_stderr->CanRead())
    {
        char buffer[4096];
        buffer[m_stderr->Read(buffer, WXSIZEOF(buffer) - 1).LastRead()] = '\0';
        m_outputCtrl->AppendText(buffer);
    }

    m_outputCtrl->SetDefaultStyle(wxTextAttr(*wxBLACK));
    while (m_stdout->CanRead())
    {
        char buffer[4096];
        buffer[m_stdout->Read(buffer, WXSIZEOF(buffer) - 1).LastRead()] = '\0';
        m_outputCtrl->AppendText(buffer);
    }

    m_outputCtrl->SetDefaultStyle(wxTextAttr(wxColor(100, 100, 100)));
    m_outputCtrl->AppendText(wxString::Format("\nProcess finished with exit code %d\n", e.GetExitCode()));
    m_inputCtrl->Disable();

    m_history.Clear();

    if (e.GetExitCode() == 0)
        m_callbackSuccess();
    else
        m_callbackError();
}

void SimplyCpp::UI::TerminalWidget::PollOutput(wxTimer& timer)
{
    if (m_process)
    {
        if (!wxProcess::Exists(m_process->GetPid()))
        {
            timer.Stop();
            m_inputCtrl->Disable();
            return;
        }

        m_outputCtrl->SetDefaultStyle(wxTextAttr(*wxRED));
        while (m_stderr->CanRead())
        {
            char buffer[4096];
            buffer[m_stderr->Read(buffer, WXSIZEOF(buffer) - 1).LastRead()] = '\0';
            m_outputCtrl->AppendText(buffer);
        }

        m_outputCtrl->SetDefaultStyle(wxTextAttr(*wxBLACK));
        while (m_stdout->CanRead())
        {
            char buffer[4096];
            buffer[m_stdout->Read(buffer, WXSIZEOF(buffer) - 1).LastRead()] = '\0';
            m_outputCtrl->AppendText(buffer);
        }

        m_inputCtrl->Enable();
    }
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
        RunCommand(m_sLastCommand, m_LastCommandEnv);
    }
    m_inputCtrl->Enable();
}

BEGIN_EVENT_TABLE(TerminalWidget, wxPanel)
    EVT_TIMER(wxID_ANY, TerminalWidget::OnTimer)

    EVT_TOOL(ID_TERMINATE, TerminalWidget::OnTerminateClick)
    EVT_TOOL(ID_RESTART, TerminalWidget::OnRestartClick)
END_EVENT_TABLE()