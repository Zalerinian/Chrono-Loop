using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace LevelEditor
{
    public class TransformComponent : Component
    {
        protected Label mLbName, mLbPosition, mLbRotation, mLbScale;
        protected Label mLbPosX, mLbPosY, mLbPosZ;
        protected Label mLbRotX, mLbRotY, mLbRotZ;
        protected Label mLbScaleX, mLbScaleY, mLbScaleZ;

        protected TextBox mName;
        protected Panel mPosPanel, mRotPanel, mScalePanel;

        protected NumericUpDown mPosX, mPosY, mPosZ;
        protected NumericUpDown mRotX, mRotY, mRotZ;
        protected NumericUpDown mScaleX, mScaleY, mScaleZ;

        public TransformComponent(List<Component> _container) : base(_container)
        {
            #region Component Creation
            mLbName = new Label();
            mLbPosition = new Label();
            mLbRotation = new Label();
            mLbScale = new Label();

            mLbPosX = new Label();
            mLbPosY = new Label();
            mLbPosZ = new Label();
            mLbRotX = new Label();
            mLbRotY = new Label();
            mLbRotZ = new Label();
            mLbScaleX = new Label();
            mLbScaleY = new Label();
            mLbScaleZ = new Label();

            mName = new TextBox();
            mPosPanel = new Panel();
            mRotPanel = new Panel();
            mScalePanel = new Panel();

            mPosX = new NumericUpDown();
            mPosY = new NumericUpDown();
            mPosZ = new NumericUpDown();
            mRotX = new NumericUpDown();
            mRotY = new NumericUpDown();
            mRotZ = new NumericUpDown();
            mScaleX = new NumericUpDown();
            mScaleY = new NumericUpDown();
            mScaleZ = new NumericUpDown();

            mGroupBox.Controls.Add(mLbName);
            mGroupBox.Controls.Add(mLbPosition);
            mGroupBox.Controls.Add(mLbRotation);
            mGroupBox.Controls.Add(mLbScale);

            mGroupBox.Controls.Add(mName);
            mGroupBox.Controls.Add(mPosPanel);
            mGroupBox.Controls.Add(mRotPanel);
            mGroupBox.Controls.Add(mScalePanel);
            #endregion

            #region Component Setup
            mLbPosX.Name = "mLbPosX";
            mLbPosY.Name = "mLbPosY";
            mLbPosZ.Name = "mLbPosZ";

            mLbRotX.Name = "mLbRotX";
            mLbRotY.Name = "mLbRotY";
            mLbRotZ.Name = "mLbRotZ";

            mLbScaleX.Name = "mLbScaleX";
            mLbScaleY.Name = "mLbScaleY";
            mLbScaleZ.Name = "mLbScaleZ";

            //SetupTransformPanel(mPosPanel, 90, )
            #endregion
        }
    }
}