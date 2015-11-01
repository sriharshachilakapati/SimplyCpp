#include "stdafx.h"
#include "TerminalWidget.h"

TerminalWidget::TerminalWidget(wxWindow* parent) : wxPanel(parent, wxID_ANY)
{
	m_process = new wxProcess(wxEXEC_ASYNC);
	m_process->Redirect();

	m_inputCtrl = new wxTextCtrl(this, wxID_ANY);
	m_outputCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);

	m_outputCtrl->SetEditable(false);
	m_outputCtrl->SetBackgroundColour(wxColor(255, 255, 255));

	m_inputCtrl->SetWindowStyle(wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB);

	wxFont monospace = wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT, false, _("Consolas"));
	m_inputCtrl->SetFont(monospace);
	m_outputCtrl->SetFont(monospace);

	wxExecute(_("cmd"), wxEXEC_ASYNC, m_process);

	m_stdout = m_process->GetInputStream();
	m_stdin = m_process->GetOutputStream();
	m_stderr = m_process->GetErrorStream();

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(m_outputCtrl, 1, wxEXPAND | wxALL);
	sizer->Add(m_inputCtrl, 0, wxALIGN_BOTTOM | wxEXPAND);

	SetSizer(sizer);
	m_timer = new wxTimer(this, wxID_ANY);
	m_timer->Start(500);

	m_inputCtrl->Bind(wxEVT_TEXT_ENTER, &TerminalWidget::OnEnter, this);
}

TerminalWidget::~TerminalWidget()
{
	if (wxProcess::Exists(m_process->GetPid()))
	{
		wxProcess *process = m_process;
        m_process = NULL;
        process->SetNextHandler(NULL);

        process->CloseOutput();
		wxDELETE(m_process);
	}
}

void TerminalWidget::OnTimer(wxTimerEvent& e)
{
	if (!wxProcess::Exists(m_process->GetPid()))
	{
		e.GetTimer().Stop();
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
}

void TerminalWidget::OnEnter(wxCommandEvent& WXUNUSED(e))
{
	if (wxProcess::Exists(m_process->GetPid()))
	{
		if (!m_inputCtrl->GetValue().IsEmpty())
		{
			wxTextOutputStream out(*m_stdin);

			out << m_inputCtrl->GetValue() << '\n';

			m_inputCtrl->Clear();
		}
	}
}

BEGIN_EVENT_TABLE(TerminalWidget, wxPanel)
	EVT_TIMER(wxID_ANY, TerminalWidget::OnTimer)
END_EVENT_TABLE()