using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Hourglass
{
	public class ColoredMeshComponent : MeshComponent
	{
		private Button mColor = new Button();

		public ColoredMeshComponent(int _yOffset = 0) : base(_yOffset)
		{
			#region Component Creation
			mShape = new ColoredShape();

			mGroupBox.Controls.Add(mColor);

			#endregion

			#region Component Setup
			int ContentWidth = (mGroupBox.Size - mGroupBox.Padding.Size - mGroupBox.Margin.Size).Width;

			mColor.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			mColor.Location = new System.Drawing.Point(90, 51);
			mColor.Size = new System.Drawing.Size(ContentWidth - mColor.Left, 24);
			mColor.Font = new System.Drawing.Font("Microsoft Sans Serif", 10, System.Drawing.FontStyle.Bold);
			mColor.Text = "Mesh Color";
			mColor.Click += OnSelectColor;

			{
				List<string>.Enumerator it = ResourceManager.Instance.Objects.GetEnumerator();
				while (it.MoveNext())
				{
					mMesh.Items.Add(it.Current);
				}
			}

			mMesh.SelectedIndexChanged += OnMeshSelectionChange;

			#endregion
		}

		protected void OnMeshSelectionChange(object sender, EventArgs e)
		{
			if(!string.IsNullOrWhiteSpace(mMesh.Text))
			{
				((ColoredShape)mShape).Load(Settings.ProjectPath + ResourceManager.Instance.ResourceDirectory + mMesh.Text, mColor.BackColor);
				mShape.FillBuffers();
			}
			ReleaseControl();
		}

		protected void OnSelectColor(object sender, EventArgs e)
		{
			ColorDialog d = new ColorDialog();
			if(d.ShowDialog() == DialogResult.OK)
			{
				((ColoredShape)mShape).Color = d.Color;
				mColor.BackColor = d.Color;
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
		}
	}
}
