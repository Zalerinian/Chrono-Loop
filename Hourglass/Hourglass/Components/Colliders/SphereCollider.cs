using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;
using Microsoft.DirectX;

namespace Hourglass
{
    class SphereCollider : ColliderComponent
    {
        protected Label mLbPosition, mLbRadius, mLbX, mLbY, mLbZ;
        protected NumericUpDown mRadius, mX, mY, mZ;
        protected Panel mPosPanel;

        public SphereCollider(int _yOffset = 0) : base(64 + _yOffset)
        {
			mType = ComponentType.SphereCollider;

			#region Component Creation

			mLbRadius = new Label();
            mLbPosition = new Label();
            mLbX = new Label();
            mLbY = new Label();
            mLbZ = new Label();

            mRadius = new NumericUpDown();
            mX = new NumericUpDown();
            mY = new NumericUpDown();
            mZ = new NumericUpDown();

            mPosPanel = new Panel();

            mGroupBox.Controls.Add(mPosPanel);
            mGroupBox.Controls.Add(mLbRadius);
            mGroupBox.Controls.Add(mLbPosition);

            mGroupBox.Controls.Add(mRadius);

            #endregion

            #region Component Setup

            int ContentWidth = (mGroupBox.Size - mGroupBox.Padding.Size - mGroupBox.Margin.Size).Width;

            // Numeric Up-Downs
            mRadius.Anchor = (AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right);
            mRadius.Location = new System.Drawing.Point(90, 17 + _yOffset);
            mRadius.Name = "radius";
            mRadius.Size = new System.Drawing.Size(ContentWidth - mRadius.Left, 20 + _yOffset);
            mRadius.TabIndex = 45;
			mRadius.Minimum = 0;
			mRadius.Maximum = 100;
			mRadius.DecimalPlaces = 2;
			mRadius.Increment = (decimal)0.1f;


            // These numeric up-downs are children of the panel, which is docked to the top, left, and right.
            // These are not docked to the right, because their resizing is a special case.
            mY.Name = "Y";
            mY.TabIndex = 14;

            mZ.Name = "Z";
            mZ.TabIndex = 14;


            mX.Name = "X";
            mX.TabIndex = 14;


            // Labels
            mLbZ.Name = "mLbNZ";
            mLbZ.TabIndex = 33;

            mLbY.Name = "mLbNY";
            mLbY.TabIndex = 32;

            mLbX.Name = "mLbNX";
            mLbX.TabIndex = 31;

            mLbPosition.AutoSize = true;
            mLbPosition.Location = new System.Drawing.Point(6, 49 + _yOffset);
            mLbPosition.Name = "mLbNormal";
            mLbPosition.Size = new System.Drawing.Size(40, 13);
            mLbPosition.TabIndex = 11;
            mLbPosition.Text = "Position";

            mLbRadius.AutoSize = true;
            mLbRadius.Location = new System.Drawing.Point(6, 19 + _yOffset);
            mLbRadius.Name = "mLbOffset";
            mLbRadius.Size = new System.Drawing.Size(35, 13);
            mLbRadius.TabIndex = 10;
            mLbRadius.Text = "Radius";

            SetupTransformPanel(mPosPanel, 90, 50 + _yOffset, ContentWidth, mLbX, mLbY, mLbZ, mX, mY, mZ);

			mX.ValueChanged += OnMatrixUpdated;
			mY.ValueChanged += OnMatrixUpdated;
			mZ.ValueChanged += OnMatrixUpdated;
			mRadius.ValueChanged += OnMatrixUpdated;


			#endregion

			mGroupBox.Text = "Sphere Collider";
            mGroupBox.Size = mGroupBox.PreferredSize;
            OnMenuClick_Reset(null, null);
        }

        public float Radius {
            get {
                return (float)mRadius.Value;
            }
            set {
                mRadius.Value = (decimal)value;
                mShape.Scale = new Microsoft.DirectX.Vector3(value, value, value);
            }
        }

		public Vector3 Position {
			get {
				Vector3 v = new Vector3();
				v.X = (float)mX.Value;
				v.Y = (float)mY.Value;
				v.Z = (float)mZ.Value;
				return v;
			}
			set {
				mX.Value = (decimal)value.X;
				mY.Value = (decimal)value.Y;
				mZ.Value = (decimal)value.Z;
				Shape.Position = value;
			}
		}

		public override void OnMenuClick_Reset(object sender, EventArgs e)
        {
            base.OnMenuClick_Reset(sender, e);
            mRadius.Value = 1;
            mX.Value = 0;
            mY.Value = 0;
            mZ.Value = 0;
			mShape = new ColoredShape("Assets\\Colliders\\Sphere.obj", System.Drawing.Color.Red);
			mShape.FillMode = Microsoft.DirectX.Direct3D.FillMode.WireFrame;
		}

		protected void OnMatrixUpdated(object sender, EventArgs e) {
			Position = new Vector3((float)mX.Value, (float)mY.Value, (float)mZ.Value);
			Radius = (float)mRadius.Value;
		}

		public override void WriteData(BinaryWriter w)
		{
			base.WriteData(w);
			w.Write((float)mRadius.Value);
			w.Write((float)mX.Value);
			w.Write((float)mY.Value);
			w.Write((float)mZ.Value);
		}

		public override void ReadData(BinaryReader r, int _version)
		{
			base.ReadData(r, _version);
			mRadius.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
			mX.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
			mY.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
			mZ.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
		}

	}
}