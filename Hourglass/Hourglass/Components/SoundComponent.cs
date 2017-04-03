using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;
using System.Collections;

namespace Hourglass
{
    class SoundComponent : Component
    {
        protected System.Drawing.Font mPlaceholderFont, mActiveFont;

        protected Label mLbEvent, mLBSound;
        protected Button mAddSound, mRemoveSound;
        protected List<ComboBox> mEvent, mSound;
        protected EventHandler mResize;
        protected Dictionary<string, List<string>> mCombos;
        protected Dictionary<string, List<UInt64>> mID;
        protected SplitterPanel mParent;

        public EventHandler Resize {
            get {
                return mResize;
            }
            set {
                mResize = value;
            }
        }


        public SplitterPanel Parent {
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
            mCombos = new Dictionary<string, List<string>>();
            mID = new Dictionary<string, List<UInt64>>();
            #region Component Creation

            mLbEvent = new Label();
            mLBSound = new Label();

            mEvent = new List<ComboBox>();
            mSound = new List<ComboBox>();

            mEvent.Add(new ComboBox());
            mSound.Add(new ComboBox());
            mAddSound = new Button();
            mRemoveSound = new Button();

            mGroupBox.Controls.Add(mLbEvent);
            mGroupBox.Controls.Add(mLBSound);
            mGroupBox.Controls.Add(mEvent[0]);
            mGroupBox.Controls.Add(mSound[0]);
            mGroupBox.Controls.Add(mAddSound);
            mGroupBox.Controls.Add(mRemoveSound);

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
            mEvent[0].SelectedValueChanged += UpdateSounds;

            mSound[0].Anchor = AnchorStyles.Top | AnchorStyles.Left;
            mSound[0].Location = new System.Drawing.Point(mEvent[0].Right, 49 + _yOffset);
            mSound[0].Size = new System.Drawing.Size(100, 24);
            mSound[0].DropDownStyle = ComboBoxStyle.DropDownList;

            mAddSound.Anchor = AnchorStyles.Top | AnchorStyles.Left;
            mAddSound.Location = new System.Drawing.Point(mEvent[0].Left, 49 + _yOffset + (mEvent[0].Height * mEvent.Count));
            mAddSound.Size = new System.Drawing.Size(mSound[0].Right - mEvent[mEvent.Count - 1].Left, 24);
            mAddSound.Text = "Add Sound";
            mAddSound.Click += AddEvent;

            mRemoveSound.Anchor = AnchorStyles.Top | AnchorStyles.Left;
            mRemoveSound.Location = new System.Drawing.Point(mEvent[0].Left, _yOffset + mAddSound.Bottom);
            mRemoveSound.Size = new System.Drawing.Size(mSound[0].Right - mEvent[mEvent.Count - 1].Left, 24);
            mRemoveSound.Text = "Remove Sound";
            mRemoveSound.Click += RemoveEvent;
            #endregion

            ReadSounds();

            foreach (var kvp in mCombos)
            {
                mEvent[0].Items.Add(kvp.Key);
            }
            mGroupBox.Text = "Sound Component";
            mGroupBox.Size = mGroupBox.PreferredSize;
            mGroupBox.AutoSize = true;
            OnMenuClick_Reset(null, null);
        }

        public void UpdateSounds(object sender, EventArgs e)
        {
            int index = mEvent.FindIndex(item => (sender as ComboBox).Top == item.Top);
            mSound[index].Items.Clear();
            if (mEvent[index].SelectedIndex >= 0)
            {
                foreach (String str in mCombos[mEvent[index].Text])
                {
                    mSound[index].Items.Add(str);
                }
            }
        }

        public void ReadSounds()
        {
            List<string>.Enumerator it = ResourceManager.Instance.Sounds.GetEnumerator();
            while (it.MoveNext())
            {
                if (it.Current != "Init.txt")
                {
                    try
                    {
                        using (StreamReader sr = new StreamReader(Settings.ProjectPath + ResourceManager.Instance.SoundDirectory + it.Current))
                        {
                            // Read the stream to a string, and write the string to the console.
                            String line = sr.ReadLine();
                            line = sr.ReadLine();
                            while (line != "")
                            {
                                String parse = line.Split('\t')[2];
                                List<String> chunks = new List<string>(parse.Split(new char[]{ '_' }, 2));
                                if (!mCombos.ContainsKey(chunks[0].ToLower()))
                                {
                                    mID.Add(chunks[0].ToLower(), new List<ulong>());
                                    mCombos.Add(chunks[0].ToLower(), new List<string>());
                                }
                                mCombos[chunks[0].ToLower()].Add(chunks[1]);
                                string uit = line.Split('\t')[1];
                                mID[chunks[0].ToLower()].Add(Convert.ToUInt64(uit));
                                line = sr.ReadLine();
                            }
                        }
                    }
                    catch (Exception e)
                    {
                        Console.WriteLine("The file could not be read:");
                        Console.WriteLine(e.Message);
                    }
                }
            }
        }

        public override void OnMenuClick_Reset(object sender, EventArgs e)
        {
            mEvent[0].SelectedIndex = -1;
            mSound[0].SelectedIndex = -1;
            for (int i = 1; i < mEvent.Count; i = 1)
            {
                mGroupBox.Controls.Remove(mEvent[i]);
                mGroupBox.Controls.Remove(mSound[i]);

                mEvent.RemoveAt(i);
                mSound.RemoveAt(i);

            }
            mAddSound.Location = new System.Drawing.Point(mEvent[mEvent.Count - 1].Left, mEvent[mEvent.Count - 1].Bottom);
            mRemoveSound.Location = new System.Drawing.Point(mEvent[0].Left, mAddSound.Bottom);
            if (mParent != null)
            {
                Resize.Invoke(mParent, e);
            }
            ReleaseControl();
        }

        public override void WriteData(BinaryWriter w)
        {
            base.WriteData(w);
            w.Write(mEvent.Count);
            for (int i = 0; i < mEvent.Count; i++)
            {
                switch (mEvent[i].Text.ToLower())
                {
                    case "sfx":
                    case "play":
                        w.Write((byte)0);
                        break;
                    case "pause":
                        w.Write((byte)1);
                        break;
                    case "resume":
                        w.Write((byte)2);
                        break;
                    case "stop":
                        w.Write((byte)3);
                        break;
                    default:
                        break;
                }
                w.Write(mID[mEvent[i].Text.ToLower()][mSound[i].SelectedIndex]);
            }
            byte term = 0;
            w.Write(term);
        }

        public override void ReadData(BinaryReader r)
        {
            int count = r.ReadInt32();
            for (int i = 0; i < count; i++)
            {
                if (mEvent.Count < i)
                {
                    AddEvent(null, null);
                }
                byte b = r.ReadByte();
                UInt64 id = r.ReadUInt64();
                switch (b)
                {
                    case 0:
                        if (mID["play"].Contains(id))
                            mEvent[i].SelectedText = "play";
                        else
                            mEvent[i].SelectedText = "sfx";
                        break;
                    case 1:
                        mEvent[i].SelectedText = "pause";
                        break;
                    case 2:
                        mEvent[i].SelectedText = "resume";
                        break;
                    case 3:
                        mEvent[i].SelectedText = "stop";
                        break;
                    default:
                        break;
                }
                mSound[i].SelectedIndex = mID[mEvent[i].SelectedText].IndexOf(id);
            }
        }

        public void AddEvent(object sender, EventArgs e)
        {
            mEvent.Add(new ComboBox());
            mSound.Add(new ComboBox());

            mGroupBox.Controls.Add(mEvent[mEvent.Count - 1]);
            mGroupBox.Controls.Add(mSound[mSound.Count - 1]);

            mEvent[mEvent.Count - 1].Anchor = AnchorStyles.Left | AnchorStyles.Top;
            mEvent[mEvent.Count - 1].Location = new System.Drawing.Point(6, 49 + (24 * (mEvent.Count - 1)));
            mEvent[mEvent.Count - 1].Size = new System.Drawing.Size(100, 24);
            mEvent[mEvent.Count - 1].DropDownStyle = ComboBoxStyle.DropDownList;
            mEvent[mEvent.Count - 1].SelectedValueChanged += UpdateSounds;

            mSound[mSound.Count - 1].Anchor = AnchorStyles.Top | AnchorStyles.Left;
            mSound[mSound.Count - 1].Location = new System.Drawing.Point(mEvent[mEvent.Count - 1].Right, mEvent[mEvent.Count - 1].Top);
            mSound[mSound.Count - 1].Size = new System.Drawing.Size(100, 24);
            mSound[mSound.Count - 1].DropDownStyle = ComboBoxStyle.DropDownList;

            mAddSound.Location = new System.Drawing.Point(mEvent[mEvent.Count - 1].Left, mEvent[mEvent.Count - 1].Bottom);
            mRemoveSound.Location = new System.Drawing.Point(mEvent[0].Left, mAddSound.Bottom);

            foreach (var kvp in mCombos)
            {
                mEvent[mEvent.Count - 1].Items.Add(kvp.Key);
            }
            Resize.Invoke(mParent, e);
        }

        public void RemoveEvent(object sender, EventArgs e)
        {
            mGroupBox.Controls.Remove(mEvent[mEvent.Count - 1]);
            mGroupBox.Controls.Remove(mSound[mSound.Count - 1]);

            mEvent.RemoveAt(mEvent.Count - 1);
            mSound.RemoveAt(mSound.Count - 1);

            mAddSound.Location = new System.Drawing.Point(mEvent[mEvent.Count - 1].Left, mEvent[mEvent.Count - 1].Bottom);
            mRemoveSound.Location = new System.Drawing.Point(mEvent[0].Left, mAddSound.Bottom);

            Resize.Invoke(mParent, e);
        }
    }
}