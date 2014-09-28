using System;
using System.Drawing;
using System.Windows.Forms;

namespace SimplyCpp
{
	/// <summary>
	/// Description of AboutForm.
	/// </summary>
	public partial class AboutForm : Form
	{
		public AboutForm()
		{
			//
			// The InitializeComponent() call is required for Windows Forms designer support.
			//
			InitializeComponent();
			
			ShowDialog();
		}
		
		void PictureBox1Click(object sender, EventArgs e)
		{
			
		}
		
		void Button1Click(object sender, EventArgs e)
		{
			Close();
		}
		
		void Button2Click(object sender, EventArgs e)
		{
			System.Diagnostics.Process.Start("http://goharsha.com/simplycpp");
		}
		
		protected override void WndProc(ref Message message)
		{
			const int WM_NCHITTEST = 0x0084;

			if (message.Msg == WM_NCHITTEST)
				return;

			base.WndProc(ref message);
}
	}
}
