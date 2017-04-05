using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;

namespace Hourglass
{
    public class CodeComponent : Component
    {
        protected System.Drawing.Font mPlaceholderFont, mActiveFont;

        protected Label mLbCode;
        protected ComboBox mCode;

        public CodeComponent(int _yOffset = 0) : base()
        {
            mType = ComponentType.Code;
            #region Component Creation

            mLbCode = new Label();

            mCode = new ComboBox();

            mGroupBox.Controls.Add(mLbCode);
            mGroupBox.Controls.Add(mCode);

            #endregion

            #region Component Setup
            int ContentWidth = (mGroupBox.Size - mGroupBox.Padding.Size - mGroupBox.Margin.Size).Width;

            // Fonts
            mPlaceholderFont = new System.Drawing.Font("Calibri", 8.25f, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            mActiveFont = new System.Drawing.Font("Calibri", 8.25f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));

            mLbCode.AutoSize = true;
            mLbCode.Location = new System.Drawing.Point(6, 26);
            mLbCode.Name = "mLbCode";
            mLbCode.Text = "Code File";

            mCode.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
            mCode.Location = new System.Drawing.Point(90, 21 + _yOffset);
            mCode.Size = new System.Drawing.Size(ContentWidth - mCode.Left, 24);
            mCode.DropDownStyle = ComboBoxStyle.DropDownList;

            #endregion

            List<string>.Enumerator it = ResourceManager.Instance.Codes.GetEnumerator();
            while (it.MoveNext())
            {
                mCode.Items.Add(it.Current);
            }

            mGroupBox.Text = "Code Component";
            mGroupBox.Size = mGroupBox.PreferredSize;
            OnMenuClick_Reset(null, null);
        }

        public override void OnMenuClick_Reset(object sender, EventArgs e)
        {
            mCode.SelectedIndex = -1;
            ReleaseControl();
        }

        public override void WriteData(BinaryWriter w)
        {
            base.WriteData(w);
            string s = mCode.Text;
            w.Write(s.Length + 1);
            w.Write(s.ToCharArray());
            byte term = 0;
            w.Write(term);
        }

        public override void ReadData(BinaryReader r)
        {
            string filename = new string(r.ReadChars(r.ReadInt32() - 1));
            r.ReadByte(); // The null terminator breaks things in C#, but is necessary in C++, so we need to skip it in C#
            filename = filename.Substring(filename.LastIndexOf("\\") + 1);
            int index = mCode.Items.IndexOf(filename);
            if (index >= 0)
            {
                mCode.SelectedIndex = index;
            }
        }
    }
}