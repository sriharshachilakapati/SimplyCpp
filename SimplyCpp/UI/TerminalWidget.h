#pragma once
namespace SimplyCpp
{
    namespace UI
    {
        typedef std::function<void()> Callback;

        class TerminalWidget : public wxPanel
        {
        public:
            TerminalWidget(wxWindow* parent);
            ~TerminalWidget();

            void TerminateProcess();
            void RunCommand(const wxString& command);
            void RunCommand(const wxString& command, const wxExecuteEnv& env);

            void RunCommand(const wxString& command, const wxExecuteEnv& env, Callback&& callback);
            void ClearOutput();

        private:
            void OnTimer(wxTimerEvent& e);
            void OnEnter(wxCommandEvent& e);
            void OnKeyDown(wxKeyEvent& e);
            void OnTerminate(wxProcessEvent& e);

            void OnTerminateClick(wxCommandEvent& e);
            void OnRestartClick(wxCommandEvent& e);

            wxProcess* m_process;

            wxInputStream* m_stdout;
            wxOutputStream* m_stdin;
            wxInputStream* m_stderr;

            wxTextCtrl* m_inputCtrl;
            wxTextCtrl* m_outputCtrl;

            wxTimer* m_timer;

            wxString m_sLastCommand;
            wxExecuteEnv m_LastCommandEnv;

            wxArrayString m_history;
            int m_nHistoryIndex;

            Callback m_callback;

            DECLARE_EVENT_TABLE()
        };

        enum EventIDs
        {
            ID_TERMINATE = 200,
            ID_RESTART
        };
    }
}