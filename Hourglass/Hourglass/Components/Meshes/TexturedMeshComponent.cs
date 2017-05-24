using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;

namespace Hourglass
{
	public class TexturedMeshComponent : MeshComponent
	{
		protected ComboBox mTexture, mEmissive, mSpecular, mNormal;
		protected Label mLbDiffuse, mLbEmissive, mLbSpecular, mLbNormal;

		public TexturedMeshComponent(int _yOffset = 0) : base(131 + _yOffset)
		{
			mType = ComponentType.TexturedMesh;

			#region Component Creation
			mLbDiffuse = new Label();
			mLbEmissive = new Label();
			mLbSpecular = new Label();
			mLbNormal = new Label();

			mTexture = new ComboBox();
			mEmissive = new ComboBox();
			mSpecular = new ComboBox();
			mNormal = new ComboBox();

			mShape = new TexturedShape();

            mGroupBox.Controls.Add(mLbDiffuse);
            mGroupBox.Controls.Add(mLbEmissive);
			mGroupBox.Controls.Add(mLbSpecular);
			mGroupBox.Controls.Add(mLbNormal);
			mGroupBox.Controls.Add(mTexture);
			mGroupBox.Controls.Add(mEmissive);
			mGroupBox.Controls.Add(mSpecular);
			mGroupBox.Controls.Add(mNormal);
			#endregion

			#region Component Setup
			int ContentWidth = (mGroupBox.Size - mGroupBox.Padding.Size - mGroupBox.Margin.Size).Width;

			mLbDiffuse.AutoSize = true;
			mLbDiffuse.Location = new System.Drawing.Point(6, 21 + _yOffset);
			mLbDiffuse.Name = "mLbDiffuse";
			mLbDiffuse.Text = "Diffuse Texture";

			mLbEmissive.AutoSize = true;
			mLbEmissive.Location = new System.Drawing.Point(6, 54 + _yOffset);
			mLbEmissive.Name = "mLbEmissive";
			mLbEmissive.Text = "Emissive Texture";

			mLbSpecular.AutoSize = true;
			mLbSpecular.Location = new System.Drawing.Point(6, 87 + _yOffset);
			mLbSpecular.Name = "mLbSpecular";
			mLbSpecular.Text = "Specular Texture";

			mLbNormal.AutoSize = true;
			mLbNormal.Location = new System.Drawing.Point(6, 120 + _yOffset);
			mLbNormal.Name = "mLbNormal";
			mLbNormal.Text = "Normal Texture";

			mTexture.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			mTexture.Location = new System.Drawing.Point(100, 19 + _yOffset);
			mTexture.Size = new System.Drawing.Size(ContentWidth - mTexture.Left, 24);
			mTexture.DropDownStyle = ComboBoxStyle.DropDownList;

			mEmissive.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			mEmissive.Location = new System.Drawing.Point(100, 52 + _yOffset);
			mEmissive.Size = new System.Drawing.Size(ContentWidth - mEmissive.Left, 24);
			mEmissive.DropDownStyle = ComboBoxStyle.DropDownList;

			mSpecular.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			mSpecular.Location = new System.Drawing.Point(100, 84 + _yOffset);
			mSpecular.Size = new System.Drawing.Size(ContentWidth - mSpecular.Left, 24);
			mSpecular.DropDownStyle = ComboBoxStyle.DropDownList;

			mNormal.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			mNormal.Location = new System.Drawing.Point(100, 117 + _yOffset);
			mNormal.Size = new System.Drawing.Size(ContentWidth - mNormal.Left, 24);
			mNormal.DropDownStyle = ComboBoxStyle.DropDownList;


			{
				List<string>.Enumerator it = ResourceManager.Instance.Textures.GetEnumerator();
				while (it.MoveNext())
				{
					mTexture.Items.Add(it.Current);
					mEmissive.Items.Add(it.Current);
					mSpecular.Items.Add(it.Current);
					mNormal.Items.Add(it.Current);
				}
			}

			mMesh.SelectedIndexChanged += OnMeshSelectionChange;
			mTexture.SelectedIndexChanged += OnTextureSelectionChange;

			#endregion

			mGroupBox.Size = mGroupBox.PreferredSize;
			OnMenuClick_Reset(null, null);
		}

		protected void OnMeshSelectionChange(object sender, EventArgs e)
		{
			if(!string.IsNullOrWhiteSpace(mMesh.Text))
			{
				((TexturedShape)mShape).Load(Settings.ProjectPath + ResourceManager.Instance.ResourceDirectory + mMesh.Text);
				//mShape.FillBuffers();
			}
			ReleaseControl();
		}

		protected void OnTextureSelectionChange(object sender, EventArgs e)
		{
			if(!string.IsNullOrWhiteSpace(mTexture.Text))
			{
				((TexturedShape)mShape).SetTexture(TexturedShape.TextureType.Diffuse, Settings.ProjectPath + ResourceManager.Instance.ResourceDirectory + mTexture.Text);
			}
			ReleaseControl();
		}

		public override void OnMenuClick_Delete(object sender, EventArgs e)
		{
			base.OnMenuClick_Delete(sender, e);
			mShape.Dispose();
		}

		public override void OnMenuClick_Reset(object sender, EventArgs e)
		{
			base.OnMenuClick_Reset(sender, e);
			mTexture.SelectedIndex = -1;
			mEmissive.SelectedIndex = -1;
			mTransparency.Value = 1;
		}

		public override void WriteData(BinaryWriter w)
		{
			base.WriteData(w);
			string s = (".." + ResourceManager.Instance.ResourceDirectory + mTexture.Text);
			w.Write(s.Length + 1);
			w.Write(s.ToCharArray());
			byte term = 0;
			w.Write(term);

			s = (".." + ResourceManager.Instance.ResourceDirectory + mEmissive.Text);
			w.Write(s.Length + 1);
			w.Write(s.ToCharArray());
			w.Write(term);

			s = ".." + ResourceManager.Instance.ResourceDirectory + mSpecular.Text;
			w.Write(s.Length + 1);
			w.Write(s.ToCharArray());
			w.Write(term);

			s = ".." + ResourceManager.Instance.ResourceDirectory + mNormal.Text;
			w.Write(s.Length + 1);
			w.Write(s.ToCharArray());
			w.Write(term);
		}

		public override void ReadData(BinaryReader r, int _version)
		{
			base.ReadData(r, _version);
			string filename = new string(r.ReadChars(r.ReadInt32() - 1));
			r.ReadByte(); // The null terminator breaks things in C#, but is necessary in C++, so we need to skip it in C#
			filename = filename.Substring(filename.LastIndexOf("\\") + 1);
			int index = mTexture.Items.IndexOf(filename);
			if (index >= 0)
			{
				mTexture.SelectedIndex = index;
			}
			else
			{
				((TexturedShape)mShape).Load("Assets\\Error.obj");
				((TexturedShape)mShape).SetTexture(TexturedShape.TextureType.Diffuse, "Assets\\red.png");
				mTexture.Text = "Error locating diffuse texture.";
			}

			if(_version >= 2) {
				filename = new string(r.ReadChars(r.ReadInt32() - 1));
				r.ReadByte(); // The null terminator breaks things in C#, but is necessary in C++, so we need to skip it in C#
				filename = filename.Substring(filename.LastIndexOf("\\") + 1);
				index = mEmissive.Items.IndexOf(filename);
				if (index >= 0) {
					mEmissive.SelectedIndex = index;
				} else {
					mEmissive.Text = "Error locating emissive texture.";
				}
			}
			if(_version >= 6) {
				filename = new string(r.ReadChars(r.ReadInt32() - 1));
				r.ReadByte(); // Ignore null terminator.
				filename = filename.Substring(filename.LastIndexOf("\\") + 1);
				index = mSpecular.Items.IndexOf(filename);
				if(index >= 0) {
					mSpecular.SelectedIndex = index;
				} else {
					mSpecular.Text = "Couldn't find " + filename;
				}
			}
			if (_version >= 7) {
				filename = new string(r.ReadChars(r.ReadInt32() - 1));
				r.ReadByte(); // Ignore null terminator.
				filename = filename.Substring(filename.LastIndexOf("\\") + 1);
				index = mNormal.Items.IndexOf(filename);
				if (index >= 0) {
					mNormal.SelectedIndex = index;
				} else {
					mNormal.Text = "Couldn't find " + filename;
				}
			}
		}

        public int CheckForTexture(string _object) {
            return CheckForValue(mTexture, _object);
        }

		public int CheckForEmissive(string _object) {
			return CheckForValue(mEmissive, _object);
		}

        public void SelectTexture(int _index) {
            if(_index > 0 && _index < mTexture.Items.Count) {
                mTexture.SelectedIndex = _index;
            }
        }

		public void SelectEmissive(int _index) {
			if (_index > 0 && _index < mEmissive.Items.Count) {
				mEmissive.SelectedIndex = _index;
			}
		}

		public override void CopyData(ref Component _other) {
			if (!(_other is TexturedMeshComponent)) {
				throw new InvalidDataException("Given component does not match the calling type.");
			}
			base.CopyData(ref _other);
			TexturedMeshComponent comp = _other as TexturedMeshComponent;
			comp.mTexture.SelectedIndex = mTexture.SelectedIndex;
			comp.mEmissive.SelectedIndex = mEmissive.SelectedIndex;
			comp.mSpecular.SelectedIndex = mSpecular.SelectedIndex;
			comp.mNormal.SelectedIndex = mNormal.SelectedIndex;
		}
	}
}
