using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;

namespace Hourglass {
	public class LightComponent : MeshComponent {
		private Button mColor = new Button();

		public System.Drawing.Color Color {
			get {
				return mColor.BackColor;
			}
			set {
				mColor.BackColor = System.Drawing.Color.FromArgb((int)(((float)mTransparency.Value) * 255), value.R, value.G, value.B);
				((ColoredShape)mShape).Color = mColor.BackColor;
			}
		}

		public override int RenderStage {
			get {
				return 1;
			}
		}

		public LightComponent(int _yOffset = 0) : base(45 + _yOffset) {
			mType = ComponentType.Light;

			#region Component Creation
			mShape = new ColoredShape();

			mGroupBox.Controls.Add(mColor);
			Renderer.Instance.AddLightObject(this);
			#endregion

			#region Component Setup
			int ContentWidth = (mGroupBox.Size - mGroupBox.Padding.Size - mGroupBox.Margin.Size).Width;

			mColor.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			mColor.Location = new System.Drawing.Point(6, 21);
			mColor.Size = new System.Drawing.Size(ContentWidth - mColor.Left, 24);
			mColor.Font = new System.Drawing.Font("Microsoft Sans Serif", 10, System.Drawing.FontStyle.Bold);
			mColor.Text = "Mesh Color";
			mColor.Click += OnSelectColor;

			{
				List<string>.Enumerator it = ResourceManager.Instance.Objects.GetEnumerator();
				while (it.MoveNext()) {
					mMesh.Items.Add(it.Current);
				}
			}

			mMesh.SelectedIndexChanged += OnMeshSelectionChange;
			mTransparency.ValueChanged += MTransparency_ValueChanged;

			#endregion

			mGroupBox.Size = mGroupBox.PreferredSize;
			OnMenuClick_Reset(null, null);
		}

		private void MTransparency_ValueChanged(object sender, EventArgs e) {
			Color = Color; // Property handles using alpha value.
		}

		protected void OnMeshSelectionChange(object sender, EventArgs e) {
			if (!string.IsNullOrWhiteSpace(mMesh.Text)) {
				((ColoredShape)mShape).Load(Settings.ProjectPath + ResourceManager.Instance.ResourceDirectory + mMesh.Text, mColor.BackColor);
				mShape.FillBuffers();
			}
			ReleaseControl();
		}

		protected void OnSelectColor(object sender, EventArgs e) {
			ColorDialog d = new ColorDialog() {
				Color = Color
			};
			if (d.ShowDialog() == DialogResult.OK) {
				Color = d.Color;
			}
			ReleaseControl();
		}

		public override void OnMenuClick_Delete(object sender, EventArgs e) {
			base.OnMenuClick_Delete(sender, e);
			mShape.Dispose();
			Renderer.Instance.RemoveLightObject(this);
		}

		public override void OnMenuClick_Reset(object sender, EventArgs e) {
			base.OnMenuClick_Reset(sender, e);
			mTransparency.Value = 1;
			mPixel.Enabled = false;
			mVertex.Enabled = false;
			mGeo.Enabled = false;
			mPixel.SelectedIndex = -1;
			mVertex.SelectedIndex = -1;
			mGeo.SelectedIndex = -1;
			Color = Color;
			mShape.Dispose();
		}

		public override void WriteData(BinaryWriter w) {
			base.WriteData(w);
			int colorValue = mColor.BackColor.ToArgb();
			w.Write(colorValue);
		}

		public override void ReadData(BinaryReader r, int _version) {
			base.ReadData(r, _version);
			Color = System.Drawing.Color.FromArgb(r.ReadInt32());
		}

		public override void CopyData(ref Component _other) {
			if (!(_other is LightComponent)) {
				throw new InvalidDataException("Given component does not match the calling type.");
			}
			base.CopyData(ref _other);
			LightComponent comp = _other as LightComponent;
			comp.Color = Color;
            comp.mMesh.SelectedIndex = mMesh.SelectedIndex;
		}
	}
}
