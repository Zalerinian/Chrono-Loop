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

            mGroupBox.Text = "Plane Collider";
            mGroupBox.Size = mGroupBox.PreferredSize;
            OnMenuClick_Reset(null, null);
        }

        protected override void OnMenuClick_Reset(object sender, EventArgs e) {
            base.OnMenuClick_Reset(sender, e);
            mOffset.Value = 0;
            mNX.Value = 0;
            mNY.Value = 1;
            mNZ.Value = 0;
        }

    }
}