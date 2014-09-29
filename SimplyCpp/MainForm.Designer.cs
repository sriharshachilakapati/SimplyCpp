namespace SimplyCpp
{
	partial class MainForm
	{
		/// <summary>
		/// Designer variable used to keep track of non-visual components.
		/// </summary>
		private System.ComponentModel.IContainer components = null;
		
		/// <summary>
		/// Disposes resources used by the form.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing) {
				if (components != null) {
					components.Dispose();
				}
			}
			base.Dispose(disposing);
		}
		
		/// <summary>
		/// This method is required for Windows Forms designer support.
		/// Do not change the method contents inside the source code editor. The Forms designer might
		/// not be able to load this method if it was changed manually.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
			this.fastColoredTextBox1 = new FastColoredTextBoxNS.FastColoredTextBox();
			this.documentMap1 = new FastColoredTextBoxNS.DocumentMap();
			this.mainMenu1 = new System.Windows.Forms.MainMenu(this.components);
			this.ofd = new System.Windows.Forms.OpenFileDialog();
			this.sfd = new System.Windows.Forms.SaveFileDialog();
			this.logPanel = new System.Windows.Forms.Panel();
			this.logBox = new System.Windows.Forms.TextBox();
			this.panel1 = new System.Windows.Forms.Panel();
			this.label1 = new System.Windows.Forms.Label();
			this.CloseLog = new System.Windows.Forms.Button();
			this.contextMenu1 = new System.Windows.Forms.ContextMenu();
			this.fontDialog1 = new System.Windows.Forms.FontDialog();
			((System.ComponentModel.ISupportInitialize)(this.fastColoredTextBox1)).BeginInit();
			this.logPanel.SuspendLayout();
			this.panel1.SuspendLayout();
			this.SuspendLayout();
			// 
			// fastColoredTextBox1
			// 
			this.fastColoredTextBox1.AutoCompleteBrackets = true;
			this.fastColoredTextBox1.AutoCompleteBracketsList = new char[] {
						'(',
						')',
						'{',
						'}',
						'[',
						']',
						'\"',
						'\"',
						'\'',
						'\''};
			this.fastColoredTextBox1.AutoIndentCharsPatterns = "\r\n^\\s*[\\w\\.]+(\\s\\w+)?\\s*(?<range>=)\\s*(?<range>[^;]+);\r\n^\\s*(case|default)\\s*[^:]" +
			"*(?<range>:)\\s*(?<range>[^;]+);\r\n";
			this.fastColoredTextBox1.AutoScrollMinSize = new System.Drawing.Size(25, 20);
			this.fastColoredTextBox1.BackBrush = null;
			this.fastColoredTextBox1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.fastColoredTextBox1.CharHeight = 15;
			this.fastColoredTextBox1.CharWidth = 7;
			this.fastColoredTextBox1.CurrentLineColor = System.Drawing.Color.LightBlue;
			this.fastColoredTextBox1.Cursor = System.Windows.Forms.Cursors.IBeam;
			this.fastColoredTextBox1.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(180)))), ((int)(((byte)(180)))), ((int)(((byte)(180)))));
			this.fastColoredTextBox1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.fastColoredTextBox1.Font = new System.Drawing.Font("Consolas", 9.75F);
			this.fastColoredTextBox1.Hotkeys = resources.GetString("fastColoredTextBox1.Hotkeys");
			this.fastColoredTextBox1.IsReplaceMode = false;
			this.fastColoredTextBox1.LeftBracket = '(';
			this.fastColoredTextBox1.LeftBracket2 = '{';
			this.fastColoredTextBox1.Location = new System.Drawing.Point(0, 0);
			this.fastColoredTextBox1.Name = "fastColoredTextBox1";
			this.fastColoredTextBox1.Paddings = new System.Windows.Forms.Padding(0, 5, 0, 0);
			this.fastColoredTextBox1.RightBracket = ')';
			this.fastColoredTextBox1.RightBracket2 = '}';
			this.fastColoredTextBox1.SelectionColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(255)))));
			this.fastColoredTextBox1.Size = new System.Drawing.Size(626, 438);
			this.fastColoredTextBox1.TabIndex = 1;
			this.fastColoredTextBox1.Zoom = 100;
			this.fastColoredTextBox1.TextChanged += new System.EventHandler<FastColoredTextBoxNS.TextChangedEventArgs>(this.FastColoredTextBox1TextChanged);
			// 
			// documentMap1
			// 
			this.documentMap1.Dock = System.Windows.Forms.DockStyle.Right;
			this.documentMap1.ForeColor = System.Drawing.Color.SkyBlue;
			this.documentMap1.Location = new System.Drawing.Point(626, 0);
			this.documentMap1.Name = "documentMap1";
			this.documentMap1.Size = new System.Drawing.Size(158, 438);
			this.documentMap1.TabIndex = 4;
			this.documentMap1.Target = this.fastColoredTextBox1;
			this.documentMap1.Text = "documentMap1";
			// 
			// ofd
			// 
			this.ofd.Filter = "C/C++Source Files|*.c;*.cpp;*.h;*.hpp;*.inl";
			// 
			// sfd
			// 
			this.sfd.DefaultExt = "cpp";
			this.sfd.Filter = "C/C++Source Files|*.c;*.cpp;*.h;*.hpp;*.inl";
			// 
			// logPanel
			// 
			this.logPanel.Controls.Add(this.logBox);
			this.logPanel.Controls.Add(this.panel1);
			this.logPanel.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.logPanel.Location = new System.Drawing.Point(0, 438);
			this.logPanel.Name = "logPanel";
			this.logPanel.Size = new System.Drawing.Size(784, 124);
			this.logPanel.TabIndex = 5;
			// 
			// logBox
			// 
			this.logBox.BackColor = System.Drawing.SystemColors.ButtonHighlight;
			this.logBox.Dock = System.Windows.Forms.DockStyle.Fill;
			this.logBox.Font = new System.Drawing.Font("Consolas", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.logBox.Location = new System.Drawing.Point(0, 23);
			this.logBox.Multiline = true;
			this.logBox.Name = "logBox";
			this.logBox.ReadOnly = true;
			this.logBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.logBox.Size = new System.Drawing.Size(784, 101);
			this.logBox.TabIndex = 3;
			// 
			// panel1
			// 
			this.panel1.Controls.Add(this.label1);
			this.panel1.Controls.Add(this.CloseLog);
			this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
			this.panel1.Location = new System.Drawing.Point(0, 0);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(784, 23);
			this.panel1.TabIndex = 5;
			// 
			// label1
			// 
			this.label1.BackColor = System.Drawing.SystemColors.ActiveCaption;
			this.label1.Dock = System.Windows.Forms.DockStyle.Top;
			this.label1.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label1.ForeColor = System.Drawing.SystemColors.ControlText;
			this.label1.Location = new System.Drawing.Point(0, 0);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(762, 22);
			this.label1.TabIndex = 2;
			this.label1.Text = " Messages";
			this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// CloseLog
			// 
			this.CloseLog.Dock = System.Windows.Forms.DockStyle.Right;
			this.CloseLog.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.CloseLog.Location = new System.Drawing.Point(762, 0);
			this.CloseLog.Name = "CloseLog";
			this.CloseLog.Size = new System.Drawing.Size(22, 23);
			this.CloseLog.TabIndex = 4;
			this.CloseLog.Text = "×";
			this.CloseLog.TextAlign = System.Drawing.ContentAlignment.TopCenter;
			this.CloseLog.UseVisualStyleBackColor = true;
			this.CloseLog.Click += new System.EventHandler(this.CloseLogClick);
			// 
			// fontDialog1
			// 
			this.fontDialog1.AllowScriptChange = false;
			this.fontDialog1.AllowSimulations = false;
			this.fontDialog1.AllowVectorFonts = false;
			this.fontDialog1.AllowVerticalFonts = false;
			this.fontDialog1.FixedPitchOnly = true;
			this.fontDialog1.ShowEffects = false;
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(784, 562);
			this.Controls.Add(this.fastColoredTextBox1);
			this.Controls.Add(this.documentMap1);
			this.Controls.Add(this.logPanel);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Menu = this.mainMenu1;
			this.Name = "MainForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "SimplyCpp";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainFormFormClosing);
			this.Load += new System.EventHandler(this.MainFormLoad);
			((System.ComponentModel.ISupportInitialize)(this.fastColoredTextBox1)).EndInit();
			this.logPanel.ResumeLayout(false);
			this.logPanel.PerformLayout();
			this.panel1.ResumeLayout(false);
			this.ResumeLayout(false);
		}
		private System.Windows.Forms.FontDialog fontDialog1;
		private System.Windows.Forms.ContextMenu contextMenu1;
		private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.Button CloseLog;
		private System.Windows.Forms.TextBox logBox;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Panel logPanel;
		private System.Windows.Forms.SaveFileDialog sfd;
		private System.Windows.Forms.OpenFileDialog ofd;
		private System.Windows.Forms.MainMenu mainMenu1;
		private FastColoredTextBoxNS.DocumentMap documentMap1;
		private FastColoredTextBoxNS.FastColoredTextBox fastColoredTextBox1;
	}
}
