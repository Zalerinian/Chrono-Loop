using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;
using Microsoft.DirectX;

namespace Hourglass
{
    class ButtonCollider : Component, IRenderable {
        protected Label mLbPosition, mLbScale, mLbNormal, mLbMass, mLbForce;
        protected Label mLbPosX, mLbPosY, mLbPosZ;
        protected Label mLbScaleX, mLbScaleY, mLbScaleZ;
        protected Label mLbNX, mLbNY, mLbNZ;
        protected NumericUpDown mMass, mForce;
        protected NumericUpDown mPosX, mPosY, mPosZ;
        protected NumericUpDown mScaleX, mScaleY, mScaleZ;
        protected NumericUpDown mNX, mNY, mNZ;
        protected Panel mPosPanel, mScalePanel, mNormalPanel;

		protected ColoredShape mShape;

		public int RenderStage {
			get {
				return 0;
			}
		}

		public Vector3 PushNormal {
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
				Shape.Position = value;
			}
		}

		public float NormalForce{
			get {
				return (float)mForce.Value;
			}
			set {
				mForce.Value = (decimal)value;
			}
		}

		public float Mass {
			get {
				return (float)mMass.Value;
			}
			set {
				mMass.Value = (decimal)value;
			}
		}

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
				mShape.Position = value;
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

		public RenderShape Shape {
			get {
				return mShape;
			}
		}

        public ButtonCollider(int _yOffset = 0) : base()
        {
			mType = ComponentType.ButtonCollider;

			mShape = new ColoredShape();

			#region Component Creation

			mLbPosition = new Label();
            mLbScale = new Label();
            mLbNormal = new Label();
            mLbMass = new Label();
            mLbForce = new Label();

            mLbPosX = new Label();
            mLbPosY = new Label();
            mLbPosZ = new Label();

            mLbScaleX = new Label();
            mLbScaleY = new Label();
            mLbScaleZ = new Label();

            mLbNX = new Label();
            mLbNY = new Label();
            mLbNZ = new Label();

            mMass = new NumericUpDown();
            mForce = new NumericUpDown();

            mPosX = new NumericUpDown();
            mPosY = new NumericUpDown();
            mPosZ = new NumericUpDown();

            mScaleX = new NumericUpDown();
            mScaleY = new NumericUpDown();
            mScaleZ = new NumericUpDown();

            mNX = new NumericUpDown();
            mNY = new NumericUpDown();
            mNZ = new NumericUpDown();
            
            mPosPanel = new Panel();
            mScalePanel = new Panel();
            mNormalPanel = new Panel();

            mGroupBox.Controls.Add(mPosPanel);
            mGroupBox.Controls.Add(mScalePanel);
            mGroupBox.Controls.Add(mNormalPanel);
            mGroupBox.Controls.Add(mLbPosition);
            mGroupBox.Controls.Add(mLbScale);
            mGroupBox.Controls.Add(mLbNormal);
            mGroupBox.Controls.Add(mLbMass);
            mGroupBox.Controls.Add(mLbForce);
            mGroupBox.Controls.Add(mMass);
            mGroupBox.Controls.Add(mForce);

            #endregion

            #region Component Setup

            int ContentWidth = (mGroupBox.Size - mGroupBox.Padding.Size - mGroupBox.Margin.Size).Width;

            // Numeric Up-Downs
            mMass.Anchor = (AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right);
            mMass.Location = new System.Drawing.Point(90, 115 + _yOffset);
            mMass.Name = "offset";
            mMass.Size = new System.Drawing.Size(ContentWidth - mMass.Left, 25);
            mMass.TabIndex = 45;
            mMass.Increment = new decimal(0.1);
            mMass.Minimum = new decimal(0);
            mMass.Maximum = new decimal(999999.9);
            mMass.DecimalPlaces = 2;

            mForce.Anchor = (AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right);
            mForce.Location = new System.Drawing.Point(90, 140 + _yOffset);
            mForce.Name = "mForce";
            mForce.Size = new System.Drawing.Size(ContentWidth - mForce.Left, 25);
            mForce.Increment = new decimal(0.1);
            mForce.Minimum = new decimal(0);
            mForce.Maximum = new decimal(999999.9);
            mForce.DecimalPlaces = 2;


            // These numeric up-downs are children of the panel, which is docked to the top, left, and right.
            // These are not docked to the right, because their resizing is a special case.


            // Labels
            mLbPosX.Name = "mLbPosX";
            mLbPosY.Name = "mLbPosY";
            mLbPosZ.Name = "mLbPosZ";
            mLbScaleX.Name = "mLbScaleX";
            mLbScaleY.Name = "mLbScaleY";
            mLbScaleZ.Name = "mLbScaleZ";
            mLbNZ.Name = "mLbNZ";
            mLbNY.Name = "mLbNY";
            mLbNX.Name = "mLbNX";

            mLbPosition.AutoSize = true;
            mLbPosition.Location = new System.Drawing.Point(6, 25 + _yOffset);
            mLbPosition.Name = "mLbPosition";
            mLbPosition.Size = new System.Drawing.Size(35, 13);
            mLbPosition.TabIndex = 10;
            mLbPosition.Text = "Position";

            mLbScale.AutoSize = true;
            mLbScale.Location = new System.Drawing.Point(6, 55 + _yOffset);
            mLbScale.Name = "mLbScale";
            mLbScale.Size = new System.Drawing.Size(40, 13);
            mLbScale.TabIndex = 11;
            mLbScale.Text = "Scale";

            mLbNormal.AutoSize = true;
            mLbNormal.Location = new System.Drawing.Point(6, 85 + _yOffset);
            mLbNormal.Name = "mLbNormal";
            mLbNormal.Size = new System.Drawing.Size(40, 13);
            mLbNormal.TabIndex = 11;
            mLbNormal.Text = "Push Normal";

            mLbMass.AutoSize = true;
            mLbMass.Location = new System.Drawing.Point(6, 119 + _yOffset);
            mLbMass.Name = "mLbMass";
            mLbMass.Size = new System.Drawing.Size(40, 13);
            mLbMass.TabIndex = 0;
            mLbMass.Text = "Mass";

            mLbForce.AutoSize = true;
            mLbForce.Location = new System.Drawing.Point(6, 144 + _yOffset);
            mLbForce.Name = "mLbForce";
            mLbForce.Size = new System.Drawing.Size(40, 13);
            mLbForce.TabIndex = 0;
            mLbForce.Text = "Normal Force";

            SetupTransformPanel(mPosPanel, 90, 20, ContentWidth, mLbPosX, mLbPosY, mLbPosZ, mPosX, mPosY, mPosZ);
            SetupTransformPanel(mScalePanel, 90, 50, ContentWidth, mLbScaleX, mLbScaleY, mLbScaleZ, mScaleX, mScaleY, mScaleZ);
            SetupTransformPanel(mNormalPanel, 90, 80 + _yOffset, ContentWidth, mLbNX, mLbNY, mLbNZ, mNX, mNY, mNZ);

			#endregion

			mPosX.ValueChanged += OnMatrixUpdated;
			mPosY.ValueChanged += OnMatrixUpdated;
			mPosZ.ValueChanged += OnMatrixUpdated;
			mScaleX.ValueChanged += OnMatrixUpdated;
			mScaleY.ValueChanged += OnMatrixUpdated;
			mScaleZ.ValueChanged += OnMatrixUpdated;

			mGroupBox.Text = "Button Collider";
            mGroupBox.Size = mGroupBox.PreferredSize;
            OnMenuClick_Reset(null, null);
        }

        public override void OnMenuClick_Reset(object sender, EventArgs e)
        {
            mPosX.Value = 0;
            mPosY.Value = 0;
            mPosZ.Value = 0;

            mScaleX.Value = 0;
            mScaleY.Value = 0;
            mScaleZ.Value = 0;

            mNX.Value = 0;
            mNY.Value = 1;
            mNZ.Value = 0;

            mMass.Value = 0;
            mForce.Value = 0;
			mShape = new ColoredShape("Assets\\Colliders\\Cube.obj", System.Drawing.Color.Red);
			mShape.FillMode = Microsoft.DirectX.Direct3D.FillMode.WireFrame;
		}

		protected void OnMatrixUpdated(object sender, EventArgs e) {
			if (mShape != null) {
				mShape.World = CreateMatrix();
			}
		}

		protected Vector3 GetScaleVector() {
			return new Vector3((float)mScaleX.Value, (float)mScaleY.Value, (float)mScaleZ.Value);
		}

		protected Vector3 GetPositionVector() {
			return new Vector3((float)mPosX.Value, (float)mPosY.Value, (float)mPosZ.Value);
		}

		protected Matrix CreateMatrix() {
			// This is assuming you want it to rotate in place. To rotate around a point, you'd do STR.
			Matrix mat = Matrix.Identity;
			mat *= Matrix.Scaling(GetScaleVector());
			mat *= Matrix.Translation(GetPositionVector());
			return mat;
		}

		public override void WriteData(BinaryWriter w)
		{
			base.WriteData(w);
			w.Write((float)mMass.Value);
			w.Write((float)mForce.Value);
			w.Write((float)mPosX.Value);
			w.Write((float)mPosY.Value);
			w.Write((float)mPosZ.Value);
			w.Write((float)mScaleX.Value);
			w.Write((float)mScaleY.Value);
			w.Write((float)mScaleZ.Value);
			w.Write((float)mNX.Value);
			w.Write((float)mNY.Value);
			w.Write((float)mNZ.Value);
		}

		public override void ReadData(BinaryReader r, int _version)
		{
			mMass.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
			mForce.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));

			mPosX.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
			mPosY.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
			mPosZ.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));

			mScaleX.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
			mScaleY.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
			mScaleZ.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));

			mNX.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
			mNY.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
			mNZ.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
		}


		public override void CopyData(ref Component _other) {
			if (!(_other is ButtonCollider)) {
				throw new InvalidDataException("Given component does not match calling type!");
			}
			ButtonCollider comp = _other as ButtonCollider;
			comp.PushNormal = PushNormal;
			comp.Mass = Mass;
			comp.Position = Position;
			comp.Scale = Scale;
			comp.NormalForce = NormalForce;
		}
	}
}