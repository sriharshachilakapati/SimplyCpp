using System;
using System.IO;
using FastColoredTextBoxNS;
using System.Text;
using System.Diagnostics;
using System.Windows.Forms;

namespace SimplyCpp
{

    /// <summary>
    /// Main Interface form.
    /// </summary>
    public partial class MainForm : Form
	{
		
		private string CurrentFile { get; set; }  // The PATH of the current file
		private string SavedCode   { get; set; }  // The CODE saved in the current file
		
		private CppHighlighter highlighter;  // To Highlight Cpp code
		
		/// <summary>
		/// Main Constructor. Opens the editor with empty file
		/// </summary>
		public MainForm()
		{
			//
			// The InitializeComponent() call is required for Windows Forms designer support.
			//
			InitializeComponent();
			
			CurrentFile = "Untitled.cpp";
			SavedCode   = "";
			
			UpdateTitle();
			
        	highlighter = new CppHighlighter(fastColoredTextBox1);
		}
		
		/// <summary>
		/// Alternative Constructor. Opens the editor with an existing file.
		/// </summary>
		/// <param name="fileName">The file to open for editing</param>
		public MainForm(string fileName) : this()
		{
			CurrentFile = fileName;
				
			TextReader reader = new StreamReader(CurrentFile);
				
			SavedCode = reader.ReadToEnd();
			reader.Close();
			reader.Dispose();
			
			fastColoredTextBox1.Text = SavedCode;
			
			highlighter.HighlightAll();
			UpdateTitle();
		}
		
		/// <summary>
		/// Check whether the code is saved or not!
		/// </summary>
		DialogResult CheckSaveUpdate()
		{
			if ((!File.Exists(CurrentFile) && !string.IsNullOrEmpty(fastColoredTextBox1.Text)) || fastColoredTextBox1.Text != SavedCode)
			{
				return MessageBox.Show("Do you want to save your code? If not, all the changes you made since last save will be lost.", Text,
						                       MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);
			}
			else
			{
				return DialogResult.OK;
			}
		}
		
		/// <summary>
		/// Updates the title of the application
		/// </summary>
		void UpdateTitle()
		{
			Text = Path.GetFileName(CurrentFile) + " - SimplyCpp";
		}
		
		/// <summary>
		/// Called whenever the code in the editor is changed
		/// </summary>
		void FastColoredTextBox1TextChanged(object sender, TextChangedEventArgs e)
		{
			// Only Highlight the visible area to prevent excess
			// memory usage when working with large source files.
			
			if (highlighter != null)
				highlighter.HighlightVisible();
		}
		
		/// <summary>
		/// Convineance Method to create menuitems. This exists because I'm
		/// using NativeMenu instead of the MenuStrip control.
		/// </summary>
		/// <param name="caption">The text on the menuitem</param>
		/// <param name="shortcut">The shortcut key</param>
		/// <param name="parent">Parent Menu-Item</param>
		/// <param name="handler">EventHandler to handle click event</param>
		/// <param name="Checked">Whether the menu-item is checked</param>
		void AddMenuItem(string caption, Shortcut shortcut, MenuItem parent, EventHandler handler, bool Checked)
		{
			MenuItem item = new MenuItem(caption, handler);
			item.Shortcut = shortcut;
			item.ShowShortcut = true;
			item.Checked = Checked;
			
			parent.MenuItems.Add(item);
		}
		
		/// <summary>
		/// Convineance Method to create menuitems. This exists because I'm
		/// using ContextMenu instead of ContextMenuStrip control.
		/// </summary>
		/// <param name="caption">The text on the menuitem</param>
		/// <param name="shortcut">The shortcut key</param>
		/// <param name="parent">Parent ContextMenu</param>
		/// <param name="handler">EventHandler to handle click event</param>
		void AddMenuItem(string caption, Shortcut shortcut, ContextMenu parent, EventHandler handler)
		{
			MenuItem item = new MenuItem(caption, handler);
			item.Shortcut = shortcut;
			item.ShowShortcut = true;
			
			parent.MenuItems.Add(item);
		}
		
		/// <summary>
		/// Adds a separator to a menu-item.
		/// </summary>
		void AddSeparator(MenuItem parent)
		{
			parent.MenuItems.Add("-");
		}
		
		/// <summary>
		/// Adds a separator to a contextmenu
		/// </summary>
		void AddSeparator(ContextMenu parent)
		{
			parent.MenuItems.Add("-");
		}
		
		/// <summary>
		/// Occurs when the 'New' menuitem is clicked
		/// or when the user pressed Ctrl+N shortcut.
		/// </summary>
		void NewClick(object sender, EventArgs e)
		{
			switch (CheckSaveUpdate())
			{
				case DialogResult.Yes:
					Save();
					break;
				case DialogResult.No:
					break;
				case DialogResult.Cancel:
					return;
			}
			
			CurrentFile = "Untitled.cpp";
			SavedCode   = "";
			
			fastColoredTextBox1.Text = SavedCode;
			
			UpdateTitle();
		}
		
		/// <summary>
		/// Occurs when the user clicks the "Open" menuitem
		/// or when the shortcut Ctrl
		void OpenClick(object sender, EventArgs e)
		{
			switch (CheckSaveUpdate())
			{
				case DialogResult.Yes:
					Save();
					break;
				case DialogResult.No:
					break;
				case DialogResult.Cancel:
					return;
			}
			
			if (ofd.ShowDialog() == DialogResult.OK)
			{
				CurrentFile = ofd.FileName;
				
				TextReader reader = new StreamReader(CurrentFile);
				
				SavedCode = reader.ReadToEnd();
				reader.Close();
				reader.Dispose();
				
				// TODO: Find a better solution than this hack
				fastColoredTextBox1.SelectAll();
				fastColoredTextBox1.Focus();
				SendKeys.SendWait("{DEL}");
				
				fastColoredTextBox1.Text = SavedCode;
				
				highlighter.HighlightAll();
				UpdateTitle();
			}
		}
		
		void SaveClick(object sender, EventArgs e)
		{
			Save();
		}
		
		void SaveAsClick(object sender, EventArgs e)
		{
			SaveAs();
		}
		
		void ExitClick(object sender, EventArgs e)
		{
			Close();
		}
		
		void CutClick(object sender, EventArgs e)
		{
			fastColoredTextBox1.Cut();
		}
		
		void CopyClick(object sender, EventArgs e)
		{
			fastColoredTextBox1.Copy();
		}
		
		void PasteClick(object sender, EventArgs e)
		{
			fastColoredTextBox1.Paste();
			highlighter.HighlightAll();
		}
		
		void SelectAllClick(object sender, EventArgs e)
		{
			fastColoredTextBox1.SelectAll();
		}
		
		void FindClick(object sender, EventArgs e)
		{
			fastColoredTextBox1.ShowFindDialog();
		}
		
		void ReplaceClick(object sender, EventArgs e)
		{
			fastColoredTextBox1.ShowReplaceDialog();
		}
		
		void Compile1Click(object sender, EventArgs e)
		{
			Compile();
		}
		
		void RunClick(object sender, EventArgs e)
		{
			string exeName    = Path.GetFileNameWithoutExtension(CurrentFile) + ".exe";
			string exePath    = Directory.GetParent(CurrentFile) + "";
			
			if (File.Exists(Path.Combine(exePath, exeName)))
				Run();
		}
		
		void CompileRunClick(object sender, EventArgs e)
		{
			Compile();
			
			string exeName    = Path.GetFileNameWithoutExtension(CurrentFile) + ".exe";
			string exePath    = Directory.GetParent(CurrentFile) + "";
			
			if (File.Exists(Path.Combine(exePath, exeName)))
				Run();
		}
		
		void ShowMessagesClick(object sender, EventArgs e)
		{
			MenuItem item = sender as MenuItem;
			
			if (item.Checked)
				logPanel.Hide();
			else
				logPanel.Show();
			
			item.Checked = !item.Checked;
		}
		
		void ShowHideDocumentMapClick(object sender, EventArgs e)
		{
			MenuItem item = sender as MenuItem;
			
			if (item.Checked)
				documentMap1.Hide();
			else
				documentMap1.Show();
			
			item.Checked = !item.Checked;
		}
		
		void ChangeFontClick(object sender, EventArgs e)
		{
			fontDialog1.Font = fastColoredTextBox1.Font;
			
			if (fontDialog1.ShowDialog() == DialogResult.OK)
			{
				fastColoredTextBox1.Font = fontDialog1.Font;
				fastColoredTextBox1.Refresh();
			}
		}
		
		void WordWrapClick(object sender, EventArgs e)
		{
			MenuItem item = sender as MenuItem;
			
			if (item.Checked)
				fastColoredTextBox1.WordWrap = false;
			else
				fastColoredTextBox1.WordWrap = true;
			
			item.Checked = !item.Checked;
		}
		
		void AboutClick(object sender, EventArgs e)
		{
			new AboutForm();
		}
		
		void MainFormLoad(object sender, EventArgs e)
		{
			MenuItem item = new MenuItem("File");
			mainMenu1.MenuItems.Add(item);
			
			AddMenuItem("New",     Shortcut.CtrlN,      item, new EventHandler(NewClick), false);
			AddMenuItem("Open",    Shortcut.CtrlO,      item, new EventHandler(OpenClick), false);
			AddMenuItem("Save",    Shortcut.CtrlS,      item, new EventHandler(SaveClick), false);
			AddMenuItem("Save As", Shortcut.CtrlShiftS, item, new EventHandler(SaveAsClick), false);
			AddSeparator(item);
			AddMenuItem("Exit", Shortcut.AltF4, item, new EventHandler(ExitClick), false);
			
			item = new MenuItem("Edit");
        	mainMenu1.MenuItems.Add(item);
        	
        	AddMenuItem("Cut",        Shortcut.CtrlX, item, new EventHandler(CutClick), false);
        	AddMenuItem("Copy",       Shortcut.CtrlC, item, new EventHandler(CopyClick), false);
        	AddMenuItem("Paste",      Shortcut.CtrlV, item, new EventHandler(PasteClick), false);
        	AddMenuItem("Select All", Shortcut.CtrlA, item, new EventHandler(SelectAllClick), false);
        	AddSeparator(item);
        	AddMenuItem("Find",       Shortcut.CtrlF, item, new EventHandler(FindClick), false);
        	AddMenuItem("Replace",    Shortcut.CtrlH, item, new EventHandler(ReplaceClick), false);
            
            item = new MenuItem("Compile");
            mainMenu1.MenuItems.Add(item);
            
            AddMenuItem("Compile",        Shortcut.AltF9, item, new EventHandler(Compile1Click), false);
            AddMenuItem("Run",            Shortcut.CtrlF9, item, new EventHandler(RunClick), false);
            AddMenuItem("Compile && Run", Shortcut.F9, item, new EventHandler(CompileRunClick), false);
            
            item = new MenuItem("Window");
            mainMenu1.MenuItems.Add(item);
            
            AddMenuItem("Show/Hide Messages", Shortcut.CtrlM, item, new EventHandler(ShowMessagesClick), true);
            AddMenuItem("Show/Hide Document Pane", Shortcut.CtrlD, item, new EventHandler(ShowHideDocumentMapClick), true);
            
            item = new MenuItem("Editor");
            mainMenu1.MenuItems.Add(item);
            
            AddMenuItem("Change Font", Shortcut.None, item, new EventHandler(ChangeFontClick), false);
            AddMenuItem("Wordwrap", Shortcut.None, item, new EventHandler(WordWrapClick), false);
            
            item = new MenuItem("Help");
            mainMenu1.MenuItems.Add(item);
            
            AddMenuItem("About", Shortcut.None, item, new EventHandler(AboutClick), false);
            
            fastColoredTextBox1.ContextMenu = contextMenu1;
            
            AddMenuItem("Cut",        Shortcut.CtrlX, contextMenu1, new EventHandler(CutClick));
            AddMenuItem("Copy",       Shortcut.CtrlC, contextMenu1, new EventHandler(CopyClick));
        	AddMenuItem("Paste",      Shortcut.CtrlV, contextMenu1, new EventHandler(PasteClick));
        	AddMenuItem("Select All", Shortcut.CtrlA, contextMenu1, new EventHandler(SelectAllClick));
        	AddSeparator(contextMenu1);
        	AddMenuItem("Find",       Shortcut.CtrlF, contextMenu1, new EventHandler(FindClick));
        	AddMenuItem("Replace",    Shortcut.CtrlH, contextMenu1, new EventHandler(ReplaceClick));
		}
		
		void Save()
		{
			if (!File.Exists(CurrentFile))
			{
				SaveAs();
				return;
			}
			
			fastColoredTextBox1.SaveToFile(CurrentFile, Encoding.Default);
			SavedCode = fastColoredTextBox1.Text;
			
			UpdateTitle();
		}
		
		void SaveAs()
		{
			if (sfd.ShowDialog() == DialogResult.OK)
			{
				CurrentFile = sfd.FileName;
				SavedCode = fastColoredTextBox1.Text;
				fastColoredTextBox1.SaveToFile(CurrentFile, Encoding.Default);
			}
			
			UpdateTitle();
		}
		
		void Compile()
		{
			switch (CheckSaveUpdate())
			{
				case DialogResult.Yes:
					Save();
					break;
				case DialogResult.No:
					MessageBox.Show("You must save your program in order to compile it!", Text);
					return;
				case DialogResult.Cancel:
					return;
			}
			
			string fileName   = Path.GetFileName(CurrentFile);
			string exeName    = Path.GetFileNameWithoutExtension(CurrentFile) + ".exe";
			string workingDir = Directory.GetParent(CurrentFile) + "";
			string compile    = Directory.GetParent(Application.ExecutablePath) + "\\compile.cmd";
			
			File.Delete(Path.Combine(workingDir, exeName));
			
			StartProcess(true, "cmd", "/c", "\"" + compile + "\"", workingDir, fileName, exeName);
		}
		
		void Run()
		{
			string exeName    = Path.GetFileNameWithoutExtension(CurrentFile) + ".exe";
			string workingDir = Directory.GetParent(CurrentFile) + "";
			string run        = Directory.GetParent(Application.ExecutablePath) + "\\run.cmd";
			
			logBox.AppendText(GetTimestamp() + "  Running " + exeName + "\n");
			StartProcess(false, "cmd", "/c", "\"" + run + "\"", workingDir, exeName);
		}
		
		void StartProcess(bool hidden, string command, params string[] args)
		{
			ProcessStartInfo pStartInfo = new ProcessStartInfo();
			
			pStartInfo.FileName = command;
			pStartInfo.Arguments = string.Join(" ", args);
			pStartInfo.UseShellExecute = false;
			
			if (hidden)
			{
				pStartInfo.RedirectStandardError  = true;
				pStartInfo.RedirectStandardOutput = true;
				pStartInfo.CreateNoWindow = true;
			}
			
			Process proc = new Process();
			proc.StartInfo = pStartInfo;
			
			proc.Start();
			
			logBox.Clear();
			
			string output = "";
			
			if (hidden)
			{
				proc.BeginOutputReadLine();
				proc.BeginErrorReadLine();
			
				proc.OutputDataReceived += delegate(object sender, DataReceivedEventArgs e)
				{ 
					if (!string.IsNullOrEmpty(e.Data) || !string.IsNullOrWhiteSpace(e.Data))
						output += GetTimestamp() + "  " + e.Data + Environment.NewLine;
				};
				
				proc.ErrorDataReceived += delegate(object sender, DataReceivedEventArgs e)
				{
					if (!string.IsNullOrEmpty(e.Data) || !string.IsNullOrWhiteSpace(e.Data))
						output += GetTimestamp() + "  Error: " + e.Data + Environment.NewLine;
				};
			}
			
			proc.WaitForExit();
			
			logBox.AppendText(output);
		}
		
		public static String GetTimestamp()
		{
            return DateTime.Now.ToString("hh:mm:ss:");
        }
		
		void MainFormFormClosing(object sender, FormClosingEventArgs e)
		{
			switch (CheckSaveUpdate())
			{
				case DialogResult.Yes:
					Save();
					break;
				case DialogResult.No:
					break;
				case DialogResult.Cancel:
					e.Cancel = true;
					break;
			}
		}
		
		void CloseLogClick(object sender, EventArgs e)
		{
			logPanel.Hide();
		}
	}
}
