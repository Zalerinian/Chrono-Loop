using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Hourglass {

    class BoxCollider : ColliderComponent {
        protected Panel mPosPanel, mRotPanel, mScalePanel;

        protected Label mLbPosition, mLbPosX, mLbPosY, mLbPosZ;
        protected Label mLbRotation, mLbRotX, mLbRotY, mLbRotZ;
        protected Label mLbScale, mLbScaleX, mLbScaleY, mLbScaleZ;

        protected NumericUpDown mPosX, mPosY, mPosZ;
        protected NumericUpDown mRotX, mRotY, mRotZ;
        protected NumericUpDown mScaleX, mScaleY, mScaleZ;


        public BoxCollider(BaseObject _owner, int _yOffset = 0) : base(_owner, 100 + _yOffset) {

            #region Component Creation

            mPosPanel = new Panel();
            mRotPanel = new Panel();
            mScalePanel = new Panel();

            mLbPosition = new Label();
            mLbPosX = new Label();
            mLbPosY = new Label();
            mLbPosZ = new Label();
            mPosPanel.Controls.Add(mLbPosX);
            mPosPanel.Controls.Add(mLbPosY);
            mPosPanel.Controls.Add(mLbPosZ);

            mLbRotation = new Label();
            mLbRotX = new Label();
            mLbRotY = new Label();
            mLbRotZ = new Label();
            mRotPanel.Controls.Add(mLbRotX);
            mRotPanel.Controls.Add(mLbRotY);
            mRotPanel.Controls.Add(mLbRotZ);

            mLbScale = new Label();
            mLbScaleX = new Label();
            mLbScaleY = new Label();
            mLbScaleZ = new Label();
            mScalePanel.Controls.Add(mLbScaleX);
            mScalePanel.Controls.Add(mLbScaleY);
            mScalePanel.Controls.Add(mLbScaleZ);

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
            mGroupBox.Controls.Add(mLbRotation);
            mGroupBox.Controls.Add(mLbScale);
            mGroupBox.Controls.Add(mPosPanel);
            mGroupBox.Controls.Add(mRotPanel);
            mGroupBox.Controls.Add(mScalePanel);

            #endregion

            #region Component Setup

            int ContentWidth = (mGroupBox.Size - mGroupBox.Padding.Size - mGroupBox.Margin.Size).Width;

            // Labels
            mLbPosition.AutoSize = true;
            mLbPosition.Location = new System.Drawing.Point(6, 23 + _yOffset);
            mLbPosition.Name = "mLbPosition";
            mLbPosition.Size = new System.Drawing.Size(44, 13);
            mLbPosition.TabIndex = 0;
            mLbPosition.Text = "Position";



            mLbScale.AutoSize = true;
            mLbScale.Location = new System.Drawing.Point(6, 93 + _yOffset);
            mLbScale.Name = "mLbScale";
            mLbScale.Size = new System.Drawing.Size(34, 13);
            mLbScale.TabIndex = 17;
            mLbScale.Text = "Scale";

            mLbRotation.AutoSize = true;
            mLbRotation.Location = new System.Drawing.Point(6, 59 + _yOffset);
            mLbRotation.Name = "mLbRotation";
            mLbRotation.Size = new System.Drawing.Size(47, 13);
            mLbRotation.TabIndex = 18;
            mLbRotation.Text = "Rotation";

            // these are setup via the SetupTransformPanel function.
            mLbPosX.Name = "mLbPosX";
            mLbPosY.Name = "mLbPosY";
            mLbPosZ.Name = "mLbPosZ";

            mLbRotX.Name = "mLbRotX";
            mLbRotY.Name = "mLbRotY";
            mLbRotZ.Name = "mLbRotZ";

            mLbScaleX.Name = "mLbScaleX";
            mLbScaleY.Name = "mLbScaleY";
            mLbScaleZ.Name = "mLbScaleZ";

            // Numeric Up-Downs
            // These are setup in the SetupTransformPanel function.
            mPosX.Name = "mPosX";
            mPosX.TabIndex = 11;

            mPosY.Name = "mPosY";
            mPosY.TabIndex = 12;

            mPosZ.Name = "mPosZ";
            mPosZ.TabIndex = 13;

            mRotZ.Name = "mRotZ";
            mRotZ.TabIndex = 21;

            mRotY.Name = "mRotY";
            mRotY.TabIndex = 21;

            mRotX.Name = "mRotX";
            mRotX.TabIndex = 21;

            mScaleZ.Name = "mScaleZ";
            mScaleZ.TabIndex = 27;

            mScaleY.Name = "mScaleY";
            mScaleY.TabIndex = 27;

            mScaleX.Name = "mScaleX";
            mScaleX.TabIndex = 27;


            // Panels
            SetupTransformPanel(mPosPanel, 90, 20 + _yOffset,   ContentWidth, mLbPosX, mLbPosY, mLbPosZ, mPosX, mPosY, mPosZ);
            SetupTransformPanel(mRotPanel, 90, 55 + _yOffset,   ContentWidth, mLbRotX, mLbRotY, mLbRotZ, mRotX, mRotY, mRotZ);
            SetupTransformPanel(mScalePanel, 90, 90 + _yOffset, ContentWidth, mLbScaleX, mLbScaleY, mLbScaleZ, mScaleX, mScaleY, mScaleZ);

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

        private void OnTransformPanelResize(object sender, EventArgs e)
        {
            // This assumes that the first 3 children of the panel are the Labels, and the 
            // last three are the X, Y, and Z Numeric Up-Downs in order.
            Panel panel = sender as Panel;
            Label lx = panel.Controls[0] as Label,
                ly = panel.Controls[1] as Label,
                lz = panel.Controls[2] as Label;
            NumericUpDown nx = panel.Controls[3] as NumericUpDown,
                ny = panel.Controls[4] as NumericUpDown,
                nz = panel.Controls[5] as NumericUpDown;
            int ContentWidth = (panel.Size - panel.Margin.Size - panel.Padding.Size).Width;
            int nudSize = (ContentWidth - (lx.Size.Width * 3)) / 3;
            System.Drawing.Size size = new System.Drawing.Size();

            size.Height = nx.Height;
            size.Width = nudSize;

            // X Label
            lx.Left = 3;
            nx.Left = lx.Left + lx.Size.Width;

            // X Numeric Up-Down
            nx.Size = size;

            // Y Label
            ly.Left = 1 + nx.Right;
            ny.Left = ly.Left + ly.Size.Width;

            // Y Numeric Up-Down
            ny.Size = size;

            // Z Label
            lz.Left = ny.Right;
            nz.Left = lz.Left + lz.Size.Width;

            // Z Numeric Up-Down
            nz.Size = size;
        }
    }

}