﻿using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Diagnostics;

namespace LevelEditor
{
    public class TransformComponent : Component
    {
        protected Label mLbName, mLbPosition, mLbRotation, mLbScale;
        protected Label mLbPosX, mLbPosY, mLbPosZ;
        protected Label mLbRotX, mLbRotY, mLbRotZ;
        protected Label mLbScaleX, mLbScaleY, mLbScaleZ;

        protected TextBox mName;
        protected System.Drawing.Font mPlaceholderFont, mActiveFont;
        protected Panel mPosPanel, mRotPanel, mScalePanel;

        protected NumericUpDown mPosX, mPosY, mPosZ;
        protected NumericUpDown mRotX, mRotY, mRotZ;
        protected NumericUpDown mScaleX, mScaleY, mScaleZ;

        protected bool mNameIsPlaceholder = true;

        public TransformComponent(List<Component> _container, int _yOffset = 0) : base(_container, false)
        {
            if(_yOffset != 0)
            {
                Debug.Print("A transform component has a Y-Offset. Wat.");
            }

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
            int ContentWidth = (mGroupBox.Size - mGroupBox.Padding.Size - mGroupBox.Margin.Size).Width;

            // Fonts
            mPlaceholderFont = new System.Drawing.Font("Calibri", 8.25f, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            mActiveFont = new System.Drawing.Font("Calibri", 8.25f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));


            // Labels
            mLbPosX.Name = "mLbPosX";
            mLbPosY.Name = "mLbPosY";
            mLbPosZ.Name = "mLbPosZ";

            mLbRotX.Name = "mLbRotX";
            mLbRotY.Name = "mLbRotY";
            mLbRotZ.Name = "mLbRotZ";

            mLbScaleX.Name = "mLbScaleX";
            mLbScaleY.Name = "mLbScaleY";
            mLbScaleZ.Name = "mLbScaleZ";

            mLbName.AutoSize = true;
            mLbName.Location = new System.Drawing.Point(6, 23 + _yOffset);
            mLbName.Name = "mLbName";
            mLbName.Text = "Name";

            mLbPosition.AutoSize = true;
            mLbPosition.Location = new System.Drawing.Point(6, 55 + _yOffset);
            mLbPosition.Name = "mLbPosition";
            mLbPosition.Text = "Position";

            mLbRotation.AutoSize = true;
            mLbRotation.Location = new System.Drawing.Point(6, 80 + _yOffset);
            mLbRotation.Name = "mLbRotation";
            mLbRotation.Text = "Rotation";

            mLbScale.AutoSize = true;
            mLbScale.Location = new System.Drawing.Point(6, 105 + _yOffset);
            mLbScale.Name = "mLbScale";
            mLbScale.Text = "Scale";

            // Text Boxes
            mName.Location = new System.Drawing.Point(90, 20);
            mName.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
            mName.Size = new System.Drawing.Size(ContentWidth - mName.Left, 20);
            mName.GotFocus += OnNameGetFocus;
            mName.LostFocus += OnNameLoseFocus;


            // Panels
            SetupTransformPanel(mPosPanel, 90, 50 + _yOffset, ContentWidth, mLbPosX, mLbPosY, mLbPosZ, mPosX, mPosY, mPosZ);
            SetupTransformPanel(mRotPanel, 90, 75 + _yOffset, ContentWidth, mLbRotX, mLbRotY, mLbRotZ, mRotX, mRotY, mRotZ);
            SetupTransformPanel(mScalePanel, 90, 100 + _yOffset, ContentWidth, mLbScaleX, mLbScaleY, mLbScaleZ, mScaleX, mScaleY, mScaleZ);
            #endregion

            mGroupBox.Text = "Transform";
            mGroupBox.Size = mGroupBox.PreferredSize;
        }

        protected override void OnMenuClick_Reset(object sender, EventArgs e)
        {
            mName.Text = "Object Name...";
            mName.Font = mPlaceholderFont;
            mName.ForeColor = System.Drawing.SystemColors.ControlDark;

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

        protected void OnNameGetFocus(object sender, EventArgs e)
        {
            if (mNameIsPlaceholder)
            {
                mNameIsPlaceholder = false;
                mName.Font = mActiveFont;
                mName.ForeColor = System.Drawing.SystemColors.ControlText;
                mName.Text = "";
            }
        }

        protected void OnNameLoseFocus(object sender, EventArgs e)
        {
            if(String.IsNullOrWhiteSpace(mName.Text))
            {
                mNameIsPlaceholder = true;
                mName.Font = mPlaceholderFont;
                mName.ForeColor = System.Drawing.SystemColors.ControlDark;
                mName.Text = "Object Name...";
            }
        }
    }
}