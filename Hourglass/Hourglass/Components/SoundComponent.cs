using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;

namespace Hourglass
{
    class SoundComponent : Component
    {
        protected System.Drawing.Font mPlaceholderFont, mActiveFont;

        protected Label mLbEvent, mLBSound;
        protected Button mAddSound;
        protected List<ComboBox> mEvent, mSound;
        protected EventHandler mResize;
        protected Form mParent;

        public EventHandler Resize {
            get {
                return mResize;
            }
            set {
                mResize = value;
            }
        }


        public Form Parent {
            get {
                return mParent;
            }
            set {
                mParent = value;
            }
        }

        public SoundComponent(int _yOffset = 0) : base()
        {
            mType = ComponentType.Code;
            #region Component Creation

            mLbEvent = new Label();
            mLBSound = new Label();

            mEvent = new List<ComboBox>();
            mSound = new List<ComboBox>();

            mEvent.Add(new ComboBox());
            mSound.Add(new ComboBox());
            mAddSound = new Button();

            mGroupBox.Controls.Add(mLbEvent);
            mGroupBox.Controls.Add(mLBSound);
            mGroupBox.Controls.Add(mEvent[0]);
            mGroupBox.Controls.Add(mSound[0]);
            mGroupBox.Controls.Add(mAddSound);

            #endregion

            #region Component Setup
            int ContentWidth = (mGroupBox.Size - mGroupBox.Padding.Size - mGroupBox.Margin.Size).Width;

            // Fonts
            mPlaceholderFont = new System.Drawing.Font("Calibri", 8.25f, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            mActiveFont = new System.Drawing.Font("Calibri", 8.25f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));

            mLbEvent.AutoSize = true;
            mLbEvent.Location = new System.Drawing.Point(6, 26);
            mLbEvent.Name = "mLbEvent";
            mLbEvent.Text = "Event";

            mLBSound.Visible = true;
            mLBSound.AutoSize = true;
            mLBSound.Location = new System.Drawing.Point(90, 26);
            mLBSound.Name = "mLbSound";
            mLBSound.Text = "Sound";

            mEvent[0].Anchor = AnchorStyles.Left | AnchorStyles.Top;
            mEvent[0].Location = new System.Drawing.Point(6, 49 + _yOffset);
            mEvent[0].Size = new System.Drawing.Size(100, 24);
            mEvent[0].DropDownStyle = ComboBoxStyle.DropDownList;

            mSound[0].Visible = true;
            mSound[0].Anchor = AnchorStyles.Top | AnchorStyles.Left;
            mSound[0].Location = new System.Drawing.Point(mEvent[0].Right, 49 + _yOffset);
            mSound[0].Size = new System.Drawing.Size(100, 24);
            mSound[0].DropDownStyle = ComboBoxStyle.DropDownList;

            mAddSound.Anchor = AnchorStyles.Top | AnchorStyles.Left;
            mAddSound.Location = new System.Drawing.Point(mEvent[0].Left, 49 + _yOffset + mEvent[0].Height);
            mAddSound.Size = new System.Drawing.Size(mSound[0].Right - mEvent[0].Left, 24);
            mAddSound.Text = "Add Sound";
            mAddSound.Click += UpdateEvent;


            #endregion

            List<string>.Enumerator it = ResourceManager.Instance.Sounds.GetEnumerator();
            while (it.MoveNext())
            {
                mEvent[0].Items.Add(it.Current);
                mSound[0].Items.Add(it.Current);
            }

            mGroupBox.Text = "Sound Component";
            mGroupBox.Size = mGroupBox.PreferredSize;
            mGroupBox.AutoSize = true;
            OnMenuClick_Reset(null, null);
        }

        public override void OnMenuClick_Reset(object sender, EventArgs e)
        {
            mEvent[0].SelectedIndex = -1;
            mSound[0].SelectedIndex = -1;
            ReleaseControl();
        }

        public override void WriteData(BinaryWriter w)
        {
            base.WriteData(w);
            string s = ".." + ResourceManager.Instance.ActionDirectory + mSound[0].Text;
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
            int index = mSound[0].Items.IndexOf(filename);
            if (index >= 0)
            {
                mSound[0].SelectedIndex = index;
            }
        }

        public void UpdateEvent(object sender, EventArgs e)
        {
        }
    }
}