using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;

namespace Hourglass {

	public class ColliderComponent : Component, IRenderable {

		protected NumericUpDown mMass, mStaticFriction, mElasticity, mKineticFriction, mDrag;
		protected Label mLbMass, mLbStaticFriction, mLbElasticity, mLbKineticFriction, mLbDrag, mLbColor;
		protected ColoredShape mShape = null;
		protected Button mColor;
		protected CheckBox mMovable, mTrigger, mPickUpAble;

		public RenderShape Shape {
			get {
				return mShape;
			}
		}

		public int RenderStage {
			get {
				return 0;
			}
		}

		public bool Movable {
			get {
				return mMovable.Checked;
			}
			set {
				mMovable.Checked = value;
			}
		}

		public bool Trigger {
			get {
				return mTrigger.Checked;
			}
			set {
				mTrigger.Checked = value;
			}
		}

        public bool PickUpAble
        {
            get
            {
                return mPickUpAble.Checked;
            }
            set
            {
                mPickUpAble.Checked = value;
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

		public float StaticFriction {
			get {
				return (float)mStaticFriction.Value;
			}
			set {
				mStaticFriction.Value = (decimal)value;
			}
		}

		public float KineticFriction {
			get {
				return (float)mKineticFriction.Value;
			}
			set {
				mKineticFriction.Value = (decimal)value;
			}
		}

		public float Elasticity {
			get {
				return (float)mElasticity.Value;
			}
			set {
				mElasticity.Value = (decimal)value;
			}
		}

		public float Drag {
			get {
				return (float)mDrag.Value;
			}
			set {
				mDrag.Value = (decimal)value;
			}
		}


		public ColliderComponent(int _yOffset = 0) : base() {
			#region Control Creation

			// Numeric Up-Down controls:
			mMass = new NumericUpDown();
			mStaticFriction = new NumericUpDown();
			mElasticity = new NumericUpDown();
			mKineticFriction = new NumericUpDown();
			mDrag = new NumericUpDown();

			// Labels
			mLbMass = new Label();
			mLbStaticFriction = new Label();
			mLbElasticity = new Label();
			mLbKineticFriction = new Label();
			mLbDrag = new Label();
			mLbColor = new Label();

			// Buttons
			mColor = new Button();

			// Checkboxes
			mMovable = new CheckBox();
			mTrigger = new CheckBox();
            mPickUpAble = new CheckBox();

			mGroupBox.Controls.Add(mMass);
			mGroupBox.Controls.Add(mStaticFriction);
			mGroupBox.Controls.Add(mElasticity);
			mGroupBox.Controls.Add(mKineticFriction);
			mGroupBox.Controls.Add(mDrag);
			mGroupBox.Controls.Add(mColor);
			mGroupBox.Controls.Add(mMovable);
			mGroupBox.Controls.Add(mTrigger);
            mGroupBox.Controls.Add(mPickUpAble);

			mGroupBox.Controls.Add(mLbMass);
			mGroupBox.Controls.Add(mLbStaticFriction);
			mGroupBox.Controls.Add(mLbElasticity);
			mGroupBox.Controls.Add(mLbKineticFriction);
			mGroupBox.Controls.Add(mLbDrag);
			mGroupBox.Controls.Add(mLbColor);

			mShape = new ColoredShape();

			#endregion

			#region Control Setup

			int ContentWidth = (mGroupBox.Size - mGroupBox.Padding.Size - mGroupBox.Margin.Size).Width;

			// Labels
			mLbMass.AutoSize = true;
			mLbMass.Location = new System.Drawing.Point(6, 22 + _yOffset);
			mLbMass.Name = "mLbMass";
			mLbMass.Size = new System.Drawing.Size(38, 13);
			mLbMass.TabIndex = 0;
			mLbMass.Text = "Mass";

			mLbElasticity.AutoSize = true;
			mLbElasticity.Location = new System.Drawing.Point(6, 52 + _yOffset);
			mLbElasticity.Name = "mLbElasticity";
			mLbElasticity.Size = new System.Drawing.Size(54, 13);
			mLbElasticity.TabIndex = 1;
			mLbElasticity.Text = "Elasticity";

			mLbDrag.AutoSize = true;
			mLbDrag.Location = new System.Drawing.Point(6, 79 + _yOffset);
			mLbDrag.Name = "mLbDrag";
			mLbDrag.Size = new System.Drawing.Size(36, 13);
			mLbDrag.TabIndex = 2;
			mLbDrag.Text = "Drag";

			mLbStaticFriction.AutoSize = true;
			mLbStaticFriction.Location = new System.Drawing.Point(6, 105 + _yOffset);
			mLbStaticFriction.Name = "mLbStaticFriction";
			mLbStaticFriction.Size = new System.Drawing.Size(77, 13);
			mLbStaticFriction.TabIndex = 3;
			mLbStaticFriction.Text = "Static Friction";

			mLbKineticFriction.AutoSize = true;
			mLbKineticFriction.Location = new System.Drawing.Point(6, 132 + _yOffset);
			mLbKineticFriction.Name = "mLbKineticFriction";
			mLbKineticFriction.Size = new System.Drawing.Size(82, 13);
			mLbKineticFriction.TabIndex = 4;
			mLbKineticFriction.Text = "Kinetic Friction";

			mLbColor.AutoSize = true;
			mLbColor.Location = new System.Drawing.Point(6, 155 + _yOffset);
			mLbColor.Size = new System.Drawing.Size(100, 20);
			mLbColor.Name = "mLbColor";
			mLbColor.Text = "Collider Color";

			// Numeric Up-Downs
			mElasticity.Anchor = (AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right);
			mElasticity.Location = new System.Drawing.Point(90, 49 + _yOffset);
			mElasticity.Name = "mElasticity";
			mElasticity.Size = new System.Drawing.Size(ContentWidth - mElasticity.Left, 20);
			mElasticity.TabIndex = 6;
			mElasticity.Minimum = 0;
			mElasticity.Maximum = 999999;
            mElasticity.DecimalPlaces = 2;

            mDrag.Anchor = (AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right);
			mDrag.Location = new System.Drawing.Point(90, 77 + _yOffset);
			mDrag.Name = "mDrag";
			mDrag.Size = new System.Drawing.Size(ContentWidth - mDrag.Left, 20);
			mDrag.TabIndex = 7;
			mDrag.Minimum = 0;
			mDrag.Maximum = 999999;
            mDrag.DecimalPlaces = 2;

			mMass.Anchor = (AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right);
			mMass.Location = new System.Drawing.Point(90, 20 + _yOffset);
			mMass.Name = "mMass";
			mMass.Size = new System.Drawing.Size(ContentWidth - mMass.Left, 20);
			mMass.TabIndex = 5;
			mMass.Minimum = 0;
			mMass.Maximum = 999999;
            mMass.DecimalPlaces = 2;

            mStaticFriction.Anchor = (AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right);
			mStaticFriction.Location = new System.Drawing.Point(90, 103 + _yOffset);
			mStaticFriction.Name = "mStaticFriction";
			mStaticFriction.Size = new System.Drawing.Size(ContentWidth - mStaticFriction.Left, 20);
			mStaticFriction.TabIndex = 8;
			mStaticFriction.Minimum = 0;
			mStaticFriction.Maximum = 999999;
            mStaticFriction.DecimalPlaces = 2;

            mKineticFriction.Anchor = (AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right);
			mKineticFriction.Location = new System.Drawing.Point(90, 130 + _yOffset);
			mKineticFriction.Name = "mKineticFriction";
			mKineticFriction.Size = new System.Drawing.Size(ContentWidth - mKineticFriction.Left, 20);
			mKineticFriction.TabIndex = 9;
			mKineticFriction.Minimum = 0;
			mKineticFriction.Maximum = 999999;
            mKineticFriction.DecimalPlaces = 2;

            // Buttons
            mColor.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			mColor.Location = new System.Drawing.Point(90, 155 + _yOffset);
			mColor.Size = new System.Drawing.Size(ContentWidth - mColor.Left, 20);
			mColor.Text = "Collider Color";

			// Recalculate content width.
			mGroupBox.Size = mGroupBox.PreferredSize;
			ContentWidth = (mGroupBox.Size - mGroupBox.Padding.Size - mGroupBox.Margin.Size).Width;


			// Checkboxes
			mMovable.Text = "Movable";
			mMovable.AutoSize = true;
			mMovable.Checked = true;
			mMovable.Location = new System.Drawing.Point(ContentWidth / 4, 180 + _yOffset);
			///mMovable.Size = new System.Drawing.Size(ContentWidth / 2, 25);
			mMovable.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;

			mTrigger.Text = "Is Trigger";
			mTrigger.AutoSize = true;
			mTrigger.Checked = false;
			mTrigger.Location = new System.Drawing.Point(ContentWidth / 4 * 2, 180 + _yOffset);
			//mTrigger.Size = new System.Drawing.Size(ContentWidth / 2, 25);
			mTrigger.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;

            mPickUpAble.Text = "PickUpAble";
            mPickUpAble.AutoSize = true;
            mPickUpAble.Checked = false;
            mPickUpAble.Location = new System.Drawing.Point(ContentWidth / 4, 180 + _yOffset);
            ///mMovable.Size = new System.Drawing.Size(ContentWidth / 2, 25);
            mPickUpAble.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;


            #endregion

            mGroupBox.Text = "Collider Component";
			mGroupBox.Size = mGroupBox.PreferredSize;
			mGroupBox.Resize += OnGroupboxResize;
		}

		public void OnGroupboxResize(object sender, EventArgs e) {
			int ContentWidth = (mGroupBox.Size - mGroupBox.Padding.Size - mGroupBox.Margin.Size).Width;
			mMovable.Location = new System.Drawing.Point(ContentWidth / 4 - (mMovable.Size.Width / 2), mMovable.Location.Y);
			mTrigger.Location = new System.Drawing.Point(ContentWidth / 4 * 2 - (mTrigger.Size.Width / 2), mTrigger.Location.Y);
            mPickUpAble.Location = new System.Drawing.Point(ContentWidth / 4 * 3 - (mTrigger.Size.Width / 2), mTrigger.Location.Y);
        }

		public override void OnMenuClick_Reset(object sender, EventArgs e) {
			mMass.Value = 0;
			mElasticity.Value = 0;
			mDrag.Value = 0;
			mStaticFriction.Value = 0;
			mKineticFriction.Value = 0;
			mMovable.Checked = true;
			mTrigger.Checked = false;
            mPickUpAble.Checked = false;
			if (mShape != null) {
				mShape.Dispose();
			}
		}

		public override void WriteData(System.IO.BinaryWriter w) {
			base.WriteData(w);
			w.Write((float)mMass.Value);
			w.Write((float)mStaticFriction.Value);
			w.Write((float)mKineticFriction.Value);
			w.Write((float)mElasticity.Value);
			w.Write((float)mDrag.Value);
			w.Write(mMovable.Checked);  // Added in writer Version 2
			w.Write(mTrigger.Checked);  // Added in writer Version 2
            w.Write(mPickUpAble.Checked); // Added in writer Version 4
        }

		public override void ReadData(BinaryReader r, int _version) {
			mMass.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
			mStaticFriction.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
			mKineticFriction.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
			mElasticity.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
			mDrag.Value = (decimal)(System.BitConverter.ToSingle(r.ReadBytes(4), 0));
			if (_version >= 2) {
				mMovable.Checked = r.ReadByte() == 1;
				mTrigger.Checked = r.ReadByte() == 1;
			}
            if(_version >= 4)
            {
                mPickUpAble.Checked = r.ReadByte() == 1;
            }
		}

		public override void CopyData(ref Component _other) {
			if (!(_other is ColliderComponent)) {
				throw new InvalidDataException("Given component does not match calling type!");
			}
			ColliderComponent comp = _other as ColliderComponent;
			comp.mColor.BackColor = mColor.BackColor;
			comp.mDrag.Value = mDrag.Value;
			comp.mElasticity.Value = mElasticity.Value;
			comp.mKineticFriction.Value = mKineticFriction.Value;
			comp.mMass.Value = mMass.Value;
			comp.mMovable.Checked = mMovable.Checked;
			comp.mPickUpAble.Checked = mPickUpAble.Checked;
			comp.mStaticFriction.Value = mStaticFriction.Value;
			comp.mTrigger.Checked = mTrigger.Checked;

			mShape.Color = mColor.BackColor;
		}

	}

}

