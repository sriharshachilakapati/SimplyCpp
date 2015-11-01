#pragma once
class TerminalWidget : public wxPanel
{
public:
	TerminalWidget(wxWindow* parent);
	~TerminalWidget();

private:
	void OnTimer(wxTimerEvent& e);
	void OnEnter(wxCommandEvent& e);

	wxProcess* m_process;

	wxInputStream* m_stdout;
	wxOutputStream* m_stdin;
	wxInputStream* m_stderr;

	wxTextCtrl* m_inputCtrl;
	wxTextCtrl* m_outputCtrl;

	wxTimer* m_timer;

	DECLARE_EVENT_TABLE()
};