using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;

namespace Hourglass
{
	public class TexturedMeshComponent : MeshComponent
	{
		protected ComboBox mTexture = new ComboBox();
		protected Label mLbDiffuse, mLbEmissive;

		public TexturedMeshComponent(int _yOffset = 0) : base(_yOffset)
		{
			mType = ComponentType.TexturedMesh;

			#region Component Creation
			mLbDiffuse = new Label();

			mShape = new TexturedShape();

            mGroupBox.Controls.Add(mLbDiffuse);
			mGroupBox.Controls.Add(mTexture);
			#endregion

			#region Component Setup
			int ContentWidth = (mGroupBox.Size - mGroupBox.Padding.Size - mGroupBox.Margin.Size).Width;

			mLbDiffuse.AutoSize = true;
			mLbDiffuse.Location = new System.Drawing.Point(6, 84 + _yOffset);
			mLbDiffuse.Name = "mLbDiffuse";
			mLbDiffuse.Text = "Diffuse Texture";

			mTexture.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			mTexture.Location = new System.Drawing.Point(90, 80 + _yOffset);
			mTexture.Size = new System.Drawing.Size(ContentWidth - mTexture.Left, 24);
			mTexture.DropDownStyle = ComboBoxStyle.DropDownList;

			{
				List<string>.Enumerator it = ResourceManager.Instance.Textures.GetEnumerator();
				while (it.MoveNext())
				{
					mTexture.Items.Add(it.Current);
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
				mShape.FillBuffers();
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
				mMesh.Text = "Error locating old mesh";
			}
		}

        public int CheckForTexture(string _object) {
            return CheckForValue(mTexture, _object);
        }

        public void SelectTexture(int _index) {
            if(_index > 0 && _index < mTexture.Items.Count) {
                mTexture.SelectedIndex = _index;
            }
        }
    }
}
