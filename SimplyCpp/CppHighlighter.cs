using System;
using System.IO;
using System.Xml;
using System.Drawing;
using FastColoredTextBoxNS;
using System.Text.RegularExpressions;

namespace SimplyCpp
{
	/// <summary>
	/// Highlighter to highlight C++ code in FastColoredTextBox
	/// </summary>
	public class CppHighlighter
	{
		// The FCTB control to run the highlighter on
		private FastColoredTextBox fctb;
		
		// Regex objects for the language
		private Regex r_com1, r_com2, r_com3;
		private Regex r_str;
		private Regex r_kwd;
		private Regex r_typ;
		private Regex r_lit;
		private Regex r_opr;
		private Regex r_pre;
		private Regex r_hed;
		
		// Styles for colors
		private Style s_com;
		private Style s_str;
		private Style s_kwd;
		private Style s_typ;
		private Style s_lit;
		private Style s_opr;
		private Style s_pre;
		private Style s_hed;
		
		public CppHighlighter(FastColoredTextBox fctb)
		{
			this.fctb = fctb;
			
			InitRegex();
			InitStyle();
		}
		
		private void InitRegex()
		{
			r_com1 = new Regex(@"//.*$",               RegexOptions.Multiline  | SyntaxHighlighter.RegexCompiledOption);
			r_com2 = new Regex(@"(/\*.*?\*/)|(/\*.*)", RegexOptions.Singleline | SyntaxHighlighter.RegexCompiledOption);
			r_com3 = new Regex(@"(/\*.*?\*/)|(.*\*/)", RegexOptions.Singleline | RegexOptions.RightToLeft 
			                                           | SyntaxHighlighter.RegexCompiledOption);
			
			r_str  = new Regex(@"("".*"")|('.')",      SyntaxHighlighter.RegexCompiledOption);
			
			r_kwd  = new Regex(@"\b(alignas|alignof|and|and_eq|asm|auto|bitand|bitor|bool|break|case|
                                  catch|char|char16_t|char32_t|class|compl|const|constexpr|const_cast|
								  continue|decltype|default|delete|do|double|dynamic_cast|else|enum|
								  explicit|export|extern|false|float|for|friend|goto|if|inline|int|long|
								  mutable|namespace|new|noexcept|not|not_eq|nullptr|operator|or|or_eq|
								  private|protected|public|register|reinterpret_cast|return|short|signed|
								  sizeof|static|static_assert|static_cast|struct|switch|template|this|
								  thread_local|throw|true|try|typedef|typeid|typename|union|unsigned|using|
								  virtual|void|volatile|wchar_t|while|xor|xor_eq|override|final)\b|#region\b|#endregion\b",
								
			                   SyntaxHighlighter.RegexCompiledOption | RegexOptions.IgnorePatternWhitespace);
			
			r_typ  = new Regex(@"\b(?<=(class|new|struct)\s)\s*\S*\b",                       SyntaxHighlighter.RegexCompiledOption);
			r_lit  = new Regex(@"\b-?(0x[\dA-Fa-f]+|\d*\.?\d+([Ee]-?\d+)?)\b",               SyntaxHighlighter.RegexCompiledOption);
			r_opr  = new Regex(@"[-+]{1,2}|!|<=?|>=?|={1,3}|&{1,2}|\|?\||\?|\*|\/|\~|\^|\%", SyntaxHighlighter.RegexCompiledOption);
			
			r_pre  = new Regex(@"#.*",                     SyntaxHighlighter.RegexCompiledOption);
			r_hed  = new Regex(@"(?<=(#include)\s)\s*\S*", SyntaxHighlighter.RegexCompiledOption);
		}
		
		private void InitStyle()
		{
			s_com = new TextStyle(GetBrushFromHex("#008000"), Brushes.Transparent, FontStyle.Regular);
			s_kwd = new TextStyle(GetBrushFromHex("#0000F0"), Brushes.Transparent, FontStyle.Regular);
			s_str = new TextStyle(GetBrushFromHex("#800000"), Brushes.Transparent, FontStyle.Regular);
			s_lit = new TextStyle(GetBrushFromHex("#660E7A"), Brushes.Transparent, FontStyle.Regular);
			s_typ = new TextStyle(GetBrushFromHex("#5c2699"), Brushes.Transparent, FontStyle.Regular);
			s_opr = new TextStyle(GetBrushFromHex("#000000"), Brushes.Transparent, FontStyle.Regular);
			s_pre = new TextStyle(GetBrushFromHex("#643820"), Brushes.Transparent, FontStyle.Regular);
			s_hed = new TextStyle(GetBrushFromHex("#808080"), Brushes.Transparent, FontStyle.Regular);
		}
		
		public void InitStyle(string xmlStyleDoc)
		{
			using (XmlReader reader = XmlReader.Create(xmlStyleDoc))
			{
				// TODO: FIX IN NEXT VERSION!
			}
		}
		
		private Brush GetBrushFromHex(string hexString)
		{
			if (!Regex.IsMatch(hexString, @"[#]([0-9]|[a-f]|[A-F]){6}\b"))
				throw new ArgumentException();
			
			int red   = int.Parse(hexString.Substring(1, 2), System.Globalization.NumberStyles.HexNumber);
			int green = int.Parse(hexString.Substring(3, 2), System.Globalization.NumberStyles.HexNumber);
			int blue  = int.Parse(hexString.Substring(5, 2), System.Globalization.NumberStyles.HexNumber);
			
			return new SolidBrush(Color.FromArgb(red, green, blue));
		}
		
		public void Highlight(Range range)
		{
			range.tb.CommentPrefix = "//";
			range.tb.LeftBracket = '(';
			range.tb.RightBracket = ')';
			range.tb.LeftBracket2 = '{';
			range.tb.RightBracket2 = '}';
			range.tb.BracketsHighlightStrategy = BracketsHighlightStrategy.Strategy2;

			range.tb.AutoIndentCharsPatterns = @"
                                                ^\s*[\w\.]+(\s\w+)?\s*(?<range>=)\s*(?<range>[^;]+);
                                                ^\s*(case|default)\s*[^:]*(?<range>:)\s*(?<range>[^;]+);
                                               ";
			
			range.ClearStyle(s_com, s_kwd, s_lit, s_opr, s_str, s_typ, s_pre, s_hed);

			range.SetStyle(s_com, r_com1);
			range.SetStyle(s_com, r_com2);
			range.SetStyle(s_com, r_com3);
			range.SetStyle(s_lit, r_lit);
			range.SetStyle(s_typ, r_typ);
			range.SetStyle(s_hed, r_hed);
			range.SetStyle(s_pre, r_pre);
			range.SetStyle(s_str, r_str);
			range.SetStyle(s_kwd, r_kwd);
			range.SetStyle(s_opr, r_opr);
			
			range.ClearFoldingMarkers();
			range.SetFoldingMarkers("{", "}");
			range.SetFoldingMarkers(@"/\*", @"\*/");
		}
		
		public void HighlightVisible()
		{
			Highlight(fctb.VisibleRange);
		}
		
		public void HighlightAll()
		{
			Highlight(fctb.Range);
		}
	}
}
