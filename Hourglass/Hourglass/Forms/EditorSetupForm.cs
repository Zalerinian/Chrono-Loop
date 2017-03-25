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
	public partial class EditorSetupForm : Form
	{
		public EditorSetupForm()
		{
			InitializeComponent();
		}

		private void rtbBannerEnter(object sender, EventArgs e)
		{
			tbProjDir.Select();
		}

		private void btnSelectDir_Click(object sender, EventArgs e)
		{
			FolderBrowserDialog o = new FolderBrowserDialog();
			o.ShowNewFolderButton = false;
			if (o.ShowDialog() == DialogResult.OK)
			{
				tbProjDir.Text = o.SelectedPath;
			}
		}
	}
}
