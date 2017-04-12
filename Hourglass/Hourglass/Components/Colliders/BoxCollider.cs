using System;
using System.Collections.Generic;
using System.Windows.Forms;
using Microsoft.DirectX;

namespace Hourglass {

	class BoxCollider : ColliderComponent {
		protected Panel mPosPanel, mRotPanel, mScalePanel, mGravityPanel;

		protected Label mLbPosition, mLbPosX, mLbPosY, mLbPosZ;
		protected Label mLbRotation, mLbRotX, mLbRotY, mLbRotZ;
		protected Label mLbScale, mLbScaleX, mLbScaleY, mLbScaleZ;
		protected Label mLbGravity, mLbGravX, mLbGravY, mLbGravZ;

		protected NumericUpDown mPosX, mPosY, mPosZ;
		protected NumericUpDown mRotX, mRotY, mRotZ;
		protected NumericUpDown mScaleX, mScaleY, mScaleZ;
		protected NumericUpDown mGravX, mGravY, mGravZ;

		public Vector3 Position {
			get {
				Vector3 v = new Vector3();
				v.X = (float)mPosX.Value;
				v.Y = (float)mPosY.Value;
				v.Z = (float)mPosZ.Value;
				return v;
			}
			set {
				mPosX.Value = (decimal)value.X;
				mPosY.Value = (decimal)value.Y;
				mPosZ.Value = (decimal)value.Z;
				Shape.Position = value;
			}
		}

		public Vector3 Rotation {
			get {
				Vector3 v = new Vector3();
				v.X = (float)mRotX.Value;
				v.Y = (float)mRotY.Value;
				v.Z = (float)mRotZ.Value;
				return v;
			}
			set {
				mRotX.Value = (decimal)value.X;
				mRotY.Value = (decimal)value.Y;
				mRotZ.Value = (decimal)value.Z;
				mShape.Rotation = value;
			}
		}

		public Vector3 Scale {
			get {
				Vector3 v = new Vector3();
				v.X = (float)mScaleX.Value;
				v.Y = (float)mScaleY.Value;
				v.Z = (float)mScaleZ.Value;
				return v;
			}
			set {
				mScaleX.Value = (decimal)value.X;
				mScaleY.Value = (decimal)value.Y;
				mScaleZ.Value = (decimal)value.Z;
				mShape.Scale = value;
			}
		}

		public Vector3 Gravity {
			get {
				Vector3 v = new Vector3();
				v.X = (float)mGravX.Value;
				v.Y = (float)mGravY.Value;
				v.Z = (float)mGravZ.Value;
				return v;
			}
			set {
				mGravX.Value = (decimal)value.X;
				mGravY.Value = (decimal)value.Y;
				mGravZ.Value = (decimal)value.Z;
			}
		}

		public BoxCollider(int _yOffset = 0) : base(140 + _yOffset) {
			mType = ComponentType.BoxCollider;


			#region Component Creation


			// Panels
			mPosPanel = new Panel();
			mRotPanel = new Panel();
			mScalePanel = new Panel();
			mGravityPanel = new Panel();

			// Labels
			mLbPosition = new Label();
			mLbPosX = new Label();
			mLbPosY = new Label();
			mLbPosZ = new Label();

			mLbRotation = new Label();
			mLbRotX = new Label();
			mLbRotY = new Label();
			mLbRotZ = new Label();

			mLbScale = new Label();
			mLbScaleX = new Label();
			mLbScaleY = new Label();
			mLbScaleZ = new Label();

			mLbGravity = new Label();
			mLbGravX = new Label();
			mLbGravY = new Label();
			mLbGravZ = new Label();


			mPosX = new NumericUpDown();
			mPosY = new NumericUpDown();
			mPosZ = new NumericUpDown();
			mRotX = new NumericUpDown();
			mRotY = new NumericUpDown();
			mRotZ = new NumericUpDown();
			mScaleX = new NumericUpDown();
			mScaleY = new NumericUpDown();
			mScaleZ = new NumericUpDown();
			mGravX = new NumericUpDown();
			mGravY = new NumericUpDown();
			mGravZ = new NumericUpDown();

			mGroupBox.Controls.Add(mLbPosition);
			mGroupBox.Controls.Add(mLbRotation);
			mGroupBox.Controls.Add(mLbScale);
			mGroupBox.Controls.Add(mLbGravity);
			mGroupBox.Controls.Add(mPosPanel);
			mGroupBox.Controls.Add(mRotPanel);
			mGroupBox.Controls.Add(mScalePanel);
			mGroupBox.Controls.Add(mGravityPanel);

			#endregion

			#region Component Setup

			int ContentWidth = (mGroupBox.Size - mGroupBox.Padding.Size - mGroupBox.Margin.Size).Width;

			// Labels
			mLbPosition.AutoSize = true;
			mLbPosition.Location = new System.Drawing.Point(6, 23 + _yOffset);
			mLbPosition.Name = "mLbPosition";
			mLbPosition.Size = new System.Drawing.Size(44, 13);
			mLbPosition.TabIndex = 0;
			mLbPosition.Text = "Position";



			mLbScale.AutoSize = true;
			mLbScale.Location = new System.Drawing.Point(6, 93 + _yOffset);
			mLbScale.Name = "mLbScale";
			mLbScale.Size = new System.Drawing.Size(34, 13);
			mLbScale.TabIndex = 17;
			mLbScale.Text = "Scale";

			mLbGravity.AutoSize = true;
			mLbGravity.Location = new System.Drawing.Point(6, 128 + _yOffset);
			mLbGravity.Name = "mLbGravity";
			mLbGravity.Size = new System.Drawing.Size(34, 13);
			mLbGravity.TabIndex = 18;
			mLbGravity.Text = "Gravity";

			mLbRotation.AutoSize = true;
			mLbRotation.Location = new System.Drawing.Point(6, 59 + _yOffset);
			mLbRotation.Name = "mLbRotation";
			mLbRotation.Size = new System.Drawing.Size(47, 13);
			mLbRotation.TabIndex = 18;
			mLbRotation.Text = "Rotation";

			// these are setup via the SetupTransformPanel function.
			mLbPosX.Name = "mLbPosX";
			mLbPosY.Name = "mLbPosY";
			mLbPosZ.Name = "mLbPosZ";

			mLbRotX.Name = "mLbRotX";
			mLbRotY.Name = "mLbRotY";
			mLbRotZ.Name = "mLbRotZ";

			mLbScaleX.Name = "mLbScaleX";
			mLbScaleY.Name = "mLbScaleY";
			mLbScaleZ.Name = "mLbScaleZ";

			// Numeric Up-Downs
			// These are setup in the SetupTransformPanel function.
			mPosX.Name = "mPosX";
			mPosX.TabIndex = 11;

			mPosY.Name = "mPosY";
			mPosY.TabIndex = 12;

			mPosZ.Name = "mPosZ";
			mPosZ.TabIndex = 13;

			mRotZ.Name = "mRotZ";
			mRotZ.TabIndex = 21;

			mRotY.Name = "mRotY";
			mRotY.TabIndex = 21;

			mRotX.Name = "mRotX";
			mRotX.TabIndex = 21;

			mScaleZ.Name = "mScaleZ";
			mScaleZ.TabIndex = 27;

			mScaleY.Name = "mScaleY";
			mScaleY.TabIndex = 27;

			mScaleX.Name = "mScaleX";
			mScaleX.TabIndex = 27;


			// Panels
			SetupTransformPanel(mPosPanel, 90, 20 + _yOffset, ContentWidth, mLbPosX, mLbPosY, mLbPosZ, mPosX, mPosY, mPosZ);
			SetupTransformPanel(mRotPanel, 90, 55 + _yOffset, ContentWidth, mLbRotX, mLbRotY, mLbRotZ, mRotX, mRotY, mRotZ);
			SetupTransformPanel(mScalePanel, 90, 90 + _yOffset, ContentWidth, mLbScaleX, mLbScaleY, mLbScaleZ, mScaleX, mScaleY, mScaleZ);
			SetupTransformPanel(mGravityPanel, 90, 125 + _yOffset, ContentWidth, mLbGravX, mLbGravY, mLbGravZ, mGravX, mGravY, mGravZ);

			#endregion

			mGroupBox.Text = "Box Collider";
			mGroupBox.Size = mGroupBox.PreferredSize;
		}

		public override void OnMenuClick_Reset(object sender, EventArgs e) {
			base.OnMenuClick_Reset(sender, e);
			mPosX.Value = 0;
			mPosY.Value = 0;
			mPosZ.Value = 0;

			mRotX.Value = 0;
			mRotY.Value = 0;
			mRotZ.Value = 0;

			mScaleX.Value = 0;
			mScaleY.Value = 0;
			mScaleZ.Value = 0;

			mGravX.Value = 0;
			mGravY.Value = (decimal)-9.81;
			mGravZ.Value = 0;
		}

		private void OnTransformPanelResize(object sender, EventArgs e) {
			// This assumes that the first 3 children of the panel are the Labels, and the 
			// last three are the X, Y, and Z Numeric Up-Downs in order.
			Panel panel = sender as Panel;
			Label lx = panel.Controls[0] as Label,
				ly = panel.Controls[1] as Label,
				lz = panel.Controls[2] as Label;
			NumericUpDown nx = panel.Controls[3] as NumericUpDown,
				ny = panel.Controls[4] as NumericUpDown,
				nz = panel.Controls[5] as NumericUpDown;
			int ContentWidth = (panel.Size - panel.Margin.Size - panel.Padding.Size).Width;
			int nudSize = (ContentWidth - (lx.Size.Width * 3)) / 3;
			System.Drawing.Size size = new System.Drawing.Size();

			size.Height = nx.Height;
			size.Width = nudSize;

			// X Label
			lx.Left = 3;
			nx.Left = lx.Left + lx.Size.Width;

			// X Numeric Up-Down
			nx.Size = size;

			// Y Label
			ly.Left = 1 + nx.Right;
			ny.Left = ly.Left + ly.Size.Width;

			// Y Numeric Up-Down
			ny.Size = size;

			// Z Label
			lz.Left = ny.Right;
			nz.Left = lz.Left + lz.Size.Width;

			// Z Numeric Up-Down
			nz.Size = size;
		}

		public override void WriteData(System.IO.BinaryWriter w) {
			base.WriteData(w);
			w.Write((float)mPosX.Value);
			w.Write((float)mPosY.Value);
			w.Write((float)mPosZ.Value);

			w.Write((float)mRotX.Value * FileIO.DEGREES_TO_RADIANS);
			w.Write((float)mRotY.Value * FileIO.DEGREES_TO_RADIANS);
			w.Write((float)mRotZ.Value * FileIO.DEGREES_TO_RADIANS);

			w.Write((float)mScaleX.Value);
			w.Write((float)mScaleY.Value);
			w.Write((float)mScaleZ.Value);
		}

		public override void ReadData(System.IO.BinaryReader r, int _version) {
			base.ReadData(r, _version);
			mPosX.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
			mPosY.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
			mPosZ.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));

			mRotX.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
			mRotY.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
			mRotZ.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));

			mScaleX.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
			mScaleY.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
			mScaleZ.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
		}
	}

}