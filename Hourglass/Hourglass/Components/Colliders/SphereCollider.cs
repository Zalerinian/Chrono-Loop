using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Hourglass
{
    class SphereCollider : ColliderComponent
    {
        protected Label mLbPosition, mLbRadius, mLbX, mLbY, mLbZ;
        protected NumericUpDown mRadius, mX, mY, mZ;
        protected Panel mPosPanel;

        public SphereCollider(List<Component> _container, int _yOffset = 0) : base(_container, 64 + _yOffset)
        {
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


            #endregion

            mGroupBox.Text = "Sphere Collider";
            mGroupBox.Size = mGroupBox.PreferredSize;
            OnMenuClick_Reset(null, null);
        }

        protected override void OnMenuClick_Reset(object sender, EventArgs e)
        {
            base.OnMenuClick_Reset(sender, e);
            mRadius.Value = 1;
            mX.Value = 0;
            mY.Value = 0;
            mZ.Value = 0;
        }

    }
}