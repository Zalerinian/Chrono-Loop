using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;
using Microsoft.DirectX;

namespace Hourglass
{
	class PlaneCollider : ColliderComponent
	{
		protected Label mLbOffset, mLbNormal, mLbNX, mLbNY, mLbNZ;
		protected NumericUpDown mOffset, mNX, mNY, mNZ;
		protected Panel mNormalPanel;

		public Vector3 Normal {
			get {
				Vector3 n = new Vector3();
				n.X = (float)mNX.Value;
				n.Y = (float)mNY.Value;
				n.Z = (float)mNZ.Value;
				return n;
			}
			set {
				mNX.Value = (decimal)value.X;
				mNY.Value = (decimal)value.Y;
				mNZ.Value = (decimal)value.Z;
			}
		}

		public PlaneCollider(int _yOffset = 0) : base(64 + _yOffset)
		{
			mType = ComponentType.PlaneCollider;

			#region Component Creation

			mLbOffset = new Label();
			mLbNormal = new Label();
			mLbNX = new Label();
			mLbNY = new Label();
			mLbNZ = new Label();

			mOffset = new NumericUpDown();
			mNX = new NumericUpDown();
			mNY = new NumericUpDown();
			mNZ = new NumericUpDown();

			mNormalPanel = new Panel();

			mGroupBox.Controls.Add(mNormalPanel);
			mGroupBox.Controls.Add(mLbOffset);
			mGroupBox.Controls.Add(mLbNormal);

			mGroupBox.Controls.Add(mOffset);

			#endregion

			#region Component Setup


			int ContentWidth = (mGroupBox.Size - mGroupBox.Padding.Size - mGroupBox.Margin.Size).Width;

			// Numeric Up-Downs
			mOffset.Anchor = (AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right);
			mOffset.Location = new System.Drawing.Point(90, 17 + _yOffset);
			mOffset.Name = "offset";
			mOffset.Size = new System.Drawing.Size(ContentWidth - mOffset.Left, 20);
			mOffset.TabIndex = 45;
            mOffset.Minimum = -100;
            mOffset.DecimalPlaces = 3;

			// These numeric up-downs are children of the panel, which is docked to the top, left, and right.
			// These are not docked to the right, because their resizing is a special case.
			mNY.Name = "Y";
			mNY.TabIndex = 14;

			mNZ.Name = "Z";
			mNZ.TabIndex = 14;


			mNX.Name = "X";
			mNX.TabIndex = 14;


			// Labels
			mLbNZ.Name = "mLbNZ";
			mLbNZ.TabIndex = 33;

			mLbNY.Name = "mLbNY";
			mLbNY.TabIndex = 32;

			mLbNX.Name = "mLbNX";
			mLbNX.TabIndex = 31;

			mLbNormal.AutoSize = true;
			mLbNormal.Location = new System.Drawing.Point(6, 49 + _yOffset);
			mLbNormal.Name = "mLbNormal";
			mLbNormal.Size = new System.Drawing.Size(40, 13);
			mLbNormal.TabIndex = 11;
			mLbNormal.Text = "Normal";

			mLbOffset.AutoSize = true;
			mLbOffset.Location = new System.Drawing.Point(6, 19 + _yOffset);
			mLbOffset.Name = "mLbOffset";
			mLbOffset.Size = new System.Drawing.Size(35, 13);
			mLbOffset.TabIndex = 10;
			mLbOffset.Text = "Offset";

			SetupTransformPanel(mNormalPanel, 90, 50 + _yOffset, ContentWidth, mLbNX, mLbNY, mLbNZ, mNX, mNY, mNZ);


			#endregion

			mNX.ValueChanged += OnMatrixUpdate;
			mNY.ValueChanged += OnMatrixUpdate;
			mNZ.ValueChanged += OnMatrixUpdate;
			mOffset.ValueChanged += OnMatrixUpdate;

			mGroupBox.Text = "Plane Collider";
			mGroupBox.Size = mGroupBox.PreferredSize;
			OnMenuClick_Reset(null, null);
		}

		public override void OnMenuClick_Reset(object sender, EventArgs e)
		{
			base.OnMenuClick_Reset(sender, e);
			mOffset.Value = 0;
			mNX.Value = 0;
			mNY.Value = 1;
			mNZ.Value = 0;
			mShape = new ColoredShape("Assets\\Colliders\\Plane.obj", System.Drawing.Color.Red);
			mShape.FillMode = Microsoft.DirectX.Direct3D.FillMode.WireFrame;
		}

		protected void OnMatrixUpdate(object sender, EventArgs e) {
			// Normalize the normal.
			Vector3 n = new Vector3((float)mNX.Value, (float)mNY.Value, (float)mNZ.Value);
			n.Normalize();
			//mNX.Value = (decimal)n.X;
			//mNY.Value = (decimal)n.Y;
			//mNZ.Value = (decimal)n.Z;


			// Calculate the rotation
			Vector3 r = new Vector3();
			r.X = (float)Math.PI * n.Y;
			r.Y = (float)Math.PI * n.Z; // The plane object naturally points up on the Y axis, so we need to cut the value in half and subtract an extra half PI to account for it.
			r.Z = (float)Math.PI * n.X;

			mShape.Scale = new Vector3(1, 1, 1);
			//mShape.Rotation = r;
			mShape.Position = new Vector3(n.X * (float)mOffset.Value, n.Y * (float)mOffset.Value, n.Z * (float)mOffset.Value);
		}

		public override void WriteData(BinaryWriter w)
		{
			base.WriteData(w);
			w.Write((float)mOffset.Value);
			w.Write((float)mNX.Value);
			w.Write((float)mNY.Value);
			w.Write((float)mNZ.Value);
		}

		public override void ReadData(BinaryReader r, int _version)
		{
			base.ReadData(r, _version);
			mOffset.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
			mNX.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
			mNY.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
			mNZ.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
		}


	}
}