using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Hourglass
{
	public class TexturedMeshComponent : MeshComponent
	{
		private ComboBox mTexture = new ComboBox();
		private TexturedShape mShape = new TexturedShape();

		public TexturedMeshComponent(int _yOffset = 0) : base(_yOffset)
		{
			#region Component Creation
			mGroupBox.Controls.Add(mTexture);

			#endregion

			#region Component Setup
			int ContentWidth = (mGroupBox.Size - mGroupBox.Padding.Size - mGroupBox.Margin.Size).Width;

			mTexture.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			mTexture.Location = new System.Drawing.Point(90, 51);
			mTexture.Size = new System.Drawing.Size(ContentWidth - mTexture.Left, 24);
			mTexture.DropDownStyle = ComboBoxStyle.DropDownList;

			{
				List<string>.Enumerator it = ResourceManager.Instance.Objects.GetEnumerator();
				while(it.MoveNext())
				{
					mMesh.Items.Add(it.Current);
				}
			}

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
		}

		protected void OnMeshSelectionChange(object sender, EventArgs e)
		{
			if(!string.IsNullOrWhiteSpace(mMesh.Text))
			{
				mShape.Load(Settings.ProjectPath + ResourceManager.Instance.ResourceDirectory + mMesh.Text);
				mShape.FillBuffers();
				Renderer.Instance.RemoveShape(mShape);
				Renderer.Instance.AddShape(mShape);
			}
			else
			{
				Renderer.Instance.RemoveShape(mShape);
			}
		}

		protected void OnTextureSelectionChange(object sender, EventArgs e)
		{
			if(!string.IsNullOrWhiteSpace(mTexture.Text))
			{
				mShape.SetTexture(TexturedShape.TextureType.Diffuse, Settings.ProjectPath + ResourceManager.Instance.ResourceDirectory + mTexture.Text);
			}
		}

		protected override void OnMenuClick_Delete(object sender, EventArgs e)
		{
			base.OnMenuClick_Delete(sender, e);
			Renderer.Instance.RemoveShape(mShape);
			mShape.Dispose();
		}

		protected override void OnMenuClick_Reset(object sender, EventArgs e)
		{
			base.OnMenuClick_Reset(sender, e);
			mTexture.SelectedIndex = -1;
		}
	}
}
