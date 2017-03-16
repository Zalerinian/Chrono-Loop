using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace LevelEditor {

    class BoxCollider : ColliderComponent {
        protected Label mLbPosition, mLbPosX, mLbPosY, mLbPosZ;
        protected Label mLbRotation, mLbRotX, mLbRotY, mLbRotZ;
        protected Label mLbScale, mLbScaleX, mLbScaleY, mLbScaleZ;

        protected NumericUpDown mPosX, mPosY, mPosZ;
        protected NumericUpDown mRotX, mRotY, mRotZ;
        protected NumericUpDown mScaleX, mScaleY, mScaleZ;


        public BoxCollider(List<Component> _container, int _yOffset = 0) : base(_container, 100 + _yOffset) {

            #region Component Creation

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

            mPosX = new NumericUpDown();
            mPosY = new NumericUpDown();
            mPosZ = new NumericUpDown();
            mRotX = new NumericUpDown();
            mRotY = new NumericUpDown();
            mRotZ = new NumericUpDown();
            mScaleX = new NumericUpDown();
            mScaleY = new NumericUpDown();
            mScaleZ = new NumericUpDown();

            mGroupBox.Controls.Add(mLbPosition);
            mGroupBox.Controls.Add(mLbPosX);
            mGroupBox.Controls.Add(mLbPosY);
            mGroupBox.Controls.Add(mLbPosZ);
            mGroupBox.Controls.Add(mLbRotation);
            mGroupBox.Controls.Add(mLbRotX);
            mGroupBox.Controls.Add(mLbRotY);
            mGroupBox.Controls.Add(mLbRotZ);
            mGroupBox.Controls.Add(mLbScale);
            mGroupBox.Controls.Add(mLbScaleX);
            mGroupBox.Controls.Add(mLbScaleY);
            mGroupBox.Controls.Add(mLbScaleZ);

            mGroupBox.Controls.Add(mPosX);
            mGroupBox.Controls.Add(mPosY);
            mGroupBox.Controls.Add(mPosZ);
            mGroupBox.Controls.Add(mRotX);
            mGroupBox.Controls.Add(mRotY);
            mGroupBox.Controls.Add(mRotZ);
            mGroupBox.Controls.Add(mScaleX);
            mGroupBox.Controls.Add(mScaleY);
            mGroupBox.Controls.Add(mScaleZ);

            #endregion

            #region Component Setup

            // Labels
            mLbPosition.AutoSize = true;
            mLbPosition.Location = new System.Drawing.Point(7, 23 + _yOffset);
            mLbPosition.Name = "mLbPosition";
            mLbPosition.Size = new System.Drawing.Size(44, 13);
            mLbPosition.TabIndex = 0;
            mLbPosition.Text = "Position";


            mLbPosX.AutoSize = true;
            mLbPosX.Location = new System.Drawing.Point(88, 23 + _yOffset);
            mLbPosX.Name = "mLbPosX";
            mLbPosX.Size = new System.Drawing.Size(14, 13);
            mLbPosX.TabIndex = 0;
            mLbPosX.Text = "X";

            mLbPosY.AutoSize = true;
            mLbPosY.Location = new System.Drawing.Point(164, 23 + _yOffset);
            mLbPosY.Name = "mLbPosY";
            mLbPosY.Size = new System.Drawing.Size(14, 13);
            mLbPosY.TabIndex = 0;
            mLbPosY.Text = "Y";

            mLbPosZ.AutoSize = true;
            mLbPosZ.Location = new System.Drawing.Point(240, 23 + _yOffset);
            mLbPosZ.Name = "mLbPosZ";
            mLbPosZ.Size = new System.Drawing.Size(14, 13);
            mLbPosZ.TabIndex = 16;
            mLbPosZ.Text = "Z";

            mLbScale.AutoSize = true;
            mLbScale.Location = new System.Drawing.Point(7, 93 + _yOffset);
            mLbScale.Name = "mLbScale";
            mLbScale.Size = new System.Drawing.Size(34, 13);
            mLbScale.TabIndex = 17;
            mLbScale.Text = "Scale";

            mLbRotation.AutoSize = true;
            mLbRotation.Location = new System.Drawing.Point(7, 59 + _yOffset);
            mLbRotation.Name = "mLbRotation";
            mLbRotation.Size = new System.Drawing.Size(47, 13);
            mLbRotation.TabIndex = 18;
            mLbRotation.Text = "Rotation";

            mLbRotZ.AutoSize = true;
            mLbRotZ.Location = new System.Drawing.Point(240, 59 + _yOffset);
            mLbRotZ.Name = "mLbRotZ";
            mLbRotZ.Size = new System.Drawing.Size(14, 13);
            mLbRotZ.TabIndex = 24;
            mLbRotZ.Text = "Z";

            mLbRotY.AutoSize = true;
            mLbRotY.Location = new System.Drawing.Point(164, 59 + _yOffset);
            mLbRotY.Name = "mLbRotY";
            mLbRotY.Size = new System.Drawing.Size(14, 13);
            mLbRotY.TabIndex = 23;
            mLbRotY.Text = "Y";

            mLbRotX.AutoSize = true;
            mLbRotX.Location = new System.Drawing.Point(88, 59 + _yOffset);
            mLbRotX.Name = "mLbRotX";
            mLbRotX.Size = new System.Drawing.Size(14, 13);
            mLbRotX.TabIndex = 22;
            mLbRotX.Text = "X";

            mLbScaleZ.AutoSize = true;
            mLbScaleZ.Location = new System.Drawing.Point(240, 93 + _yOffset);
            mLbScaleZ.Name = "mLbScaleZ";
            mLbScaleZ.Size = new System.Drawing.Size(14, 13);
            mLbScaleZ.TabIndex = 30;
            mLbScaleZ.Text = "Z";

            mLbScaleY.AutoSize = true;
            mLbScaleY.Location = new System.Drawing.Point(164, 93 + _yOffset);
            mLbScaleY.Name = "mLbScaleY";
            mLbScaleY.Size = new System.Drawing.Size(14, 13);
            mLbScaleY.TabIndex = 29;
            mLbScaleY.Text = "Y";

            mLbScaleX.AutoSize = true;
            mLbScaleX.Location = new System.Drawing.Point(88, 93 + _yOffset);
            mLbScaleX.Name = "mLbScaleX";
            mLbScaleX.Size = new System.Drawing.Size(14, 13);
            mLbScaleX.TabIndex = 28;
            mLbScaleX.Text = "X";

            // Numeric Up-Downs
            mPosX.DecimalPlaces = 2;
            mPosX.Increment = new decimal(0.1);
            mPosX.Location = new System.Drawing.Point(104, 19 + _yOffset);
            mPosX.Maximum = new decimal(500.0);
            mPosX.Minimum = new decimal(-500.0);
            mPosX.Name = "mPosX";
            mPosX.Size = new System.Drawing.Size(52, 20);
            mPosX.TabIndex = 11;

            mPosY.DecimalPlaces = 2;
            mPosY.Increment = new decimal(0.1);
            mPosY.Location = new System.Drawing.Point(180, 19 + _yOffset);
            mPosY.Maximum = new decimal(500.0);
            mPosY.Minimum = new decimal(-500.0);
            mPosY.Name = "mPosY";
            mPosY.Size = new System.Drawing.Size(52, 20);
            mPosY.TabIndex = 12;

            mPosZ.DecimalPlaces = 2;
            mPosZ.Increment = new decimal(0.1);
            mPosZ.Location = new System.Drawing.Point(256, 19 + _yOffset);
            mPosZ.Maximum = new decimal(500.0);
            mPosZ.Minimum = new decimal(-500.0);
            mPosZ.Name = "mPosZ";
            mPosZ.Size = new System.Drawing.Size(52, 20);
            mPosZ.TabIndex = 13;

            mRotZ.DecimalPlaces = 2;
            mRotZ.Increment = new decimal(0.1);
            mRotZ.Location = new System.Drawing.Point(256, 55 + _yOffset);
            mRotZ.Maximum = new decimal(500.0);
            mRotZ.Minimum = new decimal(-500.0);
            mRotZ.Name = "mRotZ";
            mRotZ.Size = new System.Drawing.Size(52, 20);
            mRotZ.TabIndex = 21;


            mRotY.DecimalPlaces = 2;
            mRotY.Increment = new decimal(0.1);
            mRotY.Location = new System.Drawing.Point(180, 55 + _yOffset);
            mRotY.Maximum = new decimal(500.0);
            mRotY.Minimum = new decimal(-500.0);
            mRotY.Name = "mRotY";
            mRotY.Size = new System.Drawing.Size(52, 20);
            mRotY.TabIndex = 21;

            mRotX.DecimalPlaces = 2;
            mRotX.Increment = new decimal(0.1);
            mRotX.Location = new System.Drawing.Point(104, 55 + _yOffset);
            mRotX.Maximum = new decimal(500.0);
            mRotX.Minimum = new decimal(-500.0);
            mRotX.Name = "mRotX";
            mRotX.Size = new System.Drawing.Size(52, 20);
            mRotX.TabIndex = 21;

            mScaleZ.DecimalPlaces = 2;
            mScaleZ.Increment = new decimal(0.1);
            mScaleZ.Location = new System.Drawing.Point(256, 89 + _yOffset);
            mScaleZ.Maximum = new decimal(500.0);
            mScaleZ.Minimum = new decimal(-500.0);
            mScaleZ.Name = "mScaleZ";
            mScaleZ.Size = new System.Drawing.Size(52, 20);
            mScaleZ.TabIndex = 27;

            mScaleY.DecimalPlaces = 2;
            mScaleY.Increment = new decimal(0.1);
            mScaleY.Location = new System.Drawing.Point(180, 89 + _yOffset);
            mScaleY.Maximum = new decimal(500.0);
            mScaleY.Minimum = new decimal(-500.0);
            mScaleY.Name = "mScaleY";
            mScaleY.Size = new System.Drawing.Size(52, 20);
            mScaleY.TabIndex = 27;

            mScaleX.DecimalPlaces = 2;
            mScaleX.Increment = new decimal(0.1);
            mScaleX.Location = new System.Drawing.Point(104, 89 + _yOffset);
            mScaleX.Maximum = new decimal(500.0);
            mScaleX.Minimum = new decimal(-500.0);
            mScaleX.Name = "mScaleX";
            mScaleX.Size = new System.Drawing.Size(52, 20);
            mScaleX.TabIndex = 27;


            #endregion

            mGroupBox.Text = "Box Collider";
            mGroupBox.Size = mGroupBox.PreferredSize;
        }

        protected override void OnMenuClick_Reset(object sender, EventArgs e) {
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
        }
    }

}