using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;

namespace Hourglass {
	class ParticleEmitterComponent : Component {
		protected Label mLbMaxLiving, mLbPPL, mLbPPS, mLbPosMin, mLbPX, mLbPY, mLbPZ, mLbBoundsMin, mLbBX, mLbBY, mLbBZ, mLbPosMax, mLbPXU, mLbPYU, mLbPZU;
		protected Label mLbBoundsMax, mLbBXU, mLbBYU, mLbBZU, mLbTexture;
		protected NumericUpDown mMaxLiving, mPPL, mPPS, mPXL, mPYL, mPZL, mBXL, mBYL, mBZL, mPXU, mPYU, mPZU, mBXU, mBYU, mBZU;
		protected Panel mPosMinPanel, mPosMaxPanel, mBoundsMinPanel, mBoundsMaxPanel, mVelocityPanel, mRadialPanel;
		protected ComboBox mTexture;

		// Particle settings
		protected Label mLbLife, mLbStartSize, mLbEndSize, mLbStartOpacity, mLbEndOpacity, mLbXR, mLbYR, mLbZR;
		protected Label mLbVelocity, mLbVX, mLbVY, mLbVZ, mLbStartCol, mLbEndCol, mLbRadial;
		protected NumericUpDown mLife, mStartSize, mEndSize, mSOpac, mEOpac, mXR, mYR, mZR, mVX, mVY, mVZ;
		protected Button mStartCol, mEndCol;


		public ParticleEmitterComponent(int _yoffset = 0) : base() {
			mType = ComponentType.Particle;

			#region Component Creation

			mLbMaxLiving = new Label(); // How many particles can exist at a time?
			mLbPPL = new Label(); // Particles-Per-Life. How many particles will this emitter actually emit?
			mLbPPS = new Label(); // Particles-Per-Second. How many particles are emitted per second?
			mLbPosMin = new Label();
			mLbBoundsMin = new Label();
			mLbPosMax = new Label();
			mLbBoundsMax = new Label();
			mLbPX = new Label();
			mLbPY = new Label();
			mLbPZ = new Label();
			mLbBX = new Label();
			mLbBY = new Label();
			mLbBZ = new Label();
			mLbPXU = new Label();
			mLbPYU = new Label();
			mLbPZU = new Label();
			mLbBXU = new Label();
			mLbBYU = new Label();
			mLbBZU = new Label();

			mLbLife = new Label();
			mLbStartSize = new Label();
			mLbEndSize = new Label();
			mLbStartOpacity = new Label();
			mLbEndOpacity = new Label();
			mLbXR = new Label();
			mLbYR = new Label();
			mLbZR = new Label();
			mLbVelocity = new Label();
			mLbVX = new Label();
			mLbVY = new Label();
			mLbVZ = new Label();
			mLbStartCol = new Label();
			mLbEndCol = new Label();
			mLbRadial = new Label();

			mLbTexture = new Label();
			mTexture = new ComboBox();

			mMaxLiving = new NumericUpDown(); // How many particles can exist at a time? -> _maxp
			mPPL = new NumericUpDown(); // Particles-Per-Life. How many particles will this emitter actually emit? -> _totalp
			mPPS = new NumericUpDown(); // Particles-Per-Second. How many particles are emitted per second? -> _persec
			mPXL = new NumericUpDown();
			mPYL = new NumericUpDown();
			mPZL = new NumericUpDown();
			mBXL = new NumericUpDown();
			mBYL = new NumericUpDown();
			mBZL = new NumericUpDown();
			mPXU = new NumericUpDown();
			mPYU = new NumericUpDown();
			mPZU = new NumericUpDown();
			mBXU = new NumericUpDown();
			mBYU = new NumericUpDown();
			mBZU = new NumericUpDown();

			mLife = new NumericUpDown();
			mStartSize = new NumericUpDown();
			mEndSize = new NumericUpDown();
			mSOpac = new NumericUpDown();
			mEOpac = new NumericUpDown();
			mXR = new NumericUpDown();
			mYR = new NumericUpDown();
			mZR = new NumericUpDown();
			mVX = new NumericUpDown();
			mVY = new NumericUpDown();
			mVZ = new NumericUpDown();

			mStartCol = new Button();
			mEndCol = new Button();

			mPosMinPanel = new Panel();
			mPosMaxPanel = new Panel();
			mBoundsMinPanel = new Panel();
			mBoundsMaxPanel = new Panel();
			mVelocityPanel = new Panel();
			mRadialPanel = new Panel();

			mGroupBox.Controls.Add(mLbMaxLiving);
			mGroupBox.Controls.Add(mLbPPL);
			mGroupBox.Controls.Add(mLbPPS);
			mGroupBox.Controls.Add(mLbPosMin);
			mGroupBox.Controls.Add(mLbBoundsMin);
			mGroupBox.Controls.Add(mLbBoundsMax);
			mGroupBox.Controls.Add(mLbPosMax);
			mGroupBox.Controls.Add(mLbLife);
			mGroupBox.Controls.Add(mLbStartSize);
			mGroupBox.Controls.Add(mLbEndSize);
			mGroupBox.Controls.Add(mLbStartOpacity);
			mGroupBox.Controls.Add(mLbEndOpacity);
			mGroupBox.Controls.Add(mLbXR);
			mGroupBox.Controls.Add(mLbYR);
			mGroupBox.Controls.Add(mLbZR);
			mGroupBox.Controls.Add(mLbVelocity);
			mGroupBox.Controls.Add(mLbVX);
			mGroupBox.Controls.Add(mLbVY);
			mGroupBox.Controls.Add(mLbVZ);
			mGroupBox.Controls.Add(mLbStartCol);
			mGroupBox.Controls.Add(mLbEndCol);
			mGroupBox.Controls.Add(mLbRadial);
			mGroupBox.Controls.Add(mLbTexture);
			mGroupBox.Controls.Add(mTexture);

			mGroupBox.Controls.Add(mMaxLiving);
			mGroupBox.Controls.Add(mPPL);
			mGroupBox.Controls.Add(mPPS);
			mGroupBox.Controls.Add(mLife);
			mGroupBox.Controls.Add(mStartSize);
			mGroupBox.Controls.Add(mEndSize);
			mGroupBox.Controls.Add(mSOpac);
			mGroupBox.Controls.Add(mEOpac);
			mGroupBox.Controls.Add(mStartCol);
			mGroupBox.Controls.Add(mEndCol);
			mGroupBox.Controls.Add(mPosMinPanel);
			mGroupBox.Controls.Add(mBoundsMinPanel);
			mGroupBox.Controls.Add(mPosMaxPanel);
			mGroupBox.Controls.Add(mBoundsMaxPanel);
			mGroupBox.Controls.Add(mVelocityPanel);
			mGroupBox.Controls.Add(mRadialPanel);
			#endregion

			#region Component Setup

			int ContentWidth = (mGroupBox.Size - mGroupBox.Padding.Size - mGroupBox.Margin.Size).Width;


			mLbMaxLiving.Text = "Max Living";
			mLbMaxLiving.Location = new System.Drawing.Point(6, 20);
			mLbMaxLiving.AutoSize = true;

			mLbPPL.Text = "Total Particles";
			mLbPPL.Location = new System.Drawing.Point(6, 45);
			mLbPPL.AutoSize = true;


			mLbPPS.Text = "Particles/s";
			mLbPPS.Location = new System.Drawing.Point(6, 70);
			mLbPPS.AutoSize = true;

			mLbPosMin.Text = "Lower Position";
			mLbPosMin.Location = new System.Drawing.Point(6, 95);
			mLbPosMin.AutoSize = true;

			mLbPosMax.Text = "Upper Position";
			mLbPosMax.Location = new System.Drawing.Point(6, 120);
			mLbPosMax.AutoSize = true;

			mLbBoundsMin.Text = "Lower Bounds";
			mLbBoundsMin.Location = new System.Drawing.Point(6, 145);
			mLbBoundsMin.AutoSize = true;

			mLbBoundsMax.Text = "Upper Bounds";
			mLbBoundsMax.Location = new System.Drawing.Point(6, 170);
			mLbBoundsMax.AutoSize = true;

			SetupTransformPanel(mPosMinPanel, 90, 92, ContentWidth, mLbPX, mLbPY, mLbPZ, mPXL, mPYL, mPZL);
			SetupTransformPanel(mPosMaxPanel, 90, 117, ContentWidth, mLbPXU, mLbPYU, mLbPZU, mPXU, mPYU, mPZU);
			SetupTransformPanel(mBoundsMinPanel, 90, 142, ContentWidth, mLbBX, mLbBY, mLbBZ, mBXL, mBYL, mBZL);
			SetupTransformPanel(mBoundsMaxPanel, 90, 167, ContentWidth, mLbBXU, mLbBYU, mLbBZU, mBXU, mBYU, mBZU);

			mMaxLiving.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			mMaxLiving.Location = new System.Drawing.Point(90, 17);
			mMaxLiving.Size = new System.Drawing.Size(ContentWidth - mMaxLiving.Left, 24);
			mMaxLiving.DecimalPlaces = 0;
			mMaxLiving.Minimum = 1;
			mMaxLiving.Maximum = int.MaxValue;

			mPPL.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			mPPL.Location = new System.Drawing.Point(90, 42);
			mPPL.Size = new System.Drawing.Size(ContentWidth - mPPL.Left, 24);
			mPPL.DecimalPlaces = 0;
			mPPL.Minimum = -1;
			mPPL.Maximum = int.MaxValue;

			mPPS.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			mPPS.Location = new System.Drawing.Point(90, 67);
			mPPS.Size = new System.Drawing.Size(ContentWidth - mMaxLiving.Left, 24);
			mPPS.DecimalPlaces = 0;
			mPPS.Minimum = 1;
			mPPS.Maximum = int.MaxValue;

			mLbLife.Text = "Particle Life";
			mLbLife.Location = new System.Drawing.Point(6, 220);
			mLbLife.AutoSize = true;

			mLife.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			mLife.Location = new System.Drawing.Point(90, 217);
			mLife.Size = new System.Drawing.Size(ContentWidth - mLife.Left, 24);
			mLife.DecimalPlaces = 0;
			mLife.Minimum = 1;
			mLife.Maximum = int.MaxValue;

			mLbStartSize.Text = "Start Size";
			mLbStartSize.Location = new System.Drawing.Point(6, 245);
			mLbStartSize.AutoSize = true;

			mStartSize.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			mStartSize.Location = new System.Drawing.Point(90, 242);
			mStartSize.Size = new System.Drawing.Size(ContentWidth - mStartSize.Left, 24);
			mStartSize.DecimalPlaces = 2;
			mStartSize.Minimum = 0;
			mStartSize.Maximum = int.MaxValue;
			mStartSize.Increment = (decimal)0.05f;

			mLbEndSize.Text = "End Size";
			mLbEndSize.Location = new System.Drawing.Point(6, 270);
			mLbEndSize.AutoSize = true;

			mEndSize.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			mEndSize.Location = new System.Drawing.Point(90, 267);
			mEndSize.Size = new System.Drawing.Size(ContentWidth - mEndSize.Left, 24);
			mEndSize.DecimalPlaces = 2;
			mEndSize.Minimum = 0;
			mEndSize.Maximum = int.MaxValue;
			mEndSize.Increment = (decimal)0.05f;

			mLbStartOpacity.Text = "Start Alpha";
			mLbStartOpacity.Location = new System.Drawing.Point(6, 295);
			mLbStartOpacity.AutoSize = true;

			mSOpac.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			mSOpac.Location = new System.Drawing.Point(90, 292);
			mSOpac.Size = new System.Drawing.Size(ContentWidth - mSOpac.Left, 24);
			mSOpac.DecimalPlaces = 2;
			mSOpac.Minimum = 0;
			mSOpac.Maximum = 1;
			mSOpac.Increment = (decimal)0.05f;

			mLbEndOpacity.Text = "End Alpha";
			mLbEndOpacity.Location = new System.Drawing.Point(6, 320);
			mLbEndOpacity.AutoSize = true;

			mEOpac.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			mEOpac.Location = new System.Drawing.Point(90, 317);
			mEOpac.Size = new System.Drawing.Size(ContentWidth - mEOpac.Left, 24);
			mEOpac.DecimalPlaces = 2;
			mEOpac.Minimum = 0;
			mEOpac.Maximum = 1;
			mEOpac.Increment = (decimal)0.05f;

			mLbRadial.Text = "Radial Bounds?";
			mLbRadial.Location = new System.Drawing.Point(6, 345);
			mLbRadial.AutoSize = true;
			SetupTransformPanel(mRadialPanel, 90, 342, ContentWidth, mLbXR, mLbYR, mLbZR, mXR, mYR, mZR);

			mLbVelocity.Text = "Velocity";
			mLbVelocity.Location = new System.Drawing.Point(6, 370);
			mLbVelocity.AutoSize = true;
			SetupTransformPanel(mVelocityPanel, 90, 367, ContentWidth, mLbVX, mLbVY, mLbVZ, mVX, mVY, mVZ);

			mLbStartCol.Text = "Start Color";
			mLbStartCol.Location = new System.Drawing.Point(6, 395);
			mLbStartCol.AutoSize = true;

			mStartCol.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			mStartCol.BackColor = System.Drawing.Color.Black;
			mStartCol.Location = new System.Drawing.Point(90, 392);
			mStartCol.Size = new System.Drawing.Size(ContentWidth - mStartCol.Left, 24);

			mLbEndCol.Text = "End Color";
			mLbEndCol.Location = new System.Drawing.Point(6, 420);
			mLbEndCol.AutoSize = true;

			mEndCol.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			mEndCol.BackColor = System.Drawing.Color.Black;
			mEndCol.Location = new System.Drawing.Point(90, 417);
			mEndCol.Size = new System.Drawing.Size(ContentWidth - mStartCol.Left, 24);

			mLbEndCol.Text = "Texture";
			mLbEndCol.Location = new System.Drawing.Point(6, 445);
			mLbEndCol.AutoSize = true;

			mTexture.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
			mTexture.Location = new System.Drawing.Point(90, 442);
			mTexture.Size = new System.Drawing.Size(ContentWidth - mTexture.Left, 24);
			mTexture.DropDownStyle = ComboBoxStyle.DropDownList;
			mTexture.Items.Add("");
			{
				List<string>.Enumerator it = ResourceManager.Instance.Textures.GetEnumerator();
				while (it.MoveNext()) {
					mTexture.Items.Add(it.Current);
				}
			}

			#endregion

			mGroupBox.Text = "Particle Emitter";
			mGroupBox.Size = mGroupBox.PreferredSize;

			OnMenuClick_Reset(null, null);

			mStartCol.Click += DoColorSelect;
			mEndCol.Click += DoColorSelect;
		}

		private void DoColorSelect(object sender, EventArgs e) {
			ColorDialog d = new ColorDialog() {
				Color = ((Button)sender).BackColor
			};
			if(d.ShowDialog() == DialogResult.OK) {
				((Button)sender).BackColor = d.Color;
			}
		}

		public override void OnMenuClick_Reset(object sender, EventArgs e) {
			ReleaseControl();

			mMaxLiving.Value = 100;
			mPPS.Value = 100;
			mPPL.Value = 100;
			mPXL.Value = 0;
			mPYL.Value = 0;
			mPZL.Value = 0;
			mBXL.Value = 0;
			mBYL.Value = 2;
			mBZL.Value = 0;

			mPXU.Value = 0;
			mPYU.Value = 0;
			mPZU.Value = 0;
			mBXU.Value = 0;
			mBYU.Value = 2;
			mBZU.Value = 0;

			mLife.Value = 100;
			mStartSize.Value = (decimal)0.5f;
			mEndSize.Value = (decimal)0.5f;
			mSOpac.Value = 1;
			mEOpac.Value = 1;
			mXR.Value = 0;
			mYR.Value = 0;
			mZR.Value = 0;

			mVX.Value = 0;
			mVY.Value = 0;
			mVZ.Value = 0;

			mTexture.SelectedIndex = -1;

			mStartCol.BackColor = System.Drawing.Color.Black;
			mEndCol.BackColor = System.Drawing.Color.Black;
		}

		

		public override void CopyData(ref Component _other) {
			if (!(_other is ParticleEmitterComponent)) {
				throw new InvalidDataException("Given component does not match the calling type.");
			}
			ParticleEmitterComponent comp = _other as ParticleEmitterComponent;
			comp.mMaxLiving.Value = mMaxLiving.Value;
			comp.mPPS.Value = mPPS.Value;
			comp.mPPL.Value = mPPL.Value;
			comp.mPXL.Value = mPXL.Value;
			comp.mPYL.Value = mPYL.Value;
			comp.mPZL.Value = mPZL.Value;
			comp.mBXL.Value = mBXL.Value;
			comp.mBYL.Value = mBYL.Value;
			comp.mBZL.Value = mBZL.Value;
			comp.mPXU.Value = mPXU.Value;
			comp.mPYU.Value = mPYU.Value;
			comp.mPZU.Value = mPZU.Value;
			comp.mBXU.Value = mBXU.Value;
			comp.mBYU.Value = mBYU.Value;
			comp.mBZU.Value = mBZU.Value;
			comp.mLife.Value = mLife.Value;
			comp.mStartSize.Value = mStartSize.Value;
			comp.mEndSize.Value = mEndSize.Value;
			comp.mSOpac.Value = mSOpac.Value;
			comp.mEOpac.Value = mEOpac.Value;
			comp.mXR.Value = mXR.Value;
			comp.mYR.Value = mYR.Value;
			comp.mZR.Value = mZR.Value;
			comp.mVX.Value = mVX.Value;
			comp.mVY.Value = mVY.Value;
			comp.mVZ.Value = mVZ.Value;
			comp.mStartCol.BackColor = mStartCol.BackColor;
			comp.mEndCol.BackColor = mEndCol.BackColor;
			comp.mTexture.SelectedIndex = mTexture.SelectedIndex;
		}

		public override void ReadData(BinaryReader r, int _version) {
			mMaxLiving.Value = (decimal)BitConverter.ToSingle(r.ReadBytes(4), 0);
			mPPS.Value = (decimal)BitConverter.ToSingle(r.ReadBytes(4), 0);
			mPPL.Value = (decimal)BitConverter.ToSingle(r.ReadBytes(4), 0);
			mPXL.Value = (decimal)BitConverter.ToSingle(r.ReadBytes(4), 0);
			mPYL.Value = (decimal)BitConverter.ToSingle(r.ReadBytes(4), 0);
			mPZL.Value = (decimal)BitConverter.ToSingle(r.ReadBytes(4), 0);
			mBXL.Value = (decimal)BitConverter.ToSingle(r.ReadBytes(4), 0);
			mBYL.Value = (decimal)BitConverter.ToSingle(r.ReadBytes(4), 0);
			mBZL.Value = (decimal)BitConverter.ToSingle(r.ReadBytes(4), 0);
			mPXU.Value = (decimal)BitConverter.ToSingle(r.ReadBytes(4), 0);
			mPYU.Value = (decimal)BitConverter.ToSingle(r.ReadBytes(4), 0);
			mPZU.Value = (decimal)BitConverter.ToSingle(r.ReadBytes(4), 0);
			mBXU.Value = (decimal)BitConverter.ToSingle(r.ReadBytes(4), 0);
			mBYU.Value = (decimal)BitConverter.ToSingle(r.ReadBytes(4), 0);
			mBZU.Value = (decimal)BitConverter.ToSingle(r.ReadBytes(4), 0);

			mLife.Value = (decimal)BitConverter.ToSingle(r.ReadBytes(4), 0);
			mStartSize.Value = (decimal)BitConverter.ToSingle(r.ReadBytes(4), 0);
			mEndSize.Value = (decimal)BitConverter.ToSingle(r.ReadBytes(4), 0);
			mSOpac.Value = (decimal)BitConverter.ToSingle(r.ReadBytes(4), 0);
			mEOpac.Value = (decimal)BitConverter.ToSingle(r.ReadBytes(4), 0);
			mXR.Value = (decimal)BitConverter.ToSingle(r.ReadBytes(4), 0);
			mYR.Value = (decimal)BitConverter.ToSingle(r.ReadBytes(4), 0);
			mZR.Value = (decimal)BitConverter.ToSingle(r.ReadBytes(4), 0);
			mVX.Value = (decimal)BitConverter.ToSingle(r.ReadBytes(4), 0);
			mVY.Value = (decimal)BitConverter.ToSingle(r.ReadBytes(4), 0);
			mVZ.Value = (decimal)BitConverter.ToSingle(r.ReadBytes(4), 0);
			mStartCol.BackColor = System.Drawing.Color.FromArgb(r.ReadInt32());
			mEndCol.BackColor = System.Drawing.Color.FromArgb(r.ReadInt32());

			string filename = new string(r.ReadChars(r.ReadInt32() - 1));
			r.ReadByte(); // The null terminator breaks things in C#, but is necessary in C++, so we need to skip it in C#
			filename = filename.Substring(filename.LastIndexOf("\\") + 1);
			int index = mTexture.Items.IndexOf(filename);
			if (index >= 0) {
				mTexture.SelectedIndex = index;
			} else {
				mTexture.Text = "Error locating diffuse texture.";
			}
		}

		public override void WriteData(BinaryWriter w) {
			base.WriteData(w);
			w.Write((float)mMaxLiving.Value);
			w.Write((float)mPPS.Value);
			w.Write((float)mPPL.Value);
			w.Write((float)mPXL.Value);
			w.Write((float)mPYL.Value);
			w.Write((float)mPZL.Value);
			w.Write((float)mBXL.Value);
			w.Write((float)mBYL.Value);
			w.Write((float)mBZL.Value);
			w.Write((float)mPXU.Value);
			w.Write((float)mPYU.Value);
			w.Write((float)mPZU.Value);
			w.Write((float)mPXU.Value);
			w.Write((float)mPYU.Value);
			w.Write((float)mPZU.Value);
			w.Write((float)mLife.Value);
			w.Write((float)mStartSize.Value);
			w.Write((float)mEndSize.Value);
			w.Write((float)mSOpac.Value);
			w.Write((float)mEOpac.Value);
			w.Write((float)mXR.Value);
			w.Write((float)mYR.Value);
			w.Write((float)mZR.Value);
			w.Write((float)mVX.Value);
			w.Write((float)mVY.Value);
			w.Write((float)mVZ.Value);
			w.Write(mStartCol.BackColor.ToArgb());
			w.Write(mEndCol.BackColor.ToArgb());

			string s = (".." + ResourceManager.Instance.ResourceDirectory + mTexture.Text);
			w.Write(s.Length + 1);
			w.Write(s.ToCharArray());
			byte term = 0;
			w.Write(term);
		}
	}
}
