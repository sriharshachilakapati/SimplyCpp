using System;
using System.Windows.Forms;

namespace SimplyCpp
{
	/// <summary>
	/// Class with program entry point.
	/// </summary>
	internal sealed class Program
	{
		/// <summary>
		/// Program entry point.
		/// </summary>
		[STAThread]
		private static void Main(string[] args)
		{
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			if (args.Length > 0)
				Application.Run(new MainForm(args[0]));
			else
				Application.Run(new MainForm());
		}
		
	}
}
