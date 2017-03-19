using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Hourglass
{
    public partial class MenuButton : Button
    {
        [DefaultValue(null)]
        public ContextMenuStrip Menu { get; set; }

        [DefaultValue(false)]
        public bool ShowMenuUnderCursor { get; set; }

        [DefaultValue(20)]
        public int ArrowAreaWidth { get; set; }

        [DefaultValue(4)]
        public int ArrowMargin { get; set; }

        [DefaultValue(7)]
        public int ArrowWidth { get; set; }

        [DefaultValue(4)]
        public int ArrowHeight { get; set; }

        public MenuButton() : base()
        {
            Menu = null;
            ShowMenuUnderCursor = false;
            ArrowAreaWidth = 20;
            ArrowMargin = 4;
            ArrowWidth = 7;
            ArrowHeight = 4;
        }

        protected override void OnMouseDown(MouseEventArgs e)
        {
            if (Menu == null)
            {
                base.OnMouseDown(e);
            }
            else if (e.Button == MouseButtons.Left)
            {
                if (e.X >= ClientRectangle.Width - ArrowAreaWidth)
                {


                    Point menuLocation;
                    if (ShowMenuUnderCursor)
                    {
                        menuLocation = e.Location;
                    }
                    else
                    {
                        menuLocation = new Point(0, Height);
                    }

                    Menu.Show(this, menuLocation);
                }
                else
                {
                    base.OnMouseDown(e);
                }
            }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            if (Menu != null)
            {
                int arrowX = ClientRectangle.Width - ArrowAreaWidth + ArrowMargin;
                int arrowY = ClientRectangle.Height / 2 - 1;

                Brush brush = Enabled ? SystemBrushes.ControlText : SystemBrushes.ButtonShadow;
                Point[] arrows = new Point[] { new Point(arrowX, arrowY), new Point(arrowX + ArrowWidth, arrowY), new Point(arrowX + (ArrowWidth / 2), arrowY + ArrowHeight) };
                e.Graphics.FillPolygon(brush, arrows);

                float lineMargin = ClientRectangle.Height * 0.2f;
                e.Graphics.DrawLine(SystemPens.AppWorkspace, ClientRectangle.Width - ArrowAreaWidth - 1, lineMargin, ClientRectangle.Width - ArrowAreaWidth - 1, ClientRectangle.Height - lineMargin);
            }
        }
    }
}
