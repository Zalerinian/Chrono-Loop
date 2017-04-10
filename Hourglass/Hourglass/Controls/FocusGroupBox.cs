using System.Drawing;
using System.Windows.Forms;

namespace Hourglass {
	public class FocusGroupBox : GroupBox {
		private Color _borderColor = SystemColors.ActiveBorder;

		public Color BorderColor {
			get { return this._borderColor; }
			set { this._borderColor = value; }
		}

		protected override void OnPaint(PaintEventArgs e) {
			//get the text size in groupbox
			Size tSize = TextRenderer.MeasureText(this.Text, this.Font);

			Rectangle borderRect = e.ClipRectangle;
			borderRect.Y = (borderRect.Y + (tSize.Height / 2));
			borderRect.Height = (borderRect.Height - (tSize.Height / 2));
			ControlPaint.DrawBorder(e.Graphics, borderRect, this._borderColor, ButtonBorderStyle.Solid);

			Rectangle textRect = e.ClipRectangle;
			textRect.X = (textRect.X + 6);
			textRect.Width = tSize.Width + 6;
			textRect.Height = tSize.Height;
			e.Graphics.FillRectangle(new SolidBrush(this.BackColor), textRect);
			e.Graphics.DrawString(this.Text, this.Font, new SolidBrush(this.ForeColor), textRect);
		}
	}
}
