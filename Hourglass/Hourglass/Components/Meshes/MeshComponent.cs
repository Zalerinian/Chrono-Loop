using System;
using System.IO;
using System.Windows.Forms;
using System.Collections.Generic;

namespace Hourglass
{
    public class MeshComponent : Component, IRenderable
    {
        protected System.Drawing.Font mPlaceholderFont, mActiveFont;

        protected Label mLbMesh, mLbTransparency;
		protected ComboBox mMesh;
		protected RenderShape mShape = null;
		protected NumericUpDown mTransparency;

		public RenderShape Shape {
			get {
				return mShape;
			}
		}

        public MeshComponent(int _yOffset = 0) : base()
        {
            #region Component Creation

            mLbMesh = new Label();
			mLbTransparency = new Label();

            mMesh = new ComboBox();

			mTransparency = new NumericUpDown();

            mGroupBox.Controls.Add(mLbMesh);
			mGroupBox.Controls.Add(mLbTransparency);
			mGroupBox.Controls.Add(mTransparency);
            mGroupBox.Controls.Add(mMesh);

            #endregion

            #region Component Setup
            int ContentWidth = (mGroupBox.Size - mGroupBox.Padding.Size - mGroupBox.Margin.Size).Width;

            // Fonts
            mPlaceholderFont = new System.Drawing.Font("Calibri", 8.25f, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            mActiveFont = new System.Drawing.Font("Calibri", 8.25f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));

			mLbTransparency.AutoSize = true; // Unneccessary?
			mLbTransparency.Text = "Transparency";
			mLbTransparency.Location = new System.Drawing.Point(6, 26);
			mLbTransparency.Name = "mLbTransparency";
			mTransparency.Minimum = 0;
			mTransparency.Maximum = 1;
			mTransparency.Increment = (decimal)0.05f;
			mTransparency.DecimalPlaces = 2;

            mLbMesh.AutoSize = true;
            mLbMesh.Location = new System.Drawing.Point(6, 55);
            mLbMesh.Name = "mLbMesh";
            mLbMesh.Text = "Mesh File";

			mTransparency.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			mTransparency.Location = new System.Drawing.Point(90, 21);
			mTransparency.Size = new System.Drawing.Size(ContentWidth - mTransparency.Left, 24);

			mMesh.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			mMesh.Location = new System.Drawing.Point(90, 50 + _yOffset);
			mMesh.Size = new System.Drawing.Size(ContentWidth - mMesh.Left, 24);
			mMesh.DropDownStyle = ComboBoxStyle.DropDownList;

            {
                List<string>.Enumerator it = ResourceManager.Instance.Objects.GetEnumerator();
                while (it.MoveNext()) {
                    mMesh.Items.Add(it.Current);
                }
            }

            #endregion

            mGroupBox.Text = "Mesh Component";
            mGroupBox.Size = mGroupBox.PreferredSize;
            //OnMenuClick_Reset(null, null);
        }

        public override void OnMenuClick_Reset(object sender, EventArgs e)
        {
            mMesh.SelectedIndex = -1;
			if(mShape != null)
			{
				mShape.Dispose();
			}
			mTransparency.Value = 1;
			ReleaseControl();
		}

		public override void WriteData(BinaryWriter w)
		{
			base.WriteData(w);
			w.Write((float)mTransparency.Value);
			string s = ".." + ResourceManager.Instance.ResourceDirectory + mMesh.Text;
			w.Write(s.Length + 1);
			w.Write(s.ToCharArray());
			byte term = 0;
			w.Write(term);
		}

		public override void ReadData(BinaryReader r, int _version)
		{
			if(_version >= 2) {
				mTransparency.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
			}
			string filename = new string(r.ReadChars(r.ReadInt32() - 1));
			r.ReadByte(); // The null terminator breaks things in C#, but is necessary in C++, so we need to skip it in C#
			filename = filename.Substring(filename.LastIndexOf("\\") + 1);
			int index = mMesh.Items.IndexOf(filename);
			if (index >= 0)
			{
				mMesh.SelectedIndex = index;
			}
		}

        protected int CheckForValue(ComboBox _box, string _value) {
            for(int i = 0; i < _box.Items.Count; ++i) {
                if((string)_box.Items[i] == _value) {
                    return i;
                }
            }
            return -1;
        }

        public int CheckForMesh(string _object) {
            return CheckForValue(mMesh, _object);
        }

        public void SelectMesh(int _index) {
            if (_index > 0 && _index < mMesh.Items.Count) {
                mMesh.SelectedIndex = _index;
            }
        }

    }
}