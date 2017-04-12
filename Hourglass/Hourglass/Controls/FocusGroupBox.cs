using System.Drawing;
using System.Windows.Forms;

namespace Hourglass {
	public class FocusGroupBox : GroupBox {
		private Color _borderColor = SystemColors.ActiveBorder;
		private float mPenWidth = 1;

		public Color BorderColor {
			get { return this._borderColor; }
			set { this._borderColor = value; }
		}

		public float BorderWidth {
			get {
				return mPenWidth;
			}
			set {
				mPenWidth = value;
			}
		}

		protected override void OnPaint(PaintEventArgs e) {
			//get the text size in groupbox
			Size tSize = TextRenderer.MeasureText(this.Text, this.Font);

			Rectangle textRect = new Rectangle();
			textRect.X = (textRect.X + 6);
			textRect.Width = tSize.Width + 6;
			textRect.Height = tSize.Height;

			Rectangle borderRect = e.ClipRectangle;

			if (textRect.Bottom > e.ClipRectangle.Top && textRect.Top < e.ClipRectangle.Bottom) {
				borderRect.Y = (borderRect.Y + (tSize.Height / 2));
				borderRect.Height = (borderRect.Height - (tSize.Height / 2));
			}

			//ControlPaint.DrawBorder(e.Graphics, borderRect, this._borderColor, ButtonBorderStyle.Solid);
			Pen borderPen = new Pen(BorderColor, BorderWidth);

			// Top border
			if (borderRect.Y <= ClientRectangle.Y + tSize.Height / 2) {
				e.Graphics.DrawLine(borderPen, new Point(borderRect.X, borderRect.Y), new Point(borderRect.X + borderRect.Width, borderRect.Y));
			}

			// Left Border
			if(borderRect.X <= ClientRectangle.X) {
				e.Graphics.DrawLine(borderPen, new Point(ClientRectangle.X, borderRect.Y), new Point(ClientRectangle.X, borderRect.Y + borderRect.Height));
			}

			// Right Border
			if(borderRect.X + borderRect.Width >= ClientRectangle.Width) {
				e.Graphics.DrawLine(borderPen, new Point(ClientRectangle.Width, borderRect.Y), new Point(ClientRectangle.Width, borderRect.Y + borderRect.Height));
			}

			// Bottom Border
			if(borderRect.Y + borderRect.Height >= ClientRectangle.Height) {
				e.Graphics.DrawLine(borderPen, new Point(borderRect.X, ClientRectangle.Height), new Point(borderRect.X + borderRect.Width, ClientRectangle.Height));
			}


			if(textRect.Bottom > e.ClipRectangle.Top && textRect.Top < e.ClipRectangle.Bottom) {
				e.Graphics.FillRectangle(new SolidBrush(this.BackColor), textRect);
				e.Graphics.DrawString(this.Text, this.Font, new SolidBrush(this.ForeColor), textRect);
			}
		}
	}
}
