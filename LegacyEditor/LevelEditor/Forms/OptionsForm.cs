using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LevelEditor
{
    public partial class OptionsForm : Form
    {
        public OptionsForm()
        {
            InitializeComponent();
        }

        private void btnSoundbankSelector_Click(object sender, EventArgs e)
        {
            OpenFileDialog o = new OpenFileDialog();
            o.Filter = "Text Files (*.txt)|*.txt";
            o.FilterIndex = 1;
            o.Title = "Select the text file containing information about the soundbank...";
            if(o.ShowDialog() == DialogResult.OK)
            {
                tbSoundbankText.Text = o.FileName;
            }
        }
    }
}
