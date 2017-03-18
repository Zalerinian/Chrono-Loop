using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace LevelEditor {
    class PlaneCollider : ColliderComponent {
        protected Label mLbOffset, mLbNormal, mLbNX, mLbNY, mLbNZ;
        protected NumericUpDown mOffset, mNX, mNY, mNZ;
        protected Panel mNormalPanel;

        public PlaneCollider(List<Component> _container, int _yOffset = 0) : base(_container, 64 + _yOffset) {
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
            mNormalPanel.Controls.Add(mLbNX);
            mNormalPanel.Controls.Add(mLbNY);
            mNormalPanel.Controls.Add(mLbNZ);
            mNormalPanel.Controls.Add(mNX);
            mNormalPanel.Controls.Add(mNY);
            mNormalPanel.Controls.Add(mNZ);

            mGroupBox.Controls.Add(mNormalPanel);
            mGroupBox.Controls.Add(mLbOffset);
            mGroupBox.Controls.Add(mLbNormal);
            mGroupBox.AutoSize = false;
            //mGroupBox.Controls.Add(mOffset);

            #endregion

            #region Component Setup

            mNormalPanel.Anchor = (AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right);
            mNormalPanel.Location = new System.Drawing.Point(90, 43 + _yOffset);
            mNormalPanel.Name = "mNormalPanel";
            mNormalPanel.Size = new System.Drawing.Size(194, 34);
            mNormalPanel.TabIndex = 34;
            mNormalPanel.AutoSize = false;


            // Numeric Up-Downs
            mOffset.Anchor = (AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right);
            mOffset.Location = new System.Drawing.Point(90, 17 + _yOffset);
            mOffset.Name = "udPlaneOffset";
            mOffset.Size = new System.Drawing.Size(194, 20);
            mOffset.TabIndex = 45;
            mOffset.Resize += OnBootleg;


            // These numeric up-downs are children of the panel, which is docked to the top, left, and right.
            // These are not docked to the right, because their resizing is a special case.
            mNY.DecimalPlaces = 2;
            mNY.Increment = new decimal(0.1);
            mNY.Location = new System.Drawing.Point(81, 6 + _yOffset);
            mNY.Maximum = new decimal(5);
            mNY.Minimum = new decimal(-5);
            mNY.Name = "mNY";
            mNY.Size = new System.Drawing.Size(41, 20);
            mNY.TabIndex = 14;

            mNZ.DecimalPlaces = 2;
            mNZ.Increment = new decimal(0.1);
            mNZ.Location = new System.Drawing.Point(143, 6 + _yOffset);
            mNZ.Maximum = new decimal(5);
            mNZ.Minimum = new decimal(-5);
            mNZ.Name = "mNZ";
            mNZ.Size = new System.Drawing.Size(41, 20);
            mNZ.TabIndex = 14;

            mNX.DecimalPlaces = 2;
            mNX.Increment = new decimal(0.1);
            mNX.Location = new System.Drawing.Point(19, 6 + _yOffset);
            mNX.Maximum = new decimal(5);
            mNX.Minimum = new decimal(-5);
            mNX.Name = "mNX";
            mNX.Size = new System.Drawing.Size(41, 20);
            mNX.TabIndex = 14;


            // Labels
            mLbNZ.AutoSize = true;
            mLbNZ.Location = new System.Drawing.Point(127, 9 + _yOffset);
            mLbNZ.Name = "mLbNZ";
            mLbNZ.Size = new System.Drawing.Size(14, 13);
            mLbNZ.TabIndex = 33;
            mLbNZ.Text = "Z";

            mLbNY.AutoSize = true;
            mLbNY.Location = new System.Drawing.Point(65, 9 + _yOffset);
            mLbNY.Name = "mLbNY";
            mLbNY.Size = new System.Drawing.Size(14, 13);
            mLbNY.TabIndex = 32;
            mLbNY.Text = "Y";

            mLbNX.AutoSize = true;
            mLbNX.Location = new System.Drawing.Point(3, 9 + _yOffset);
            mLbNX.Name = "mLbNX";
            mLbNX.Size = new System.Drawing.Size(14, 13);
            mLbNX.TabIndex = 31;
            mLbNX.Text = "X";

            mLbNormal.AutoSize = true;
            mLbNormal.Location = new System.Drawing.Point(12, 49 + _yOffset);
            mLbNormal.Name = "mLbNormal";
            mLbNormal.Size = new System.Drawing.Size(40, 13);
            mLbNormal.TabIndex = 11;
            mLbNormal.Text = "Normal";

            mLbOffset.AutoSize = true;
            mLbOffset.Location = new System.Drawing.Point(12, 19 + _yOffset);
            mLbOffset.Name = "mLbOffset";
            mLbOffset.Size = new System.Drawing.Size(35, 13);
            mLbOffset.TabIndex = 10;
            mLbOffset.Text = "Offset";


            // Event Handlers
            mNormalPanel.Resize += OnNormalPanelResize;

            #endregion

            mGroupBox.Text = "Plane Collider";
            mGroupBox.Size = mGroupBox.PreferredSize;
            OnMenuClick_Reset(null, null);
        }

        private void OnBootleg(object sender, EventArgs e) {
            string stopright = "there";
        }

        protected override void OnMenuClick_Reset(object sender, EventArgs e) {
            base.OnMenuClick_Reset(sender, e);
            mOffset.Value = 0;
            mNX.Value = 0;
            mNY.Value = 1;
            mNZ.Value = 0;
        }

        private void OnNormalPanelResize(object sender, EventArgs e) {
            int ContentWidth = (mNormalPanel.Size - mNormalPanel.Margin.Size - mNormalPanel.Padding.Size).Width;
            int nudSize = (ContentWidth - (mLbNX.Size.Width * 3)) / 3;
            System.Drawing.Size size = new System.Drawing.Size();

            // X Label
            mLbNX.Left = 3;
            mNX.Left = mLbNX.Left + mLbNX.Size.Width;

            // X Numeric Up-Down
            size.Height = mNX.Height;
            size.Width = mNX.Left + nudSize;
            mNX.Size = size;

            // Y Label
            mLbNY.Left = 3 + mNX.Right;
            mNY.Left = mLbNY.Left + mLbNY.Size.Width;

            // Y Numeric Up-Down
            mNY.Size = size;

            // Z Label
            mLbNZ.Left = 3 + mNY.Right;
            mNZ.Left = mLbNZ.Left + mLbNZ.Size.Width;

            // Z Numeric Up-Down
            mNZ.Size = size;


        }
    }
}