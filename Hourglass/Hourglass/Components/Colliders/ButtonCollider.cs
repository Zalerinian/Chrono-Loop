using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Hourglass
{
    class ButtonCollider : Component
    {
        protected Label mLbPosition, mLbScale, mLbNormal, mLbMass, mLbForce;
        protected Label mLbPosX, mLbPosY, mLbPosZ;
        protected Label mLbScaleX, mLbScaleY, mLbScaleZ;
        protected Label mLbNX, mLbNY, mLbNZ;
        protected NumericUpDown mMass, mForce;
        protected NumericUpDown mPosX, mPosY, mPosZ;
        protected NumericUpDown mScaleX, mScaleY, mScaleZ;
        protected NumericUpDown mNX, mNY, mNZ;
        protected Panel mPosPanel, mScalePanel, mNormalPanel;

        public ButtonCollider(int _yOffset = 0) : base()
        {
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
        }

    }
}