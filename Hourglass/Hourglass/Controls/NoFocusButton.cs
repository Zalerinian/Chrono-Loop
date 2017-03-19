using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using System.Threading.Tasks;

namespace Hourglass
{
    public partial class NoFocusButton : Button
    {
        public NoFocusButton() : base()
        {
            InitializeComponent();

            this.SetStyle(ControlStyles.Selectable, false);
            this.MinimumSize = new Size(1, 1);
        }

        protected override bool ShowFocusCues
        {
            get
            {
                return false;
            }
        }
    }
}
